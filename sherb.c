
/*
 * Usage: sherb(.exe) [-v | --version] [--quiet] [-d [drive_letters...]]
 * Max possible command length is 73chars
 */

#if 0 // DEBUGGING
#ifdef UNICODE
# undef UNICODE
#endif
#endif

#define global  static
#define i32     int

#define MAKE_CHARS(x)   MAKE_CHARS_(x)
#define MAKE_CHARS_(x)  #x

#ifdef UNICODE
# define SHERB_FUNC(x)  w##x
#else
# define SHERB_FUNC(x)  x
#endif

#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

// WIN32_LEAN_AND_MEAN undefines shellapi.h definitions. Redefine what we need.
//#define SHERB_NOCONFIRMATION  1
//#define SHERB_NOPROGRESSUI    2
//#define SHERB_NOSOUND         4

// https://stackoverflow.com/a/2348447 - WINAPI is __stdcall
// https://tinyurl.com/yckp42s2 - EmptyRecycleBin returns HRESULT
// https://stackoverflow.com/a/46457146 - LPCTSTR changes on UNICODE define
typedef HRESULT (WINAPI *PSHERB)(HWND, LPCTSTR, DWORD);

global HANDLE pCout;

// LPTSTR is equivalant to char* or wchar_t* depending on UNICODE define
int
SHERB_FUNC(main)(int argc, LPTSTR argv[]) {
  HMODULE sh32;
  PSHERB pSHERB;
  DWORD dwFlags;
  int isQuiet = 0;

  if (argc > 1) {
    //WriteConsole(pCout, argv[0], 9, 0, 0);
    printf("%ws\n", argv[0]);
    /*if ((!strcmp(argv[1], "-v")) || (!strcmp(argv[1], "--version"))) {
      return puts("SHERB 1.0.0");
    }*/

    /*if (!strcmp(argv[1], "--quiet")) {
      isQuiet = 1;
    }*/
  }

  sh32 = LoadLibrary(TEXT("shell32.dll"));
  if (!sh32) {
    puts("Failed to locate shell32.dll within System32.");
    return GetLastError();
  }

  pSHERB = (PSHERB)GetProcAddress(sh32, MAKE_CHARS(SHEmptyRecycleBin));
  if (!pSHERB) {
    puts("Could not get the function address for SHEmptyRecycleBin.");
    return GetLastError();
  }
  
  dwFlags = SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI;
  if (isQuiet)
    dwFlags |= SHERB_NOSOUND;

  pSHERB(0, 0, dwFlags);
  // We'll need to iterate across multiple drives in the future.
  // We need to implement SHQueryRecycleBin
  //pSHERB(0, TEXT("C:/"), SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI);

  Sleep(1000);
}

/*LPSTR *
CommandLineToArgvA(LPCSTR lpCmdLine, int *pNumArgs) {
  // This is not a proper implementation and is built specifically for SHERB.
  enum { kMaxArgs = 4 };
  //LPSTR 
}*/

int
_init_args(LPTSTR **argv) {
  int argc;
  // We need to write our own version of CommandLineToArgvA
  *argv = CommandLineToArgvW(GetCommandLine(), &argc);
  //*argv = args;
  return argc;
}

int __cdecl
SHERB_FUNC(mainCRTStartup)(void) {
  LPTSTR *argv;
  int argc;
  int ret;

  pCout = GetStdHandle(STD_OUTPUT_HANDLE);
  if ((!pCout) && (pCout != INVALID_HANDLE_VALUE)) {
    ExitProcess(GetLastError());
  }

  argv = 0;
  argc = _init_args(&argv);
  if (!argc) {
    // Somehow we have no arguments?
    ExitProcess(GetLastError());
  }
  ret = SHERB_FUNC(main)(argc, argv);
  LocalFree(argv); // Might not be necessary.
  ExitProcess(ret);
}
