#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
main(int argc, char*argv[]) {
    int retour;
    printf("%s\n", argv[0]); // A
    switch (fork()) { // B
        case -1:
            perror("fork1()");
            exit(1);
        case 0:
            switch (fork()) { // C
                case -1:
                    perror("fork2()");
                    exit(1);
                case 0:
                    if (execl("./prog","prog",NULL) == -1) {
                        perror("execl");
                        exit(1);
                    }
                    break;
                default:
                    exit(0);
            }
        default:
            wait(&retour);
    }
}
