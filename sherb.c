
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <windows.h>

// WIN32_LEAN_AND_MEAN undefines shellapi.h definitions. Redefine what we need.
#define SHERB_NOCONFIRMATION  1
#define SHERB_NOPROGRESSUI    2
#define SHERB_NOSOUND         4

#define stringify(x) stringify_(x)
#define stringify_(x) #x

#define SHEmptyRecycleBin stringify(SHEmptyRecycleBinA)
#ifdef UNICODE
# undef SHEmptyRecycleBin
# define SHEmptyRecycleBin stringify(SHEmptyRecycleBinW)
#endif

// https://stackoverflow.com/a/2348447 - WINAPI is __stdcall
// https://tinyurl.com/yckp42s2 - EmptyRecycleBin returns HRESULT
// https://stackoverflow.com/a/46457146 - LPCTSTR changes on UNICODE define
typedef HRESULT (WINAPI *PSHERB)(HWND, LPCTSTR, DWORD);

int __stdcall mainCRTStartup(void) {
  ExitProcess(0);
}

int
main(int argc, char ** argv) {
  HMODULE sh32;
  PSHERB pSHERB;
  DWORD dwFlags;
  
  if (argc > 1) {
    if ((!strcmp(argv[1], "-v")) || (!strcmp(argv[1], "--version"))) {
      return puts("SHERB 1.0.0");
    }

    if (!strcmp(argv[1], "--quiet")) {
      dwFlags |= SHERB_NOSOUND;
    }
  }

  sh32 = LoadLibrary(TEXT("shell32.dll"));
  if (!sh32)
    return puts("Failed to locate shell32.dll within System32.");

  pSHERB = (PSHERB)GetProcAddress(sh32, SHEmptyRecycleBin);
  if (!pSHERB)
    return puts("Could not get the function address for SHEmptyRecycleBin.");
 
  dwFlags |= SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI;
  pSHERB(0, 0, dwFlags);

  // We'll need to iterate across multiple drives in the future.
  // We need to implement SHQueryRecycleBin
  //pSHERB(0, TEXT("C:/"), dwFlags);
  
  mainCRTStartup();
}
