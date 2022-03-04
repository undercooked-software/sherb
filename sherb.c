
#include "base_types.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "sherb.h"
#include "sherb_data.h"
#include "terminal.h"

/* LPTSTR is equivalant to char* or wchar_t* depending on UNICODE define */
int
SHERB_main(int argc, LPTSTR argv[]) {
  enum { MAX_ARGS = 4, AB_LEN = 26, DDR_LEN = 51 };
  pSHEmptyRecycleBin sherb;
  TCHAR drive[AB_LEN] = {0};
  b32 quietFlag = 0;
  b32 driveFlag = 0;
  i32 ddrLen;

  if (argc > MAX_ARGS)
    return term_puts(SHERB_USAGE);

  if (argc > 1) {
    i32 index = 1;
    if (!lstrcmp(argv[index], TEXT("-V"))) {
      term_puts(SHERB_VERSION);
      return term_puts(SHERB_WARRANTY);
    } elif (!lstrcmp(argv[index], TEXT("-L"))) {
      term_cfputs(9,
        FG_WHITE, SHERB_HEADER_TITLE,
        FG_YELLOW, SHERB_HEADER_DEVELOPER,
        FG_WHITE, SHERB_HEADER_VERSION, SHERB_HEADER_COPYRIGHT, SHERB_HEADER_RELEASE,
        FG_RESET
      );
      term_puts(SHERB_LICENSE);
      return term_cfputs(3, FG_CYAN, SHERB_REPOSITORY, FG_RESET);
    } elif (!lstrcmp(argv[index], TEXT("-h"))) {
      term_cfputs(9,
        FG_WHITE, SHERB_HEADER_TITLE,
        FG_YELLOW, SHERB_HEADER_DEVELOPER,
        FG_WHITE, SHERB_HEADER_VERSION, SHERB_HEADER_COPYRIGHT, SHERB_HEADER_RELEASE,
        FG_RESET
      );
      term_puts(SHERB_USAGE);
      term_cfputs(3, FG_LIGHTGREEN, "\nCommands:", FG_RESET);
      term_puts(SHERB_HELP_COMMANDS);
      term_cfputs(3, FG_LIGHTGREEN, "\nOptions:",  FG_RESET);
      term_puts(SHERB_HELP_OPTIONS);
      return term_puts(SHERB_WARRANTY);
    } elif (!lstrcmp(argv[index], TEXT("-q"))) {
      quietFlag = 1;
      ++index;
      if (index != argc) {
        if (lstrcmp(argv[index], TEXT("-d")) != 0)
          return term_puts(SHERB_USAGE);
        driveFlag = 1;
        ++index;
      }
    } else {
      if (lstrcmp(argv[index], TEXT("-d")) != 0)
        return term_puts(SHERB_USAGE);
      driveFlag = 1;
      ++index;
    }

    if (driveFlag) {
      if (index == argc)
        return term_cfputs(4, FG_LIGHTRED, "Error: ",
                              FG_RESET, SHERB_MISSING_DDR);
      /* Check if delimited drive string contains too many characters. */
      ddrLen = lstrlen(argv[index]);
      if (ddrLen > DDR_LEN)
        return term_cfputs(4, FG_LIGHTRED, "Error: ",
                              FG_RESET, SHERB_BAD_DDR_LEN);
      /* Process and sanitize our argument drive list. */
      {
        i32 c, i;
        c = i = 0;
        for (; c < ddrLen; ++c) {
          switch ((c % 2)) {
            case 0: {
              if (!IsCharAlpha(argv[index][c]))
                return term_cfputs(4, FG_LIGHTRED, "Error: ",
                                      FG_RESET, SHERB_MALFORMED_DDR);
              drive[i] = argv[index][c];
              ++i;
            } break;
            case 1: {
              if (!IS_DELIMETER(argv[index][c]))
                return term_cfputs(4, FG_LIGHTRED, "Error: ",
                                      FG_RESET, SHERB_MALFORMED_DDR);
            } break;
          }
        }
      }
    }
  }

  sherb = (pSHEmptyRecycleBin)GetProcAddress(SHERB_SHELL32,
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

  term.output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  if ((!term.output_handle) && (term.output_handle != INVALID_HANDLE_VALUE))
    ExitProcess(GetLastError());

  {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(term.output_handle, &csbi);
    term.base_style = csbi.wAttributes;
  }

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
