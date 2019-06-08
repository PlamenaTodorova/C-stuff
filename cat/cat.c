#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<err.h>
#include<stdio.h>
#include<string.h>

int lines = 1;

int specialPrint(char * buffer, int size, int flag) {
    if (flag) {
        //first print
        printf("%d\t", lines);
        lines++;
    }

    for (int i = 0; i < size - 1; i++) {
        if (buffer[i] == '\n') {
            printf("\n%d\t", lines);
            lines++;
        } else {
            printf("%c", buffer[i]);
        }
    }
    printf("%c", buffer[size - 1]);

    return buffer[size - 1] == '\n';
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("%s\n", "Usage: cat files...");
		errx(1, "Not enough parameters");
	}

    int flag = 0;
    int startposiotio = 1;
    int newline = 1;

    if (strcmp(argv[1], "-n") == 0) {
        startposiotio = 2;
        flag = 1;
    }
    

	for (int i = startposiotio; i < argc; i++) {
        int fd = 0; //STDIN
        if (strcmp(argv[i], "-") != 0){
            fd = open(argv[i], O_RDONLY);

		    if (fd == -1) {
			    err(2, "Can't open file with name %s", argv[i]);
		    }
        }
        
		char buffer[1024];
		ssize_t read_size;
        
		while ( (read_size = read(fd, &buffer, sizeof(buffer))) != 0 ) {
			if (read_size < 0) {
				err(3, "Error while reading");
			}

            if(flag){
                newline = specialPrint(buffer, (int)read_size, newline);
            } else {
                printf("%.*s",(int)read_size, buffer);
            }
		}

		if (strcmp(argv[i], "-") != 0){
            close(fd);
        }
	}
}