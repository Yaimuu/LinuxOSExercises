#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <time.h>

#define N 10
#define MAX_VALUE 100
#define NB_ROLES 5
#define NB_CASES_ASSIGNEES 6

// Le soldat retourne l'indice de son tour actuel
int soldat(int nextMove)
{
	int newScoreIndice = nextMove + NB_CASES_ASSIGNEES;

	return newScoreIndice;
}

// Le génie retourne un indice du tableau aléatoire et sauvegarde cet 
// indice dans le tableau de ses anciens tours
int genie(int *lastMoves, int *ptr)
{
	int nextMove = 0, isVisited = 0;
	do
	{
		isVisited = 0;
		nextMove = rand() % (N + NB_CASES_ASSIGNEES) + NB_CASES_ASSIGNEES;
		for(int i = 1; i < N + NB_CASES_ASSIGNEES; i++)
		{
			if(ptr[lastMoves[i]] == nextMove)
			{
				isVisited = 1;
			}
		}
	} while(isVisited);

	return nextMove;
}

// L'artiste retourne un indice du tableau aléatoirement
int artiste(int *ptr)
{
	return ptr[rand() % (N + NB_CASES_ASSIGNEES) + NB_CASES_ASSIGNEES];
}

// Le tricheur renvoie l'indice de la case la plus grande du tableau
int tricheur(int *ptr)
{
	int maxValue = 0;

	for(int i = NB_CASES_ASSIGNEES; i < N + NB_CASES_ASSIGNEES; i++)
	{
		maxValue = maxValue > ptr[i] ? maxValue : i;
	}

	return maxValue;
}

