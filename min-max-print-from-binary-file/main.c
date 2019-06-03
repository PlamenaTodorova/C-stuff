#include<sys/types.h>
#include<fcntl.h>
#include<err.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>

void min(int fd) {
	uint16_t number;
	uint16_t minN = UINT16_MAX;

	ssize_t read_size;
	while ( (read_size = read(fd, &number, sizeof(number))) != 0 ) {
		if (read_size < 0) {
			err(3, "Error while reading");
		}

		if (number < minN) {
			minN = number;
		}
	}


	printf("%d\n", number);
}

void max(int fd) {
	uint16_t number;
	uint16_t maxN = 0;

	ssize_t read_size;
	while ( (read_size = read(fd, &number, sizeof(number))) != 0 ) {
		if (read_size < 0) {
			err(3, "Error while reading");
		}

		if (number > maxN) {
			maxN = number;
		}
	}


	printf("%d\n", number);
}

void print(int fd) {
	uint16_t number;

	ssize_t read_size;
	while ( (read_size = read(fd, &number, sizeof(number))) != 0 ) {
		if (read_size < 0) {
			err(3, "Error while reading");
		}

		printf("%d\n", number);
	}
}

int main(int argc, char * argv[]) {
	if (argc != 3) {
		errx(1, "Invalid number of parameters.");
	}

	int fd = open(argv[2], O_RDONLY);

	if ( fd == -1 ) {
		err(2, "Can't open the file");
	}

	if ( strcmp(argv[1], "--min") == 0 ) {
		min(fd);
	}
	else if ( strcmp(argv[1], "--max") == 0 ) {
		max(fd);
	} else 	if ( strcmp(argv[1], "--printf") == 0 ) {
		print(fd);
	} else {
		close(fd);
		err(4, "Invalid parameter %s", argv[1]);
	}

	close(fd);
}