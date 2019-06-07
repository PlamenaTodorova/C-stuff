#define META_DATA 7
#define WORD_SEGMENT_SIZE 3
#define WORD_SIZE 16
#define NUMBER_SEGMENT_SIZE 14
#define BYTE_SEGMENT_SIZE 56
#define MAX_SEGMENTS 64

#define WORD_SEGMENT_TYPE 0
#define NUMBER_SEGMENT_TYPE 1
#define BYTE_SEGMENT_TYPE 2

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