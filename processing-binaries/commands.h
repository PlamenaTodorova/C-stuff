#include <stdio.h> // for printf/scanf
#include <errno.h> //errno
#include <regex.h> // for regular exprecions
#include "fileOperations.h"

void createConfigFile(int argc, char * argv[]) {
	if ( argc % 2 != 1) {
		freeSegmentParameters();
		errx(1, "Not a valid number of parameters");
	}
	
	int fd = createFile(argv[1]);
	
	int numsegments = (argc - 2) / 2;
	
	for (int j = 0; j < numsegments; j++){
		segments[j].type=0;
		for (int i = 0; i < META_DATA; i++) {
			segments[j].meta_rest[i] = 0;
		}

		for (int i = 0; i < BYTE_SEGMENT_SIZE; i++) {
			segments[j].data.bytes[i] = 0;
		}
	}
	
	for (int i = 0; i < numsegments * 2; i+=2) {
		int segmentIndex;
		int readSize = sscanf(argv[i+3], "%d", &segmentIndex);

		if (errno != 0 || readSize == 0) {
			close(fd);
			freeSegmentParameters();
			err (6, "A parameter is expected to be a number but an error occured");
		}

		char* segmentType = argv[i+4];

		if (segmentIndex < 0 || segmentIndex > MAX_SEGMENTS) {
			close(fd);
			freeSegmentParameters();
			errx(9, "Invalid segment index");
		}

		if (strlen(segmentType) != 1) {
			close(fd);
			freeSegmentParameters();
			errx(8, "Invalid segment type");
		}
		
		switch(segmentType[0]) {
			case 't':
				segments[segmentIndex].type = 0;
				break;
			case 'n':
				segments[segmentIndex].type = 1;
				break;
			case 'b': 
				segments[segmentIndex].type = 2;
				break;
			default:
				close(fd);
				freeSegmentParameters();
				errx(8, "Invalid sedment type");
		}
	}

	writeFile(numsegments, fd);
	close(fd);
}

int checkRegex(char * reg, char * str){
	regex_t regex;
	int rv;

	rv = regcomp(&regex, reg, REG_EXTENDED);

	if ( rv != 0) {
		freeSegmentParameters();
		errx (7, "Couldn't create the regex");
	}

	if ( regexec(&regex, str, 0, NULL, 0) == 0) {
		return 1; // valid match
	}

	freeSegmentParameters();
	errx(5, "Invalid type of the parameter");
}

int checkIfValid(int par_index, char * arg) {
	struct parameter par = parameters[par_index];
	if (segmentType[par.segment_number] == WORD_SEGMENT_TYPE) {
		return checkRegex(par.regEx, arg);
	} else if (segmentType[par.segment_number] == NUMBER_SEGMENT_TYPE) {
		int numarg;
		int readSize = sscanf(arg, "%d", &numarg);

		if (errno != 0 || readSize == 0) {
			freeSegmentParameters();
			err (6, "A parameter is expected to be a number but an error occured");
		}

		switch (par.position) {
			case 0:
				for (int i = 0; i < VALID_SB_SIZE; i++) {
					if (numarg == validSerialBaudrate[i]) {
						return 1;
					}
				}
				return 0;
			case 1:
				for (int i = 0; i < VALID_AB_SIZE; i++) {
					if (numarg == validAudioBitrate[i]) {
						return 1;
					}
				}
				return 0;
			case 2:
				for (int i = 0; i < VALID_SP_SIZE; i++) {
					if (numarg == validSleepPeiod[i]) {
						return 1;
					}
				}
				return 0;
		}
	} else {
		if (strlen(arg) != 1) {
			freeSegmentParameters();
			errx(5, "Invalid type of the parameter");
		}

		switch (par.position) {
			case 0:
				for (int i = 0; i < VALID_PARITY_SIZE; i++) {
					if (arg[0] == validParity[i]) {
						return 1;
					}
				}
				return 0;
			case 1:
				for (int i = 0; i < VALID_DBIT_SIZE; i++) {
					if (arg[0] == validDataBit[i]) {
						return 1;
					}
				}
				return 0;
			case 2:
				for (int i = 0; i < VALID_SBIT_SIZE; i++) {
					if (arg[0] == validStopBit[i]) {
						return 1;
					}
				}
				return 0;
		}
	}

	return 0;
}

int getParameter(char * name) {
	for ( int i = 0; i < NUMBER_OF_PARAMETERS; i++) {
		if (strcmp(parameters[i].name, name) == 0){
			return i;
		}
	}

	freeSegmentParameters();
	errx(4, "Not a valid parameter name");
}

void setParameter(char * el_name, char * el_val) {
	int par_index = getParameter(el_name);
		
	//sheck if valid
	if (!checkIfValid(par_index, el_val)) {
		freeSegmentParameters();
		errx(5, "Invalid type of the parameter");
	}
		
	struct parameter par = parameters[par_index];

	if (segments[par.segment_number].type == 0) {
		strncpy(segments[par.segment_number].data.words[par.position], el_val, strlen(el_val));
	} else if (segments[par.segment_number].type == 1) {
		sscanf(el_val, "%d", &segments[par.segment_number].data.numbers[par.position]);
	} else {
		segments[par.segment_number].data.bytes[par.position] = el_val[0];
	}
}

