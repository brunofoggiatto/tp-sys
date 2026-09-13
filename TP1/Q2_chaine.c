#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
 if (argc != 2) {
  fprintf(stderr, "uso: %s n\n", argv[0]);
  return 1;
 }
 int n = atoi(argv[1]);

 printf("processo %d, pai %d\n", getpid(), getppid());
 for (int i = 0; i < n; i++) {
  // esvazia o buffer antes do fork, senão o filho copia e repete o texto
  fflush(stdout);
  if (fork() == 0) {
   printf("processo %d, pai %d\n", getpid(), getppid());
   exit(0);
  }
  wait(NULL);
 }
 return 0;
}
