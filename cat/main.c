#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<err.h>
#include<stdio.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("%s\n", "Usage: cat files...");
		errx(1, "Not enough parameters");
	}

	for (int i = 1; i < argc; i++) {
		int fd = open(argv[i], O_RDONLY);

		if (fd == -1) {
			err(2, "Can't open file with name %s", argv[i]);
		}

		char buffer[1024];
		ssize_t read_size;

		while ( (read_size = read(fd, &buffer, sizeof(buffer))) != 0 ) {
			if (read_size < 0) {
				err(3, "Error while reading");
			}

			printf("%.*s",(int)read_size, buffer);
		}

		close(fd);
	}
}