int checkBit(int par_index){
	struct parameter par = parameters[par_index];
	struct segment seg = segments[par.segment_number];

	int meta_index = par.position / 8; //in which byte is the the parameter
	int8_t position = 7 - par.position % 8; //position it the byte

	int8_t define_position = 1 << position;

	return seg.meta_rest[meta_index] & define_position;
}

void setBit(int par_index, char * status) {
	struct parameter par = parameters[par_index];
	
	int meta_index = par.position / 8; //in which byte is the the parameter
	int8_t position = 7 - par.position % 8; //position it the byte

	int8_t change_status = 1 << position;

	if (status[0] == '1') {
		segments[par.segment_number].meta_rest[meta_index] = segments[par.segment_number].meta_rest[meta_index] | change_status; //or sets the bit to 1 or keep it 1
	} else if (status[0] == '0') {
		if(checkBit(par_index)) {
			segments[par.segment_number].meta_rest[meta_index] = segments[par.segment_number].meta_rest[meta_index] ^ change_status; //xor will work only if it is one before
		}
	} else {
		freeSegmentParameters();
		errx (3, "Invalid parameter of a command");
	}
}

void setBitCommand(int argc, char * argv[]) {//reset the start position of the file
	if (argc != 5) {
		freeSegmentParameters();
		errx (1, "Invalid number of parameters");
	}

	//check if valid parameter name
	int par_index = getParameter(argv[3]);

	//open readable and writable file
	int fd = openFile(argv[1], O_RDWR);
	int count = readFile(fd);

	setBit(par_index, argv[4]);

	//reset the start position of the file
	lseek(fd, 0, SEEK_SET);
	writeFile(count, fd);
	close(fd);	
}

void setParameterCommand(int argc, char * argv[]) {
	if (argc != 5) {
		freeSegmentParameters();
		errx (1, "Invalid number of parameters");
	}
	
	//chek if valid parameter name
	int index = getParameter(argv[3]);

	//open readable and writable file
	int fd = openFile(argv[1], O_RDWR);
	int count = readFile(fd);

	//set name (and bit)
	if (argv[2][1] == 's'){
		setParameter(argv[3], argv[4]);
		setBit(index, "1");
	} else {
		setParameter(argv[3], argv[4]);
	}

	//reset the start position of the file
	lseek(fd, 0, SEEK_SET);
	writeFile(count, fd);
	close(fd);
}

void printParameter(int par_index) {
	struct parameter par = parameters[par_index];
	struct segment seg = segments[par.segment_number];
	if (seg.type == 0) {
		printf("%s: %s\n", par.name, seg.data.words[par.position]);
	} else if (seg.type == 1) {
		printf("%s: %d\n", par.name, seg.data.numbers[par.position]);
	} else {
		printf("%s: %c\n", par.name, seg.data.bytes[par.position]);
	}
}

void getParameterCommand(int argc, char * argv[]) {
	if (argc != 4) {
		freeSegmentParameters();
		errx(1, "Invalid number of parameters");
	}

	//check valid parameter name
	int par_index = getParameter(argv[3]);

	//open and read file
	int fd = openFile(argv[1], O_RDONLY);
	readFile(fd);

	if (strcmp(argv[2], "-g")) {
		if (checkBit(par_index)){
			printParameter(par_index);
		}
	} else {
		printParameter(par_index);
	}

	close(fd);
}

void listParameter(char * name, char * command) {
	int index = getParameter(name);

	if (strcmp(command, "-L")) {
		if (checkBit(index)){
			printParameter(index);
		}
	} else {
		printParameter(index);
	}
}

void listAllCommand(int argc, char * argv[]) {
	//open file for readingOnly
	int fd = openFile(argv[1], O_RDONLY);
	readFile(fd);

	if (argc == 3) {
		//printing without parameters
		for (int i = 0; i < NUMBER_OF_PARAMETERS; i++) {
			listParameter(parameters[i].name, argv[2]);
		}
	} else {
		//printing when we have parameters
		for (int i = 3; i < argc; i++) {
			listParameter(argv[i], argv[2]);
		}
	}

	close(fd);
}

void printHelp() {
	printf("%s\n", "Program allowing the configuration of bluetooth devices");
	printf("%s\n", "Usage: program filename -s|-S|-g|-G|-l|-L|-b|-c [options]");
	printf("%s\n", "Usage: program filename -s parameter  -  sets the prameter with it's coresponding bit");
	printf("%s\n", "Usage: program filename -S parameter  -  sets the prameter without it's coresponding bit");
	printf("%s\n", "Usage: program filename -g parameter  -  gets the prameter if it's active");
	printf("%s\n", "Usage: program filename -G parameter  -  gets the prameter");
	printf("%s\n", "Usage: program filename -l [parameters] -  gets a list of prameters if they are active");
	printf("%s\n", "Usage: program filename -L [parameters] -  gets a list of prameters");
	printf("%s\n", "Usage: program filename -b parameter bit  -  (de)activates a parameters bit");
	printf("%s\n", "Usage: program filename -c [options]  -  creates a new configuration file with a specific set of segments");
	printf("%s\n", "Usage: program -h  -  help");
}