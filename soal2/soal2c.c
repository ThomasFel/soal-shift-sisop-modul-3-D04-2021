#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    pid_t child_id;
    
    int fd[4];
    int status;
    char *ps_aux_arg[] = {"ps", "aux", NULL};
    char *sort_arg[] = {"sort", "-nrk", "3,3", NULL};
    char *head_arg[] = {"head", "-5", NULL};

    pipe(fd);
    pipe(fd + 2);
    
    child_id = fork();
        
    if (child_id == 0) {
        dup2(fd[1], STDOUT_FILENO);

        close(fd[0]);
        close(fd[1]);
        close(fd[2]);
        close(fd[3]);

        execvp(*ps_aux_arg, ps_aux_arg);
    }

    else {
        child_id = fork();
        
        if (child_id == 0) {
            dup2(fd[0], STDIN_FILENO);
            dup2(fd[3], STDOUT_FILENO);

            close(fd[0]);
            close(fd[1]);
            close(fd[2]);
            close(fd[3]);

            execvp(*sort_arg, sort_arg);
        }

        else {
            child_id = fork();
            
            if (child_id == 0) {
                dup2(fd[2], STDIN_FILENO);

                close(fd[0]);
                close(fd[1]);
                close(fd[2]);
                close(fd[3]);

                execvp(*head_arg, head_arg);
            }
        }
    }

    close(fd[0]);
    close(fd[1]);
    close(fd[2]);
    close(fd[3]);

    for (int i = 0; i < 3; i++) {
        wait(&status);
    }

    return 0;
}
