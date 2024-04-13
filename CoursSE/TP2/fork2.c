#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
    pid_t pid;
    int x = 1;
    printf("debut\n");
    for(int i = 0; i < 3; i++)
    {
    	pid = fork();
    	if (pid == 0) // Child
	    {

	        printf("child : je suis %d, fils de %d. x=%d\n",  (int) getpid (),  (int) getppid (), ++x); 
	    	if(i == 3)
    		{
    			pid_t pid3[] = {fork(), fork()};
    			
    			if (pid3[0] == 0 || pid3[1] == 0) // Child
			    {

			        printf("child : je suis %d, fils de %d. x=%d\n",  (int) getpid (),  (int) getppid (), ++x); 
			    	
			    }
			    else // Parent
			    {
			        printf("parent: je suis %d, parent de %d et fils de %d. x=%d\n",  (int) getpid (), pid,  (int) getppid (), --x); 
			    }
    		}
	    }
	    else // Parent
	    {
	        printf("parent: je suis %d, parent de %d et fils de %d. x=%d\n",  (int) getpid (), pid,  (int) getppid (), --x); 
	    }
    }

    printf("Fin\n");
    return 0;
}
