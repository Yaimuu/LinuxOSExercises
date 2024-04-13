# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>

int main (void) 
{
	int tubePversF[2], tubeFversP[2];
	int val = 5;
	int status;

	if ((pipe(tubePversF) != 0) || (pipe(tubeFversP) != 0))    
	{      
		perror("Création du tube impossible");      
		exit(1);
	}

	if(!fork()) // fils
	{
		close(tubePversF[1]);
		close(tubeFversP[0]);

		write(tubeFversP[1], &val, sizeof(val));

		read(tubePversF[0], &val, sizeof(val));

		printf("message du fils : %d\n", val);
		exit(0);
	}
	else // père
	{
		close(tubePversF[0]);
		close(tubeFversP[1]);

		read(tubeFversP[0], &val, sizeof(val));
		printf("message du père : %d\n", val);
		val = val * 2;
		printf("message du père : %d\n", val);
		write(tubePversF[1], &val, sizeof(val));
	}

	exit(EXIT_SUCCESS);
}
