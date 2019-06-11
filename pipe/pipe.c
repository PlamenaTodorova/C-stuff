#include <unistd.h>
#include <sys/types.h>
#include <err.h>

int main() {
    const int READ = 0;
    const int WRITE = 1;

    pid_t fork_cut;
    pid_t fork_sort;
    pid_t fork_uniq;

    int pf1[2];
    int pf2[2];
    int pf3[2];

    if (pipe(pf1) == -1) {
        errx(1, "Couldn't create a pipe");
    }

    if ( pipe(pf2) == -1) {
        errx(1, "Couldn't create a pipe");
    }

    if (pipe(pf3) == -1) {
        errx(1, "Couldn't create a pipe");
    }

    if ((fork_cut = fork())  == -1) {
        err(2, "Couldn't create a fork");
    }

    if (fork_cut == 0) {
        close(pf1[READ]);
        dup2(pf1[WRITE], 1);
        
        if (execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", NULL) == -1) {
            err(3, "Couldn't execute cut");
        }
    }

    close(pf1[WRITE]);

    if ( (fork_sort = fork()) == -1) {
        err(2, "Couldn't create a fork");
    }

    if (fork_sort == 0) {
        dup2(pf1[READ], 0);

        close(pf2[READ]);
        dup2(pf2[WRITE], 1);
    
        if (execlp("sort", "sort", "-", NULL) == -1) {
            err(3, "Couldn't execute sort");
        }
    }

    close(pf2[WRITE]);

    if ((fork_uniq = fork())  == -1) {
        err(2, "Couldn't create a fork");
    }

    if (fork_sort == 0) {
        dup2(pf2[READ], 0);

        close(pf3[READ]);
        dup2(pf3[WRITE], 1);

        if (execlp("uniq", "uniq", "-c", NULL) == -1) {
            err(3, "Couldn't execute sort");
        }
    }

    close(pf3[WRITE]);

    dup2(pf3[READ], 0) == -1
    
    execlp("sort", "sort", "-n", NULL);
    err(1, "exec sort -n");
}