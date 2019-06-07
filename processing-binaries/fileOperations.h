#include <sys/types.h> // for open
#include <err.h>	// for err.. duh
#include <fcntl.h> // for open flags
#include <unistd.h> // for read
#include <string.h> // for strcpy
#include "parameter.h"
#include "segment.h"

int openFile(char * filename, int flags) {
	int fd = open(filename, flags);

	if ( fd == -1) {
		freeSegmentParameters();
		err(3, "Can't open configuration file %s", filename);
	}

	return fd;
}

int createFile(char * filename) {
	int fd = open(filename, O_CREAT | O_WRONLY, 0666);

	if ( fd == -1) {
		freeSegmentParameters();
		err(4, "Can't create configuration file %s", filename);
	}

	return fd;
}

void writeFile(int count, int fd) {
	for (int i = 0; i < count; i++) {
		if (write(fd, &segments[i], sizeof(segments[i])) <= 0) {
			close(fd);
			freeSegmentParameters();
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
			freeSegmentParameters();
			err(7, "An error occured while reading from a file");
		}

		count++;
	}

	return count;
}