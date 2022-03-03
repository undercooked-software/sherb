
#ifndef SHERB_DATA_H
#define SHERB_DATA_H

#include "vt_fmt.h"

#define SHERB_LATEST_RELEASE  "March 3rd, 2022"
#define SHERB_VERSION_NUMBER  "1.2.0"
#define SHERB_DEVELOPER       "Undercooked Software"

#define SHERB_HEADER_CONTENTS \
  "\t\t\tSH:ell E:mpties R:ecycle B:in\
  \n\t\t\t  Copyright (C) 2021 - 2022\
  \nSHERB " SHERB_VERSION_NUMBER "\t\t    "
#define SHERB_HEADER \
  FMT(PEA)    SHERB_HEADER_CONTENTS        FMT(PLAIN) \
  FMT(YELLOW) SHERB_DEVELOPER       "\t\t" FMT(PLAIN) \
  FMT(PEA)    SHERB_LATEST_RELEASE  "\n\n" FMT(PLAIN)

#define SHERB_USAGE \
  "Usage: sherb(.exe) [-hVL] [-q] [-d a,b,...]\n"
#define SHERB_BAD_DDR_LEN \
  FMT(RED) "ERROR: " FMT(PLAIN) \
  "Your delimited drive list contains more than 51 characters.\n"
#define SHERB_MALFORMED_DDR \
  FMT(RED) "ERROR: " FMT(PLAIN) \
  "Your delimited drive list is malformed.\n"
#define SHERB_HELP \
  FMT(PINK) "\nCommands:" FMT(PLAIN) \
  "\n  -h\thelp information\
   \n  -V\tdisplay version number\
   \n  -L\tdisplay software license" \
  FMT(PINK) "\nOptions:" FMT(PLAIN) \
  "\n  -q\tbe quiet\
   \n  -d\tempty bin of files from specified drives\n\n"
#define SHERB_LICENSE \
   "  This program may be used freely, and you are welcome to\
  \n  redistribute it under certain conditions.\n\
  \n  This program is distributed in the hope that it will be useful,\
  \n  but WITHOUT ANY WARRANTY. See the included License for more details.\n\
  \n  You should have received a copy of the SHERB License Agreement\
  \n  along with this program; see the file LICENSE.\
  \n  If not, please visit the Github repository below:\n\n\t"\
  FMT(CORNFLOWER) "https://github.com/undercooked-software/sherb\n" FMT(PLAIN)
#define SHERB_VERSION \
  "sherb " SHERB_VERSION_NUMBER \
  "\nCopyright (C) 2021-2022 Samantha Bartlett\
   \nCopyright (C) 2021-2022 " SHERB_DEVELOPER "\n"
#define SHERB_WARRANTY \
  "SHERB comes with ABSOLUTELY NO WARRANTY; for details type 'sherb -L'.\n"

#endif /* SHERB_DATA_H */
