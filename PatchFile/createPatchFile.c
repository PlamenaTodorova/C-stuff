#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct patchElem {
    uint16_t padding;
    uint8_t original;
    uint8_t changed;
};

int main(int argc, char * argv[] ) {
    if (argc != 4) {
        errx(1, "Invalid number of parameters");
    }

    int fd1 = open(argv[1], O_RDONLY);

    if (fd1 == -1) {
        err(2, "Couldn't open file %s", argv[1]);
    }

    int fd2 = open(argv[2], O_RDONLY);

    if (fd2 == -1) {
        close(fd1);
        err(2, "Couldn't open file %s", argv[2]);
    }

    int fd3 = open(argv[3], O_CREAT | O_WRONLY, 0666);

    if (fd3 == -1) {
        close(fd1);
        close(fd2);
        err(2, "Couldn't open file %s", argv[3]);
    }

    uint8_t original;
    uint8_t copied;
    uint16_t place = 0;

    ssize_t size1, size2;

    while ( (size1 = read(fd1, &original, sizeof(original)) ) != 0 && (size2 = read(fd2, &copied, sizeof(copied)) ) != 0 ) {
        if (size1 < 0) {
            close(fd1);
            close(fd2);
            close(fd3);
            err(3, "A problem occured while reading from file %s", argv[1]);
        }

        if (size2 < 0) {
            close(fd1);
            close(fd2);
            close(fd3);
            err(3, "A problem occured while reading from file %s", argv[2]);
        }

        if (original != copied) {
            struct patchElem elem;
            elem.padding = place;
            elem.original = original;
            elem.changed = copied;

            if (write(fd3, &elem, sizeof(elem)) < 0) {
                close(fd1);
                close(fd2);
                close(fd3);
                err(4, "A problem occured while writing into file %s", argv[3]);
            }
        }

        place++;
    }

    if ( size1 > 0 ) {
        printf("File %s has been changed. File %s is longer than %s.", argv[2], argv[1], argv[2]);
    }
    else if ( (size2 = read(fd2, &copied, sizeof(copied))) > 0 ) {
        printf("File %s has been changed. File %s is longer than %s.", argv[2], argv[2], argv[1]);
    }

    close(fd1);
    close(fd2);
    close(fd3);
    exit(0);    
}