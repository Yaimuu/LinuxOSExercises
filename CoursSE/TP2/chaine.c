#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) 
{
    if(argc >= 2)
    {
        int n = (int)*argv[1];
        pid_t pid[n];
        char number = (char)n;
        printf("debut : n=%s\n", number);

        for(int i = 0; i < n; i++)
        {
            /*pid[i] = fork();
            if (pid[i] == 0) 
            {
                printf("child : je suis %d, fils de %d.\n",  (int) getpid (),  (int) getppid ());
            }
            else 
            {
                printf("parent: je suis %d, parent de %d et fils de %d.\n",  (int) getpid (), pid,  (int) getppid ()); 
            }*/

            //printf("Fin\n");
        }

        
        return 0;
    }

    printf("ERREUR : Ce programme nécessite 1 argument de type entier.\n");
    return -1;
}