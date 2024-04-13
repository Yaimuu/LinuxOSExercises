#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    int a, e;
    a = 10;
    pid_t pid = fork();
    if (pid == 0) {
        a = a *2;
        printf("child: je suis %d, fils de %d.\n",  (int) getpid (),(int) getppid ());
        sleep(30);
        if (fork() == 0) {
            a = a +1;
            printf("child : je suis %d, fils de %d.\n",  (int) getpid (),  (int) getppid ());
        }
        printf("a : %d \n", a);
        exit(1);
    }
    else
    {
        sleep (30);
    }
    wait(&e);
    printf("a : %d ; e : %d \n", a, WEXITSTATUS(e));
    return(0);
}
