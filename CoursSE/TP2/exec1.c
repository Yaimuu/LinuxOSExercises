#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
    int p;
    p = fork();
    if (p == 0) {
        sleep(2);
        execl("/bin/echo", "echo", "je suis", "le fils", "!", NULL);
    }
    wait(NULL);
    printf("Je suis ton père\n");
    return 0;
}