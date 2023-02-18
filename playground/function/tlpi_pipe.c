/*
親子プロセス間のパイプを用いた通信プログラム
*/
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 10

int main(int argc, char *argv[])
{
    int pfd[2];
    char buf[BUF_SIZE];
    ssize_t numRead;

    if (pipe(pfd) == -1) /* Create the pipe */
        perror("pipe");
    
    switch(fork()){
        case -1:
            perror("fork");
        case 0: //child
            if (close(pfd[1]) == -1)
                perror("fork - child");
            for (;;){
                numRead = read(pfd[0], buf, BUF_SIZE);
                if (numRead == -1)
                    perror("read");
                if (numRead == 0)
                    break;
                if (write(STDOUT_FILENO, buf, numRead) != numRead)
                    printf("child - partial/failed write");
            }
            write(STDOUT_FILENO, "\n", 1);
            if (close(pfd[0]) == -1)
                perror("close");
            exit(EXIT_SUCCESS);
        default:
            if (close(pfd[0]) == -1)
                perror("close - parent");
            if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1]))
                printf("parent - partial/failed write");
            if (close(pfd[1]) == -1) /* Child will see EOF */
                perror("close");
            wait(NULL);
            exit(EXIT_SUCCESS);
    }
}

