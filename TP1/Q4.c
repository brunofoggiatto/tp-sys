#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void multido(char *argv[]) {
 execvp(argv[1], &argv[1]);
 perror("execvp");
 _exit(127);
}

void arbre(int n, char *argv[]) {
 for (int i = 0; i < n; i++) {
  pid_t pid = fork();
  if (pid < 0) {
   perror("fork");
   exit(1);
  }
  if (pid == 0) {
   multido(argv);
  } else {
   int status;
   wait(&status);
   if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
    fprintf(stderr, "multido: arret a l'execution %d\n", i + 1);
    exit(1);
   }
  }
 }
}

int main(int argc, char *argv[]) {
 if (argc < 2) {
  fprintf(stderr, "usage: %s cmd [args...]\n", argv[0]);
  return 1;
 }
 arbre(5, argv);
 exit(0);
}
