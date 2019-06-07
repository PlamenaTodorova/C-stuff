#include <stdlib.h> // for malloc

#define NUMBER_OF_PARAMETERS 15

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

struct parameter
{
	char * name;
	int segment_number;
	int position;
	char * regEx; // valid only for text and byte segments
} parameters[15];

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

void freeSegmentParameters() {
	for (int i = 0; i < NUMBER_OF_PARAMETERS; i++) {
		free(parameters[i].name);
		if(i <= 5 || i >= 12) {
			free(parameters[i].regEx);
		}
	}
}