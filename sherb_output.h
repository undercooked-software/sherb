
#ifndef SHERB_OUTPUT_H
#define SHERB_OUTPUT_H

#define SHERB_HEADER \
  "\t\t\tSH:ell E:mpties R:ecycle B:in\
  \n\t\t\t  Copyright (C) 2021 - 2022\
  \nSHERB 1.1.0\t\t    Undercooked Software\t\tMarch 1st, 2022\n\n"
#define SHERB_USAGE \
  "Usage: sherb(.exe) [-hVL] [-q] [-d a,b,...]\n"
#define SHERB_BAD_DDR_LEN \
  "ERROR: Your delimited drive list contains more than 51 characters.\n"
#define SHERB_MALFORMED_DDR \
  "ERROR: Your delimited drive list is malformed.\n"
#define SHERB_HELP \
  "\nCommands:\
  \n  -h\thelp information\
  \n  -V\tdisplay version number\
  \n  -L\tdisplay software license\
  \nOptions:\
  \n  -q\tbe quiet\
  \n  -d\tempty bin of files from specified drives\n\n"
#define SHERB_LICENSE \
   "  This program may be used freely, and you are welcome to\
  \n  redistribute it under certain conditions.\n\
  \n  This program is distributed in the hope that it will be useful,\
  \n  but WITHOUT ANY WARRANTY. See the included License for more details.\n\
  \n  You should have received a copy of the SHERB License Agreement\
  \n  along with this program; see the file LICENSE.\
  \n  If not, please visit the Github repository below:\n\
  \n\thttps://github.com/undercooked-software/sherb\n"
#define SHERB_VERSION \
  "sherb 1.1.0\
  \nCopyright (C) 2021-2022 Samantha Bartlett\
  \nCopyright (C) 2021-2022 Undercooked Software\n"
#define SHERB_WARRANTY \
  "SHERB comes with ABSOLUTELY NO WARRANTY; for details type 'sherb -L'.\n"

#endif /* SHERB_OUTPUT_H */
