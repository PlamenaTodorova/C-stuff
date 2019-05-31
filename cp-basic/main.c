#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<err.h>
#include<stdlib.h>

int main (int argc, char* argv[]) {
	if ( argc != 3){
		errx(1, "Not enough arguments");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1) {
		err(2, "%s can't be opened", argv[1]);
	}

	int fd2 = open(argv[2], O_CREAT | O_WRONLY, 0666 );
	if (fd2 == -1 ) {
		close(fd1);
		err(3, "Can't create the copy");
	}

	char buffer[1024];
	ssize_t readsize;

	while ((readsize = read(fd1, &buffer, sizeof(buffer))) != 0) {
		if ( readsize < 0 ) {
			close(fd1);
			close(fd2);
			err(4, "Error while reading");
		}

		if ( write(fd2, &buffer, readsize) != readsize) {
			close(fd1);
			close(fd2);
			err(5, "Error while writing");
		}
	}

	close(fd1);
	close(fd2);
	exit(0);
}