#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(){
	unsigned int semid;
	semid = semget(IPC_PRIVATE, 2, 0666|IPC_CREAT|IPC_EXCL);//crée 2 sémaphore
	semctl(semid, 0, SETVAL, 1); //initialise la valeur à 1 sur le premier sémaphore
	semctl(semid, 1, SETVAL, 0); //initialise la valeur à 0 sur le deuxième sémaphore
	int nbAffichages = 3;
	if(!fork()) {
			struct sembuf up = {0, 1, 0}; //number, operation, flag
			struct sembuf down = {1, -1, 0}; //number, operation, flag
			for (int i = 0; i < nbAffichages*3; i++)
			{
				semop(semid, &down, 1);
				printf("P2\n");
				if(i%3==2) semop(semid, &up, 1);
				
			}
	}else{
			struct sembuf up = {1, 1, 0}; //number, operation, flag
			struct sembuf down = {0, -1, 0}; //number, operation, flag
			for (int i = 0; i < nbAffichages; i++)
			{
				semop(semid, &down, 1);
				printf("P1\n");
				fflush(stdout);
				semop(semid, &up, 1);
				semop(semid, &up, 1);
				semop(semid, &up, 1);
			}
	}
	wait(NULL);
	semctl(semid, 0, IPC_RMID); //ferme le sémaphore
	semctl(semid, 1, IPC_RMID); //ferme le sémaphore
	return 0;
}