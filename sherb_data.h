
#ifndef SHERB_DATA_H
#define SHERB_DATA_H

#include "terminal.h"

#define SHERB_LATEST_RELEASE  "March 4th, 2022"
#define SHERB_VERSION_NUMBER  "1.2.0"
#define SHERB_DEVELOPER       "Undercooked Software"
#define SHERB_COPYRIGHT       "Copyright (C) 2021 - 2022"

/* Application Header */
#define SHERB_HEADER_TITLE      "\t\t\tSH:ell E:mpties R:ecycle B:in\n"
#define SHERB_HEADER_DEVELOPER  "\t\t\t   (" SHERB_DEVELOPER ")\n"
#define SHERB_HEADER_VERSION    "SHERB " SHERB_VERSION_NUMBER "\t\t  "
#define SHERB_HEADER_COPYRIGHT  SHERB_COPYRIGHT "\t\t"
#define SHERB_HEADER_RELEASE    SHERB_LATEST_RELEASE "\n\n"

/* Error Strings */
#define SHERB_USAGE \
  "Usage: sherb(.exe) [-hVL] [-q] [-d a,b,...]\n"
#define SHERB_MISSING_DDR \
  "-d was specified but no delimited drive list was passed.\n"
#define SHERB_BAD_DDR_LEN \
  "Your delimited drive list contains more than 51 characters.\n"
#define SHERB_MALFORMED_DDR \
  "Your delimited drive list is malformed.\n"

#define SHERB_HELP_COMMANDS \
  "\n  -h\thelp information \
   \n  -V\tdisplay version number \
   \n  -L\tdisplay software license\n"
#define SHERB_HELP_OPTIONS \
  "\n  -q\tbe quiet \
   \n  -d\tempty bin of files from specified drives\n\n"

#define SHERB_LICENSE \
   "  This program may be used freely, and you are welcome to \
  \n  redistribute it under certain conditions.\n \
  \n  This program is distributed in the hope that it will be useful, \
  \n  but WITHOUT ANY WARRANTY. See the included License for more details.\n \
  \n  You should have received a copy of the SHERB License Agreement \
  \n  along with this program; see the file LICENSE. \
  \n  If not, please visit the Github repository below:\n\n\t"
#define SHERB_REPOSITORY "https://github.com/undercooked-software/sherb\n"
#define SHERB_VERSION \
  "sherb " SHERB_VERSION_NUMBER \
  "\n" SHERB_COPYRIGHT " Samantha Bartlett"\
  "\n" SHERB_COPYRIGHT " " SHERB_DEVELOPER "\n"
#define SHERB_WARRANTY \
  "SHERB comes with ABSOLUTELY NO WARRANTY; for details type 'sherb -L'.\n"

#endif /* SHERB_DATA_H */
