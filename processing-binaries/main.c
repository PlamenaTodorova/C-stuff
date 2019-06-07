#include "commands.h"

int main(int argc, char * argv[]) {
	if ( argc < 2) {
		errx(1, "Invalid number of parameters");
	}

	if (argc == 2 && strcmp(argv[1], "-h")) {
		printHelp();
	} else if (argc == 2) {
		errx(2, "Not a valid command");
	}

	if (strlen(argv[2]) != 2 || argv[2][0] != '-') {
		errx(2, "Not a valid command");
	}

	generateSegmentParameters();

	//commands with 3 parameters
	switch(argv[2][1])
	{
		case 's':
			setParameterCommand(argc, argv);
			break;
		case 'S':
			setParameterCommand(argc, argv);
			break;
		case 'g':
			getParameterCommand(argc, argv);
			break;
		case 'G':
			getParameterCommand(argc, argv);
			break;
		case 'l':
			listAllCommand(argc, argv);
			break;
		case 'L':
			listAllCommand(argc, argv);
			break;
		case 'b':
			setBitCommand(argc, argv);
			break;
		case 'c':
			createConfigFile(argc, argv);
			break;
		default:
			freeSegmentParameters();
			errx(2, "Not a valid ncommand");
	}

	freeSegmentParameters();
}