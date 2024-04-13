#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int x = 0;
    printf("Début\n");
    if (fork() != 0)
    {
        if (fork() != 0)
        {
            fork();
            wait(NULL);
            x += 2;
        }
        x++;
    }
    if (x == 1)
    {
        if (fork() != 0)
        {
            fork();
            wait(NULL);
        }
        wait(NULL);
    }
    printf("Je suis %d, fils de %d\n", (int) getpid(), (int) getppid());
    printf("Fin\n");
    wait(NULL);
    return 0;
}