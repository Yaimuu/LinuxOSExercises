#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

int main()
{
    pid_t pid;
    int n = 3;
    for(int i = 0; i < n; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            printf("Je suis %d, fils de %d, n = %d\n", (int) getpid(), (int) getppid(), i);
        }
        else
        {
            wait(NULL);
        }
    }
    if (pid != 0)
    {
        wait(NULL);
    }
    return 0;
}
