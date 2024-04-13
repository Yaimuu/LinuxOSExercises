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
    semid = semget(IPC_PRIVATE, 6, 0666|IPC_CREAT|IPC_EXCL);//crée 6 sémaphore
    
    for(int i = 0; i <= 6; i++)
    {
        semctl(semid, i, SETVAL, 6-i); //initialise la valeur à i sur le sémaphore i
    }
    
    pid_t pid[6];

    struct sembuf up = {5, 1, 0}; //number, operation, flag
    struct sembuf down = {5, -1, 0}; //number, operation, flag
    
    for(int i = 0; i < 6; i++)
    {
        pid[i] = fork();
        if(pid[i])
        {
            struct sembuf up = {i, 1, 0}; //number, operation, flag
            struct sembuf down = {i, -1, 0}; //number, operation, flag

            semop(semid, &down, 1);
            printf("%d\n", i);
            printInFile(fichier, i);
            semop(semid, &up, 1);
            exit(0);
        }
    }

    semop(semid, &down, 1);
    printf("FIN\n");
    fprintf(fichier, "FIN !\n");

    wait(NULL);
    semctl(semid, 0, IPC_RMID); //ferme le sémaphore

    fclose(fichier); // /!\ Ne pas oublier de fermer …
    exit(EXIT_SUCCESS);
}
