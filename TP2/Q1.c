#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 1024

int main(int argc, char *argv[]) {

if (argc != 2) {
    printf("Usage: %s <nombre_octets>\n", argv[0]);
    return 1;
}

int n = atoi(argv[1]);

int fd[2];

if (pipe(fd) == -1) {
    perror("pipe");
    return 1;
}

char buffer[MAX];
int total = 0;

while (total < n) {

    int a_ecrire = n - total;

    if (a_ecrire > MAX)
        a_ecrire = MAX;

    int nb = write(fd[1], buffer, a_ecrire);

    if (nb == -1) {
        perror("write");
        break;
    }

    total += nb;

    if (nb == 0)
        break;
}

printf("Nombre d'octets ecrits : %d\n", total);

close(fd[0]);
close(fd[1]);

return 0;

}