int main () 
{
	/* Intializes random number generator */
	time_t t;
	srand((unsigned) time(&t));

	unsigned int semid;
	semid = semget(IPC_PRIVATE, 3, 0666|IPC_CREAT|IPC_EXCL); //crée 3 sémaphore
	semctl(semid, 0, SETVAL, 1);
	semctl(semid, 1, SETVAL, 0);
	semctl(semid, 2, SETVAL, 0);

	int * ptr; // Un pointeur
	int nbCasesAssignees = NB_CASES_ASSIGNEES;
	int nbCasesJeu = N + nbCasesAssignees;
	int shmid;

	shmid = shmget(IPC_PRIVATE, N*sizeof(int),IPC_CREAT|0700); // On crée un share memory
	ptr = shmat(shmid,NULL,0); //void * shmat(int shmit, const void * adr, int option)
	// 2: NULL pour laisser le système trouver tout seul
	// 3: SHM_RDONLY, ou 0 pour permetre Lecture / écriture (par défault )

	int max_value = 0, max_indice = nbCasesAssignees;

	pid_t pids[2] = {fork(), fork()}; // Initialisation des joueurs

	// Joueur
	ptr[0] = 1;
	// Role Joueur 1
	ptr[1] = 0;
	// Role Joueur 2
	ptr[2] = 0;
	// Compteur de tour
	ptr[3] = 0;
	// Score 1
	ptr[4] = 0;
	// Score 2
	ptr[5] = 0;
	// Fin
	ptr[6] = 0;

	struct sembuf up_A = {0, 1, 0}; //number, operation, flag
	struct sembuf down_A = {0, -1, 0};
	struct sembuf up_J1 = {1, 1, 0};
	struct sembuf down_J1 = {1, -1, 0};
	struct sembuf up_J2 = {2, 1, 0};
	struct sembuf down_J2 = {2, -1, 0};

	for(int proc = 0; proc < 2; proc++)
	{
		
		if (pids[proc] < 0) // Error
		{
			printf("Process ERROR !\n");
			exit(-1);
		}
		else if(pids[proc] > 0) //Arbitre
		{
			if(!proc) // Empêcher de faire plusieurs initialisations des variables
			{
				printf("Assignation des roles !\n");

				ptr[1] = rand() % NB_ROLES; // Role aléatoire
				ptr[2] = rand() % NB_ROLES; // Role aléatoire
				ptr[3] = 0;
				
				// Initialisation du tableau
				for(int j = nbCasesAssignees; j < N; j++)
				{
				  ptr[j] = rand() % (MAX_VALUE);
				  if(ptr[j] > max_value)
				  {
				  	max_value = ptr[j];
				  	max_indice = j;
				  }
				}
			}
			
			while(ptr[6])
			{
				semop(semid, &down_A, 1);

				printf("Arbitre :\n\n");
				printf("\tTour %d\n", ptr[3]);
				printf("\tRoles %d vs %d\n", ptr[1], ptr[2]);
				printf("\tLa valeur max est %d à l'indice %d\n", max_value, max_indice);
				printf("\tLe score du joueur %d est %d\n", ptr[0] + 1, ptr[4 + ptr[0] + 1]);
				// Vérification de victoire du Joueur venant de jouer
				if(ptr[4 + ptr[0]] == max_indice)
				{
					printf("\nLe Joueur %d gagne !!\n", ptr[0] + 1);
					ptr[6] = ptr[0] + 1;
					break;
				}

				// Next joueur
				ptr[0] = (ptr[0]+1) % 2;

				// Compteur
				ptr[3] = ptr[3] + 1;

				for(int j = 0; j < N; j++)
				{
				   printf("\tptr[%d] = %d\n", j, ptr[j]);
				}

				printf("\tNext : Joueur %d\n", ptr[0] + 1);
				
				fflush(stdout);

				if(ptr[0])
				{
			   		semop(semid, &up_J2, 1);
				}
			   	else
			   	{
			   		semop(semid, &up_J1, 1);
			   	}

				if(ptr[3] >= N)
				{
					printf("Fin au tour %d !\n", ptr[3]);
				   break;
				}
			}
			
			printf("Fin arbitre !\n");
			wait(NULL);
		}
		else //Joueurs 1 et 2
		{
			// Case 0 : meilleur indice - Le reste des cases sont les indices visités
			int scoreGenie[nbCasesJeu + 1];
			// Indice général
			int score = 0;
			int role = 0;

			while(ptr[6])
			{
				if(ptr[0])
			   		semop(semid, &down_J2, 1);
			   	else
			   		semop(semid, &down_J1, 1);

			   	int joueur = ptr[0] + 1;

				printf("\nJoueur %d :\n\n", joueur);

				printf("\tRole : %d\n", ptr[joueur]);
				printf("\tScore : %d\n", ptr[joueur]);
				/*for(int j = 0; j < N; j++)
				{
				   printf("ptr[%d] = %d\n", j, ptr[j]);
				}*/

				if(ptr[1 + joueur] == 4) // Fou
				{
					printf("\tRole : Fou\n");
					role = rand() % NB_ROLES;
				}
				else
				{
					role = ptr[1 + joueur];
				}

				switch(role)
				{
					case 0: // Soldat
						printf("\tRole : Soldat\n");
						ptr[4 + joueur] = soldat(ptr[3]);
						break;
					case 1: // Artiste
						printf("\tRole : Artiste\n");
						ptr[4 + joueur] = artiste(ptr);
						break;
					case 2: // Génie
						printf("\tRole : Génie\n");
						scoreGenie[ptr[3]] = genie(scoreGenie, ptr);
						ptr[4 + joueur] = scoreGenie[ptr[3]];
						break;
					case 3: // Tricheur
						printf("\tRole : Tricheur\n");
						ptr[4 + joueur] = tricheur(ptr);
						break;
					default:
						printf("Role innexistant !\n");
						break;
				}
				printf("\n");
				
				if(ptr[3] >= N)
				{
					printf("\tFin joueur %d !\n", ptr[0] + 1);
					semop(semid, &up_A, 1);
				   break;
				}
				
			   fflush(stdout);
			   semop(semid, &up_A, 1);
				
			}
			exit(0);
		}
	}

	wait(NULL);
   	shmctl(shmid,IPC_RMID,NULL); // /!\ Toujours fermer le partage

	semctl(semid, 0, IPC_RMID); //ferme le sémaphore Arbitre
	semctl(semid, 1, IPC_RMID); //ferme le sémaphore Joueur1
	semctl(semid, 2, IPC_RMID); //ferme le sémaphore Joueur2

	printf("FIN\n");

	return EXIT_SUCCESS;
}

