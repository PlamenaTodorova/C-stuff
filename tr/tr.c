#include <sys/types.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void delete(char * set1) {
    ssize_t readSize;
    char * buffer = (char *) malloc(8);
    
    while( (readSize = read(0, buffer, sizeof(buffer))) != 0 ) {
        if (readSize < 0) {
            err(2, "An error occured while reading");
        }
        
        for ( int i = 0; i < strlen(set1); i++ ) {
            int index = 0;
            for (int j = 0; j < readSize; j++, index++) {
                if ( buffer[j] == set1[i]) {
                    index--;
                } else {
                    buffer[index]=buffer[j];
                }
            }
            readSize = index;
        }

        if (write(1, buffer, readSize) != readSize) {
            errx(3, "An error occured while writing");
        }
    }

    free(buffer);
}

void squish(char * set1) {
    ssize_t readSize;
    char * buffer = (char *) malloc(8);
    int flag = 0;
    char last;
    
    while( (readSize = read(0, buffer, sizeof(buffer))) != 0 ) {
        if (readSize < 0) {
            err(2, "An error occured while reading");
        }
        
        for ( int i = 0; i < strlen(set1); i++ ) {
            int index = 1;
            
            if (flag && buffer[0] == set1[i] && buffer[0] == last) {
                index--;
            }
            
            for (int j = 1; j < readSize; j++, index++) {
                if ( buffer[j] == set1[i] && buffer[j] == buffer[index-1]) {
                    index--;
                } else {
                    buffer[index]=buffer[j];
                }
            }
            readSize = index;
        }

        flag = 1;
        last = buffer[readSize - 1];

        if (write(1, buffer, readSize) != readSize) {
            errx(3, "An error occured while writing");
        }
    }

    free(buffer);
}

void change(char * set1, char * set2) {
    if (strlen(set1) > strlen(set2)) {
        errx(4, "Not enough letters");
    }

    ssize_t readSize;
    char * buffer = (char *) malloc(8);
    
    while( (readSize = read(0, buffer, sizeof(buffer))) != 0 ) {
        if (readSize < 0) {
            err(2, "An error occured while reading");
        }
        
        for ( int i = 0; i < strlen(set1); i++ ) {
            for (int j = 0; j < readSize; j++) {
                if ( buffer[j] == set1[i]) {
                    buffer[j] = set2[i];
                }
            }
        }

        if (write(1, buffer, readSize) != readSize) {
            errx(3, "An error occured while writing");
        }
    }

    free(buffer);
}

int main (int argc, char * argv[]) {
    if (argc != 3) {
        errx(1, "Invalid number of parameters");
    }

    if (strcmp(argv[1], "-d") == 0) {
        delete(argv[2]);
    } else if (strcmp(argv[1], "-s") == 0) {
        squish(argv[2]);
    } else {
        change(argv[1], argv[2]);
    }
}