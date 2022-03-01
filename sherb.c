
#include "base_types.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "sherb.h"
#include "sherb_output.h"

int
SHERB_WriteConsole(LPCTSTR str) {
  return WriteConsole(SHERB_OUTPUT_HANDLE, str, lstrlen(str), 0, 0);
}

/* LPTSTR is equivalant to char* or wchar_t* depending on UNICODE define */
int
SHERB_main(int argc, LPTSTR argv[]) {
  enum { AB_LEN = 26, DDR_LEN = 51 };
  pSHEmptyRecycleBin sherb;
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
      SHERB_WriteConsole(SHERB_HEADER);
      SHERB_WriteConsole(SHERB_USAGE);
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
      /* arg[1] is -d */
      driveFlag = 1;
      ++index;
    }

    /* Extra args we don't want. */
    if (driveFlag) {
      if (index+1 != argc)
        return SHERB_WriteConsole(SHERB_USAGE);
      /* Check if delimited drive string contains more characters than permitted. */
      ddrLen = lstrlen(argv[index]);
      if (ddrLen > DDR_LEN)
        return SHERB_WriteConsole(SHERB_BAD_DDR_LEN);
      /* Let's santize our string */
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
                                       stringify(SHEmptyRecycleBin));
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
      while (*d) {
        path[0] = *d;
        sherb(0, path, dwFlags | SHERB_NOSOUND);
        d++;
      }
    }
  }

  /* TODO(sammynilla): Figure out how to avoid this sleep. Threading? Fork? */
  if (!quietFlag)
    Sleep(1000);

  return 0;
}

/* TODO(sammynilla): Write our own custom solution based on our data needs.
 * The version taken here is modified from the WINE project.
 * It's a modification of the CommandLineToArgvW implementation.
 */
#ifndef UNICODE
  #include "CommandLineToArgvA.c"
#endif

int
_init_args(LPTSTR **argv) {
  int argc;

#ifdef UNICODE
  pCommandLineToArgvW CommandLineToArgvW;
  CommandLineToArgvW =
    (pCommandLineToArgvW)GetProcAddress(SHERB_SHELL32,
                                        stringify(CommandLineToArgv));
  if (!CommandLineToArgvW)
    return 0;
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
  /* No LocalFree because we perform one task and then kill the process. */
  ExitProcess(ret);
}
