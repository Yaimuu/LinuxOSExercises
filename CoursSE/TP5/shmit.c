#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define N 10

int main (int argn, char * argv[]) 
{
	/* Intializes random number generator */
	time_t t;
	srand((unsigned) time(&t));

	pid_t pid; // pour baptiser notre enfant
	int * ptr; // Un pointeur
	int shmid;
	shmid = shmget(IPC_PRIVATE,N*sizeof(int),IPC_CREAT|0700); // On crée un share memory
	ptr = shmat(shmid,NULL,0); //void * shmat(int shmit, const void * adr, int option)
	// 2: NULL pour laisser le système trouver tout seul
	// 3: SHM_RDONLY, ou 0 pour permetre Lecture / écriture (par défault )

	int tubePversF[2], tubeFversP[2];
	if ((pipe(tubePversF) != 0) || (pipe(tubeFversP) != 0))    
	{      
		perror("Création du tube impossible");      
		exit(1);
	}

	pid=fork(); // On fait un fork et on récupère le résultat

	if(pid > 0) //processus père
	{ 
		close(tubePversF[0]);
		close(tubeFversP[1]);

		printf("Le père crée le tableau !\n");

		int indice = rand() % N; // On calcule un indice au hasard
		ptr[0]=indice; // On le stocke
		printf("ptr[%d]=%d\n", 0, ptr[0]);

		for(int j = 1; j < N; j++)
		{
		  ptr[j] = rand() % (N*10);
		  printf("ptr[%d]=%d\n", j, ptr[j]);
		}

		write(tubePversF[1], &ptr, sizeof(ptr));
		read(tubeFversP[0], &ptr, sizeof(ptr));


		printf("Le père vérifie !\n");

		for(int j = 0; j < N; j++)
		{
		  printf("ptr[%d]=%d\n", j, ptr[j]);
		}
	}
	else //processus fils
	{
		close(tubePversF[1]);
		close(tubeFversP[0]);

		read(tubePversF[0], &ptr, sizeof(ptr));

		printf("Le fils agit !\n");

		for (int i = 0; i < N; i++)
		{
			for(int j = 0; j < N; j++)
			{
				if(ptr[i] < ptr[j])
				{
					int tmp = ptr[i];
					ptr[i] = ptr[j];
					ptr[j] = tmp;
				}
			}
		}

		write(tubeFversP[1], &ptr, sizeof(ptr));
		exit(0);
	}
	return EXIT_SUCCESS;
}