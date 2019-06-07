#include <sys/types.h> // for open
#include <err.h>	// for err.. duh
#include <fcntl.h> // for open flags
#include <stdio.h> // for printf/scanf
#include <unistd.h> // for read
#include <string.h> // for strcpy
#include <errno.h> //errno
#include <regex.h> // for regular exprecions
#include <stdlib.h> // for malloc

#define META_DATA 7
#define WORD_SEGMENT_SIZE 3
#define WORD_SIZE 16
#define NUMBER_SEGMENT_SIZE 14
#define BYTE_SEGMENT_SIZE 56
#define MAX_SEGMENTS 64

#define NUMBER_OF_PARAMETERS 15

#define WORD_SEGMENT_TYPE 0
#define NUMBER_SEGMENT_TYPE 1
#define BYTE_SEGMENT_TYPE 2

#define DEVICE_NAME_SNAME "device_name"
#define DEVISE_NAME_SNUMBER 0
#define DEVICE_NAME_POSITION 0
#define DEVICE_NAME_REGEX "^[a-zA-Z0-9_-]{,15}$"

#define ROM_DEVISION_SNAME "rom_revision"
#define ROM_DEVISION_SNUMBER 0
#define ROM_DEVISION_POSITION 1
#define ROM_DEVISION_REGEX "^[a-zA-Z0-9_-.]{,15}$"

#define SERIAL_NUMBER_SNAME "serial_number"
#define SERIAL_NUMBER_SNUMBER 0
#define SERIAL_NUMBER_POSITION 2
#define SERIAL_NUMBER_REGEX "^[A-Z0-9]{,15}$"

#define BD_ADDR_PART0_SNAME "bd_addr_part0"
#define BD_ADDR_PART0_SNUMBER 1
#define BD_ADDR_PART0_POSITION 0
#define BD_ADDR_PART0_REGEX "^[A-Z0-9:]{,15}$"

#define BD_ADDR_PART1_SNAME "bd_addr_part1"
#define BD_ADDR_PART1_SNUMBER 1
#define BD_ADDR_PART1_POSITION 1
#define BD_ADDR_PART1_REGEX "^[A-Z0-9:]{,15}$"

#define BD_PASS_PART0_SNAME "bd_pass_part0"
#define BD_PASS_PART0_SNUMBER 1
#define BD_PASS_PART0_POSITION 2
#define BD_PASS_PART0_REGEX "^[a-z0-9]{,15}$"

#define SERIAL_BAUDRATE_SNAME "serial_baudrate"
#define SERIAL_BAUDRATE_SNUMBER 2
#define SERIAL_BAUDRATE_POSITION 0

#define AUDIO_BITRATE_SNAME "audio_bitrate"
#define AUDIO_BITRATE_SNUMBER 2
#define AUDIO_BITRATE_POSITION 1

#define SLEEP_PERIOD_SNAME "sleep_period"
#define SLEEP_PERIOD_SNUMBER 2
#define SLEEP_PERIOD_POSITION 2

#define SERIAL_PARITY_SNAME "serial_parity"
#define SERIAL_PARITY_SNUMBER 3
#define SERIAL_PARITY_POSITION 0

#define SERIAL_DATA_BIT_SNAME "serial_data_bit"
#define SERIAL_DATA_BIT_SNUMBER 2
#define SERIAL_DATA_BIT_POSITION 1

#define SERIAL_STOP_BIT_SNAME "serail_stop_bit"
#define SERIAL_STOP_BIT_SNUMBER 3
#define SERIAL_STOP_BIT_POSITION 2

#define BD_PASS_PART1_SNAME "bd_pass_part1"
#define BD_PASS_PART1_SNUMBER 4
#define BD_PASS_PART1_POSITION 0
#define BD_PASS_PART1_REGEX "^[a-z0-9]{,15}$"

#define ROM_CHECKSUM_PART0_SNAME "rom_checksum_part0"
#define ROM_CHECKSUM_PART0_SNUMBER 4
#define ROM_CHECKSUM_PART0_POSITION 1
#define ROM_CHECKSUM_PART0_REGEX "^[a-z0-9]{,15}$"

#define ROM_CHECKSUM_PART1_SNAME "rom_checksum_part1"
#define ROM_CHECKSUM_PART1_SNUMBER 4
#define ROM_CHECKSUM_PART1_POSITION 2
#define ROM_CHECKSUM_PART1_REGEX "^[a-z0-9]{,15}$"

#define FIRST_SEGMETN_TYPE 0
#define SECOND_SEGMETN_TYPE 0
#define THIRD_SEGMETN_TYPE 1
#define FORTH_SEGMETN_TYPE 2
#define FIFTH_SEGMETN_TYPE 0

