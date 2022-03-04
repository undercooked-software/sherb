
#ifndef SHERB_H
#define SHERB_H

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

/* WIN32_LEAN_AND_MEAN forces no inclusion of shellapi.h, among other things.
 * Redefine the few constants we need.
 */
#define SHERB_NOCONFIRMATION  1
#define SHERB_NOPROGRESSUI    2
#define SHERB_NOSOUND         4

/* https://stackoverflow.com/a/2348447 - WINAPI is __stdcall
 * https://tinyurl.com/yckp42s2 - EmptyRecycleBin returns HRESULT
 * https://stackoverflow.com/a/46457146 - LPCTSTR changes on UNICODE define
 */
typedef HRESULT   (WINAPI *pSHEmptyRecycleBin)(HWND, LPCTSTR, DWORD);
typedef LPWSTR *  (WINAPI *pCommandLineToArgvW)(LPCWSTR, int *);

global HMODULE  SHERB_SHELL32;

#endif /* SHERB_H */
