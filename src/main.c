#include "libcommon.h"
#include "libvideostego.h"

int main (int argc, char* argv[])
{
	int opt;
	char *file = "";
	char *msg = "";
	char mode = '\0';

	while (1)
	{
		static struct option long_options[] =
		{
			{"file", required_argument, 0, 'f'},
			{"message",  required_argument, 0, 'm'},
			{"write",  no_argument, 0, 'w'},
			{"read",  no_argument, 0, 'r'},
			{"info",  no_argument, 0, 'i'},
			{"help", no_argument, 0, 'h'},
			{"version", no_argument, 0, 'v'}
		};

		/* getopt_long stores the option index here. */
		int option_index = 0;

		opt = getopt_long (argc, argv, "f:m:wrihv", long_options, &option_index);

		/* Detect the end of the options. */
		if (opt == -1) break;

		switch (opt)
		{
			case 'f':
				file = optarg;
				break;
			case 'm':
				msg = optarg;
				break;
			case 'w':
			case 'r':
			case 'i':
				if (mode)
				{
					printf ("You cannot indicate more than one mode, you must choose between -w, -r or -i.\n");
					exit (EXIT_FAILURE);
				}

				mode = opt;
				break;
			case 'v':
				printf ("%s v%s\n", PROGRAM_NAME, PROGRAM_VERSION);
				exit (EXIT_SUCCESS);
			case 'h':
				help ();
				exit (EXIT_SUCCESS);
			case '?':
				if (optopt == 'f' || optopt == 'm') printUsage ();
				else if (isprint (optopt)) fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				exit (EXIT_FAILURE);
			default:
				abort ();
		}
	}

	if (optind < argc)
	{
		printf ("non-option ARGV-elements: ");
		while (optind < argc) printf ("%s", argv[optind++]);
		putchar ('\n');
		printUsage ();
		exit (EXIT_FAILURE);
	}

	if (!*file || !mode || (mode == 'w' && !*msg))
	{
		if (!*file) printf ("Option -f, --file is mandatory!\n");
		if (mode == 'w' && !*msg) printf ("Option -m, --message is mandatory!\n");
		if (!mode) printf ("Options -w, --write or -r, --read or -i, --info  are mandatory!\n");
		printUsage ();
		exit (EXIT_FAILURE);
	} else {
		FILE *openFile;

		if (mode == 'w')
		{
			openFile = fopen (file, "r+b");
		} else if (mode == 'r' || mode == 'i') {
			openFile = fopen (file, "rb");
		}

		if (openFile == NULL)
		{
			printf ("Error: Cannot read file %s.\n", file);
			exit (EXIT_FAILURE);
		} else {
			processFile (openFile, mode, msg);
		}
		
		fclose (openFile);
	}

	exit (EXIT_SUCCESS);
}
