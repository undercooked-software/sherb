
#define global static
#define i32 int
#define b32 int
#define elif else if

#define MAKE_CHARS(x)   MAKE_CHARS_(x)
#define MAKE_CHARS_(x)  #x

#define IS_DELIMETER(c)   (c == ',')
#define IS_TERMINATOR(c)  (c == '\0')

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

// <windows.h> can't compile with /Za flag to enforce c89.
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
global LPCTSTR SHERB_BAD_DDR_LEN =
  TEXT("ERROR: Your delimited drive list contains more than 51 characters.\n");
global LPCTSTR SHERB_MALFORMED_DDR =
  TEXT("ERROR: Your delimited drive list is malformed.\n");
global LPCTSTR SHERB_HELP =
  TEXT("Help Text\n");
global LPCTSTR SHERB_LICENSE =
  TEXT("License Text\n");
global LPCTSTR SHERB_VERSION =
  TEXT("SHERB 1.0.0\n");

void
SHERB_WriteConsole(LPCTSTR str) {
  WriteConsole(SHERB_OUTPUT_HANDLE, str, lstrlen(str), 0, 0);
}

// LPTSTR is equivalant to char* or wchar_t* depending on UNICODE define
int
SHERB_main(int argc, LPTSTR argv[]) {
  pSHEmptyRecycleBin sherb;
  enum { AB_LEN = 26, DDR_LEN = 51 };
  TCHAR drive[AB_LEN] = {0};
  b32 quietFlag = 0;
  b32 driveFlag = 0;
  i32 ddrLen;

  if (argc > 4)
    return SHERB_WriteConsole(SHERB_USAGE);

  if (argc > 1) {
    i32 index = 1;
    if (!lstrcmp(argv[index], TEXT("-V"))) {
      return SHERB_WriteConsole(SHERB_VERSION);
    } elif (!lstrcmp(argv[index], TEXT("-L"))) {
      return SHERB_WriteConsole(SHERB_LICENSE);
    } elif (!lstrcmp(argv[index], TEXT("-h"))) {
      return SHERB_WriteConsole(SHERB_HELP);
    } elif (!lstrcmp(argv[index], TEXT("-q"))) {
      quietFlag = 1;
      ++index;
      if (index != argc) {
        if (!lstrcmp(argv[index], TEXT("-d"))) {
          driveFlag = 1;
          ++index;
        } else {
          return SHERB_WriteConsole(SHERB_USAGE);
        }
      }
    } else {
      if (lstrcmp(argv[index], TEXT("-d")) != 0)
        return SHERB_WriteConsole(SHERB_USAGE);
      // arg[1] is -d
      driveFlag = 1;
      ++index;
    }

    // Extra args we don't want.
    if (driveFlag) {
      if (index+1 != argc)
        return SHERB_WriteConsole(SHERB_USAGE);
      // Check if delimited drive string contains more characters than permitted.
      ddrLen = lstrlen(argv[index]);
      if (ddrLen > DDR_LEN)
        return SHERB_WriteConsole(SHERB_BAD_DDR_LEN);
      // Let's santize our string
      {
        i32 c, i;
        c = i = 0;
        for (; c < ddrLen; ++c) {
          switch ((c % 2)) {
            case 0: {
              if (!IsCharAlpha(argv[index][c]))
                return SHERB_WriteConsole(SHERB_MALFORMED_DDR);
              drive[i] = argv[index][c];
              ++i;
            } break;
            case 1: {
              if (!IS_DELIMETER(argv[index][c]))
                return SHERB_WriteConsole(SHERB_MALFORMED_DDR);
            } break;
          }
        }
      }
    }
  }

  sherb =
    (pSHEmptyRecycleBin)GetProcAddress(SHERB_SHELL32,
                                       MAKE_CHARS(SHEmptyRecycleBin));
  if (!sherb)
    return GetLastError();

  {
    TCHAR *d = drive;
    TCHAR path[4] = { '?', ':', '\\', '\0', };
    const DWORD dwFlags = SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI;
    if (IS_TERMINATOR(drive[0])) {
      sherb(0, 0, (quietFlag) ? (dwFlags | SHERB_NOSOUND) : dwFlags); 
    } else {
      path[0] = *d;
      sherb(0, path, (quietFlag) ? (dwFlags | SHERB_NOSOUND) : dwFlags);
      d++;
      while (*d && !IS_TERMINATOR(*d)) {
        path[0] = *d;
        sherb(0, path, dwFlags | SHERB_NOSOUND);
        d++;
      }
    }
  }

  // TODO(sammynilla): Figure out how to avoid this sleep. Threading? Fork?
  if (!quietFlag)
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
    return argc; // argc is 0 here.
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
  /* NOTE (sammynilla):
   * CommandLineToArgv(A/W) both use LocalAlloc to designate a contiginous block
   * of memory that is used store our argument strings.
   * LocalFree is used to free that memory, but doesn't a program that finishes
   * its execution already release it's own memory? It may not be necessary.
   */
  LocalFree(argv);
  ExitProcess(ret);
}
