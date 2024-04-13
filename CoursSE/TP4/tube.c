# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
int main (void) {
	int df[2]; // le tableau contiendra deux nouveaux descripteurs de fichiers
	char buf[18]; // pour recuperer les caracteres lus dans le tube
	int pid;
	pipe(df); // Si le processus envoie des données via le df[0], il est possible d'obtenir (lire) ces informations à partir de df[1].
	if ((pid=fork()) == -1)
		perror("erreur");
	if (pid > 0) {// processus pere
		close(df[0]); // Le pere ferme le côté entrée du tube pour utiliser la sortie
		sleep(2); // attente
		write(df[1],"Je suis ton père" , 18);
		// UTF8 prends plus d'un caractère pour les accents + '\0'
		close(df[1]);
	}
	else {// processus fils
		close(df[1]); // Le fils ferme le côté sortie du tube pour utiliser l'entrée
		sleep(1); // attente 
		read(df[0] ,buf, 18);
		printf( "[%d] : %s\n", getpid(), buf);
		close(df[0]);
		printf( "Noooooooonnnnn !");
	}
	exit(EXIT_SUCCESS);
}