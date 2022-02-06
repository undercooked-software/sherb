
/*
 * Usage: sherb(.exe) [-hVL] [-q] [-d a,b,...]
 * Max possible command length is 73chars
 */


#if 1 // DEBUGGING
#undef UNICODE
#endif

#define global static

#define MAKE_CHARS(x)   MAKE_CHARS_(x)
#define MAKE_CHARS_(x)  #x

#ifdef UNICODE
# define SHERB_main           wmain
# define SHERB_mainCRTStartup wmainCRTStartup
# define CommandLineToArgv    CommandLineToArgvW
# define SHEmptyRecycleBin    SHEmptyRecycleBinW
#else
# define SHERB_main           main
# define SHERB_mainCRTStartup mainCRTStartup
# define CommandLineToArgv    CommandLineToArgvA_wine
# define SHEmptyRecycleBin    SHEmptyRecycleBinA
#endif

// <windows.h> can't compile with /Za flag to enforce c89 standard.
// The only work around I found was to create separate compilation units.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// WIN32_LEAN_AND_MEAN forces no inclusion of shellapi.h, among other things.
// Redefine the few constants we need.
#define SHERB_NOCONFIRMATION  1
#define SHERB_NOPROGRESSUI    2
#define SHERB_NOSOUND         4

// https://stackoverflow.com/a/2348447 - WINAPI is __stdcall
// https://tinyurl.com/yckp42s2 - EmptyRecycleBin returns HRESULT
// https://stackoverflow.com/a/46457146 - LPCTSTR changes on UNICODE define
typedef HRESULT   (WINAPI *pSHEmptyRecycleBin)(HWND, LPCTSTR, DWORD);
typedef LPWSTR *  (WINAPI *pCommandLineToArgvW)(LPCWSTR, int *);

global HANDLE   SHERB_OUTPUT_HANDLE;
global HMODULE  SHERB_SHELL32;

global LPCTSTR SHERB_USAGE = 
  TEXT("Usage: sherb(.exe) [-hVL] [-q] [-d a,b,...]\n");

void
SHERB_WriteConsole(LPCTSTR str) {
  WriteConsole(SHERB_OUTPUT_HANDLE, str, lstrlen(str), 0, 0);
}

// LPTSTR is equivalant to char* or wchar_t* depending on UNICODE define
int
SHERB_main(int argc, LPTSTR argv[]) {
  pSHEmptyRecycleBin sherb;
  DWORD dwFlags;
  int isQuiet = 0;

  if (argc > 1) {
    if (!lstrcmp(argv[1], TEXT("-V"))) {
      return SHERB_WriteConsole(TEXT("SHERB 1.0.0\n"));
    }
    if (!lstrcmp(argv[1], TEXT("-q"))) {
      isQuiet = 1;
    }
  }

  dwFlags = SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI;
  if (isQuiet)
    dwFlags |= SHERB_NOSOUND;

  sherb =
    (pSHEmptyRecycleBin)GetProcAddress(SHERB_SHELL32, 
                                       MAKE_CHARS(SHEmptyRecycleBin));
  if (!sherb)
    return GetLastError();

  sherb(0, 0, dwFlags);

  // TODO: See if we can figure out a way to avoid this sleep. Threading? Fork?
  if (!isQuiet)
    Sleep(1000);
}

// TODO(sammynilla): Write our own custom solution based on our data needs.
// The version taken here is modified from the WINE project.
// It's a modification of the CommandLineToArgvW implementation.
#ifndef UNICODE
  #include "CommandLineToArgvA.c"
#endif

int
_init_args(LPTSTR **argv) {
  int argc = 0;

#ifdef UNICODE
  pCommandLineToArgvW CommandLineToArgvW;
  CommandLineToArgvW =
    (pCommandLineToArgvW)GetProcAddress(SHERB_SHELL32,
                                        MAKE_CHARS(CommandLineToArgv));
  if (!CommandLineToArgvW)
    return GetLastError();
#endif

  *argv = CommandLineToArgv(GetCommandLine(), &argc);
  return argc;
}

int __cdecl
SHERB_mainCRTStartup(void) {
  LPTSTR *argv;
  int argc;
  int ret;

  SHERB_OUTPUT_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
  if ((!SHERB_OUTPUT_HANDLE) && (SHERB_OUTPUT_HANDLE != INVALID_HANDLE_VALUE))
    ExitProcess(GetLastError());

  SHERB_SHELL32 = LoadLibrary(TEXT("shell32.dll"));
  if (!SHERB_SHELL32)
    ExitProcess(GetLastError());

  argv = 0;
  argc = _init_args(&argv);
  if (!argc)
    ExitProcess(GetLastError());
  ret = SHERB_main(argc, argv);
  LocalFree(argv); // Might not be necessary.
  ExitProcess(ret);
}
