#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) { fprintf(stderr, "usage: %s cmd [args...]\n", argv[0]); return 1; }
    execvp(argv[1], &argv[1]);   
    perror("execvp");
    return 127;
}
