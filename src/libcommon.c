#include "libcommon.h"
#include "libvideostego.h"
#include <unistd.h>
#include<stdio.h> 

#if defined(__LP64__) || defined(_LP64)
# define BUILD_64   1
#endif

#ifdef BUILD_64
# define BITS_PER_LONG 64
#else
# define BITS_PER_LONG 32
#endif

/*
 * Procedure: printUsage
 * ----------------------------
 *   Procedure for printing the usage of this program.
 */
void printUsage ()
{
	fputs (("USAGE\n"), stdout);
	printf ("\t%s [-f filename] [-w [-m message] | -r | -i] [-h] [-v]\n", PROGRAM_NAME);
}

/*
 * Procedure: help
 * ----------------------------
 *   Procedure for printing the help of this program.
 */
void help ()
{
	printUsage ();
	fputs (("\nDESCRIPTION\n"), stdout);
	fputs (("\n\
\tTool to apply steganography in video files.\n"), stdout);
	fputs (("\nOPTIONS\n"), stdout);
	fputs (("\n\
\t-f, --file\tInput video file.\n\
\t-m, --message\tMessage to hide. It has to be specified only if the mode is -w, --write.\n\
\t-w, --write\tWrite mode. Hide a secret message in video file.\n\
\t-r, --read\tRead mode. Read a secret message in video file.\n\
\t-i, --info\tInfo mode. Print general structure and containers of video file.\n\
\t-v, --version\tPrint the program version.\n\
\t-h, --help\tPrint this help.\n"), stdout);
	fputs (("\nLICENSE\n"), stdout);
	fputs (("\n\
\tLicense GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>\n\
\tThis is free software: you are free to change and redistribute it.\n\
\tThere is NO WARRANTY, to the extent permitted by law.\n"), stdout);
}

/*
 * Function: getBasename
 * ----------------------------
 *   Returns file base name from absolute path.
 *   
 *   Params:
 *      filename: Absolute path to file.
 *
 *   Return: File base name.
 */
char *getBasename (char *filename)
{
	return basename (strdup (filename));
}

/*
 * Function: intToBin
 * ----------------------------
 *   Returns an 8-bit string representing an integer.
 *   
 *   Params:
 *      number: Number to convert from integer to bit string.
 *
 *   Return: String of 8 bits.
 */
char *intToBin (unsigned int number)
{
	static char s[BITS_PER_LONG + 1] = {0};
    char *p = s + BITS_PER_LONG;
    register size_t i;

    if (!number) return "00000000";

    for (i = 0; i < 8; i++)
		*(--p) = (number>>i & 1) ? '1' : '0';

    return p;
}
