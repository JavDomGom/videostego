#ifndef LIBCOMMON_H
#define LIBCOMMON_H

#define _GNU_SOURCE

#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define PROGRAM_NAME "VideoStego"
#define PROGRAM_VERSION "1.0.1"
#define AUTHORS "Javier Dominguez Gomez"

void printUsage ();
void help ();
char *getBasename (char*);
char *intToBin (unsigned int);

#endif  /* LIBCOMMON_H */