#define VALID_USEFUL_SEGMENTS 5
const int segmentType[5] = {FIRST_SEGMETN_TYPE, SECOND_SEGMETN_TYPE, THIRD_SEGMETN_TYPE, FORTH_SEGMETN_TYPE, FIFTH_SEGMETN_TYPE};
#define VALID_SB_SIZE 6
const int validSerialBaudrate[VALID_SB_SIZE] = {1200, 2400, 4800, 9600, 19200, 115200};
#define VALID_AB_SIZE 7
const int validAudioBitrate[VALID_AB_SIZE] = {32, 96, 128, 160, 192, 256, 320};
#define VALID_SP_SIZE 5
const int validSleepPeiod[VALID_SP_SIZE] = {100, 500, 1000, 5000, 10000};

#define VALID_PARITY_SIZE 3
const char validParity[VALID_SB_SIZE] = {'N', 'E', 'O'};
#define VALID_DBIT_SIZE 4
const char validDataBit[VALID_AB_SIZE] = {5,6,7,8};
#define VALID_SBIT_SIZE 5
const char validStopBit[VALID_SP_SIZE] = {0, 1};

struct parameter
{
	char * name;
	int segment_number;
	int position;
	char * regEx; // valid only for text and byte segments
} parameters[15];

struct segment
{
	char type;
	char meta_rest[META_DATA];

	union 
	{
		char words[WORD_SEGMENT_SIZE][WORD_SIZE];
		int32_t numbers[NUMBER_SEGMENT_SIZE];
		char bytes[BYTE_SEGMENT_SIZE];
	} data;
} __attribute__((packed)) segments[MAX_SEGMENTS];

void generateSegmentParameters(){
	parameters[0].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[0].name, DEVICE_NAME_SNAME);
	parameters[0].segment_number = DEVISE_NAME_SNUMBER;
	parameters[0].position = DEVICE_NAME_POSITION;
	parameters[0].regEx = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[0].regEx, DEVICE_NAME_REGEX);
	
	parameters[1].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[1].name, ROM_DEVISION_SNAME);
	parameters[1].segment_number = ROM_DEVISION_SNUMBER;
	parameters[1].position = ROM_DEVISION_POSITION;
	parameters[1].regEx = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[1].regEx, ROM_DEVISION_REGEX);
	
	parameters[2].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[2].name, SERIAL_NUMBER_SNAME);
	parameters[2].segment_number = SERIAL_NUMBER_SNUMBER;
	parameters[2].position = SERIAL_NUMBER_POSITION;
	parameters[2].regEx = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[2].regEx, SERIAL_NUMBER_REGEX);

	parameters[3].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[3].name, BD_ADDR_PART0_SNAME);
	parameters[3].segment_number = BD_ADDR_PART0_SNUMBER;
	parameters[3].position = BD_ADDR_PART0_POSITION;
	parameters[3].regEx = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[3].regEx, BD_ADDR_PART0_REGEX);
	
	parameters[4].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[4].name, BD_ADDR_PART1_SNAME);
	parameters[4].segment_number = BD_ADDR_PART1_SNUMBER;
	parameters[4].position = BD_ADDR_PART1_POSITION;
	parameters[4].regEx = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[4].regEx, BD_ADDR_PART1_REGEX);
	
	parameters[5].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[5].name, BD_PASS_PART0_SNAME);
	parameters[5].segment_number = BD_PASS_PART0_SNUMBER;
	parameters[5].position = BD_PASS_PART0_POSITION;
	parameters[5].regEx = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[5].regEx, BD_PASS_PART0_REGEX);
	
	parameters[6].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[6].name, SERIAL_BAUDRATE_SNAME);
	parameters[6].segment_number = SERIAL_BAUDRATE_SNUMBER;
	parameters[6].position = SERIAL_BAUDRATE_POSITION;

	parameters[7].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[7].name, AUDIO_BITRATE_SNAME);
	parameters[7].segment_number = AUDIO_BITRATE_SNUMBER;
	parameters[7].position = AUDIO_BITRATE_POSITION;

	parameters[8].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[8].name, SLEEP_PERIOD_SNAME);
	parameters[8].segment_number = SLEEP_PERIOD_SNUMBER;
	parameters[8].position = SLEEP_PERIOD_POSITION;

	parameters[9].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[9].name, SERIAL_PARITY_SNAME);
	parameters[9].segment_number = SERIAL_PARITY_SNUMBER;
	parameters[9].position = SERIAL_PARITY_POSITION;
	
	parameters[10].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[10].name, SERIAL_DATA_BIT_SNAME);
	parameters[10].segment_number = SERIAL_DATA_BIT_SNUMBER;
	parameters[10].position = SERIAL_DATA_BIT_POSITION;
	
	parameters[11].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[11].name, SERIAL_STOP_BIT_SNAME);
	parameters[11].segment_number = SERIAL_STOP_BIT_SNUMBER;
	parameters[11].position = SERIAL_STOP_BIT_POSITION;
	
	parameters[12].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[12].name, BD_PASS_PART1_SNAME);
	parameters[12].segment_number = BD_PASS_PART1_SNUMBER;
	parameters[12].position = BD_PASS_PART1_POSITION;
	parameters[12].regEx = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[12].regEx, BD_PASS_PART1_REGEX);

	parameters[13].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[13].name, ROM_CHECKSUM_PART0_SNAME);
	parameters[13].segment_number = ROM_CHECKSUM_PART0_SNUMBER;
	parameters[13].position = ROM_CHECKSUM_PART0_POSITION;
	parameters[13].regEx = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[13].regEx, ROM_CHECKSUM_PART0_REGEX);

	parameters[14].name = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[14].name, ROM_CHECKSUM_PART1_SNAME);
	parameters[14].segment_number = ROM_CHECKSUM_PART1_SNUMBER;
	parameters[14].position = ROM_CHECKSUM_PART1_POSITION;
	parameters[14].regEx = (char *) malloc(sizeof(char) * 20);
	strcpy(parameters[14].regEx, ROM_CHECKSUM_PART1_REGEX);
}

