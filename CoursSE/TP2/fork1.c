#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
    pid_t pid;
    int x = 1;
    printf("debut\n");
    pid = fork();
    if (pid == 0) {
        printf("child : je suis %d, fils de %d. x=%d\n",  (int) getpid (),  (int) getppid (), ++x); }
    else 
    {
        printf("parent: je suis %d, parent de %d et fils de %d. x=%d\n",  (int) getpid (), pid,  (int) getppid (), --x); 
    }
    printf("Fin\n");
    return 0;
}
