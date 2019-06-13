#include <err.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char * argv[]) {
    if (argc != 2) {
        errx(1, "Invalid number of parameters");
    }

    const int READ = 0;
    const int WRITE = 1;

    pid_t find_fork;
    pid_t sort_fork;
    pid_t cut_fork;

    int pf1[2];
    int pf2[2];
    int pf3[2];

    if (pipe(pf1) == -1 ) {
        err(2, "Culdn't pipe");
    }

    if (pipe(pf2) == -1 ) {
        err(2, "Culdn't pipe");
    }

    if (pipe(pf3) == -1 ) {
        err(2, "Culdn't pipe");
    }

    if ((find_fork = fork()) == -1) {
        err(3, "Culdn't fork");
    }

    if ( find_fork == 0) {
        close(pf1[READ]);
        
        if (dup2(pf1[WRITE], 1) == -1) {
            err(4, "Couldn't dublicate");
        }

        if (execlp("find", "find", argv[1], "-printf", "%T@-%p\n", NULL) == -1) {
            err(5, "Couldn't execute find");
        }
    }

    close(pf1[WRITE]);

    if ((sort_fork = fork()) == -1) {
        err(3, "Culdn't fork");
    }

    if ( sort_fork == 0) {
        close(pf2[READ]);

        if (dup2(pf1[READ], 0) == -1) {
            err(4, "Couldn't dublicate");
        }
        
        if (dup2(pf2[WRITE], 1) == -1) {
            err(4, "Couldn't dublicate");
        }

        if (execlp("sort", "sort", "-nr", NULL) == -1) {
            err(5, "Couldn't execute find");
        }
    }

    close(pf2[WRITE]);

    if ((cut_fork = fork()) == -1) {
        err(3, "Culdn't fork");
    }

    if ( cut_fork == 0) {
        close(pf3[READ]);

        if (dup2(pf2[READ], 0) == -1) {
            err(4, "Couldn't dublicate");
        }
        
        if (dup2(pf3[WRITE], 1) == -1) {
            err(4, "Couldn't dublicate");
        }

        if (execlp("cut", "cut", "-d-", "-f2", NULL) == -1) {
            err(5, "Couldn't execute find");
        }
    }

    close(pf3[WRITE]);
        
    if (dup2(pf3[READ], 0) == -1) {
        err(4, "Couldn't dublicate");
    }

    if (execlp("head", "head","-n1", NULL) == -1) {
        err(5, "Couldn't execute cat");
    }
}