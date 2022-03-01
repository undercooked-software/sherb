
#ifndef SHERB_OUTPUT_H
#define SHERB_OUTPUT_H

#include "base_types.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

global LPCTSTR SHERB_HEADER =
TEXT("\t\t\tSH:ell E:mpties R:ecycle B:in\
    \n\t\t\t  Copyright (C) 2021 - 2022\
    \nSHERB 1.1.0\t\t    Undercooked Software\t\tMarch 1st, 2022\n\n");
global LPCTSTR SHERB_USAGE =
TEXT("Usage: sherb(.exe) [-hVL] [-q] [-d a,b,...]\n");
global LPCTSTR SHERB_BAD_DDR_LEN =
TEXT("ERROR: Your delimited drive list contains more than 51 characters.\n");
global LPCTSTR SHERB_MALFORMED_DDR =
TEXT("ERROR: Your delimited drive list is malformed.\n");
global LPCTSTR SHERB_HELP =
TEXT("\nCommands:\
    \n  -h\thelp information\
    \n  -V\tdisplay version number\
    \n  -L\tdisplay software license\
    \nOptions:\
    \n  -q\tbe quiet\
    \n  -d\tempty bin of files from specified drives\n\n");
global LPCTSTR SHERB_LICENSE =
TEXT("License Text\n");
global LPCTSTR SHERB_VERSION =
TEXT("sherb 1.1.0\
    \nCopyright (C) 2021-2022 Samantha Bartlett\
    \nCopyright (C) 2021-2022 Undercooked Software\n");
global LPCTSTR SHERB_WARRANTY =
TEXT("SHERB comes with ABSOLUTELY NO WARRANTY; for details type 'sherb -L'.\n");

#endif /* SHERB_OUTPUT_H */