void freeSetmentParameters() {
	for (int i = 0; i < NUMBER_OF_PARAMETERS; i++) {
		free(parameters[i].name);
		if(i <= 5 || i >= 12) {
			free(parameters[i].regEx);
		}
	}
}

int openFile(char * filename, int flags) {
	int fd = open(filename, flags);

	if ( fd == -1) {
		freeSetmentParameters();
		err(3, "Can't open configuration file %s", filename);
	}

	return fd;
}

int createFile(char * filename) {
	int fd = open(filename, O_CREAT | O_WRONLY, 0666);

	if ( fd == -1) {
		freeSetmentParameters();
		err(4, "Can't create configuration file %s", filename);
	}

	return fd;
}

void writeFile(int count, int fd) {
	for (int i = 0; i < count; i++) {
		if (write(fd, &segments[i], sizeof(segments[i])) <= 0) {
			close(fd);
			freeSetmentParameters();
			err(7, "Couldn't rewrite the file");
		}
	}
}

int readFile(int fd) {
	int count = 0;

	ssize_t readSize;

	while((readSize = read(fd, &segments[count], sizeof(segments[count]))) != 0 ) {
		if (readSize < 0) {
			close(fd);
			freeSetmentParameters();
			err(7, "An error occured while reading from a file");
		}

		count++;
	}

	return count;
}

void createConfigFile(int argc, char * argv[]) {
	if ( argc % 2 != 1) {
		freeSetmentParameters();
		errx(3, "Not a valid number of parameters for command create");
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
			freeSetmentParameters();
			err (4, "A parameter is expected to be a number but an error occured");
		}

		char* segmentType = argv[i+4];

		if (segmentIndex < 0 || segmentIndex > 63) {
			close(fd);
			freeSetmentParameters();
			errx(5, "Invalid segment index");
		}

		if (strlen(segmentType) != 1) {
			close(fd);
			freeSetmentParameters();
			errx(6, "Invalid segment type");
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
				freeSetmentParameters();
				errx(6, "Invalid sedment type");
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
		freeSetmentParameters();
		errx (10, "Couldn't create the regex");
	}

	if ( regexec(&regex, str, 0, NULL, 0) == 0) {
		return 1; // valid match
	}

	freeSetmentParameters();
	errx(11, "Parameter is of invalid type");
}

int checkIfValid(int par_index, char * arg) {
	struct parameter par = parameters[par_index];
	if (segmentType[par.segment_number] == WORD_SEGMENT_TYPE) {
		return checkRegex(par.regEx, arg);
	} else if (segmentType[par.segment_number] == NUMBER_SEGMENT_TYPE) {
		int numarg;
		int readSize = sscanf(arg, "%d", &numarg);

		if (errno != 0 || readSize == 0) {
			freeSetmentParameters();
			err (4, "A parameter is expected to be a number but an error occured");
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
			freeSetmentParameters();
			errx(11, "Parameter is of invalid type");
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

	freeSetmentParameters();
	errx(1, "Not a valid parameter name");
}

void setParameter(char * el_name, char * el_val) {
	int par_index = getParameter(el_name);
		
	//sheck if valid
	if (!checkIfValid(par_index, el_val)) {
		freeSetmentParameters();
		errx(12, "Invalid parameter");
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
		freeSetmentParameters();
		errx (9, "Not a valid status to change the %s index", parameters[par_index].name);
	}
}

void setBitCommand(int argc, char * argv[]) {//reset the start position of the file
	if (argc != 5) {
		freeSetmentParameters();
		errx (8, "Invalid number of parameters for command set bit");
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
		freeSetmentParameters();
		errx (7, "Invalid number of parameters for command set parameters");
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
		freeSetmentParameters();
		errx(9, "Invalid number of parameters for command get parameters");
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

int main(int argc, char * argv[]) {
	if ( argc < 2) {
		errx(1, "Not enough parameters.");
	}

	if (argc == 2 && strcmp(argv[1], "-h")) {
		printHelp();
	} else if (argc == 2) {
		errx(2, "Not a valid command");
	}

	if (strlen(argv[2]) != 2 || argv[2][0] != '-') {
		errx(2, "Not a valid nommand");
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
			freeSetmentParameters();
			errx(2, "Not a valid nommand");
	}

	freeSetmentParameters();
}