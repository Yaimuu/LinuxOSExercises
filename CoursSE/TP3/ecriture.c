#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>

void printInFile (FILE* fichier, int processNumber)
{
    fprintf(fichier, "Execution du processus %d !\n", processNumber);
}

int main(void){

    FILE* fichier = NULL;//C'est un pointeur sur une structure de type FILE qui va permettre d'écrire dans le fichier.

    fichier = fopen("./pingouin.txt", "w"); // ouvre le fichier, "w" est un mode d'ouverture en écriture seule.

    if (fichier == NULL){
	printf("fichier non trouvé");
	exit(EXIT_FAILURE); // oups !
    }

    unsigned int semid;
    semid = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT|IPC_EXCL);//crée 1 sémaphore
    semctl(semid, 0, SETVAL, 0); //initialise la valeur à 0 sur le sémaphore
    pid_t pid[6];

    struct sembuf up = {0, 1, 0}; //number, operation, flag
    struct sembuf down = {0, -6, 0}; //number, operation, flag
    
    for(int i = 0; i < 6; ++i)
    {
        pid[i] = fork();
        if(pid[i])
        {
            printf("%d\n", i);
            printInFile(fichier, i);
            semop(semid, &up, 1);
            exit(0);
        }
    }

    //printInFile(fichier,1);

    semop(semid, &down, 1);
    printf("FIN\n");
    fprintf(fichier, "FIN !\n");

    wait(NULL);
    semctl(semid, 0, IPC_RMID); //ferme le sémaphore

    fclose(fichier); // /!\ Ne pas oublier de fermer …
    exit(EXIT_SUCCESS);
}
