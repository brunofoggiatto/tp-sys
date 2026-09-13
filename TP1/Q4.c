#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
 if (argc < 2) {
  fprintf(stderr, "uso: %s comando [argumentos]\n", argv[0]);
  return 1;
 }
 for (int i = 1; i <= 5; i++) {
  if (fork() == 0) {
   execvp(argv[1], argv + 1);
   perror("execvp");
   exit(1);
  }
  int status;
  wait(&status);
  if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
   fprintf(stderr, "execução %d falhou, parando\n", i);
   return 1;
  }
 }
 return 0;
}
