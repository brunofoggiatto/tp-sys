#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
 int alors = 0, sinon = 0;

 // troca "alors" e "sinon" por NULL: cada parte vira um argv pronto para o execvp
 for (int i = 1; i < argc; i++) {
  if (alors == 0 && strcmp(argv[i], "alors") == 0) {
   alors = i;
   argv[i] = NULL;
  } else if (alors != 0 && sinon == 0 && strcmp(argv[i], "sinon") == 0) {
   sinon = i;
   argv[i] = NULL;
  }
 }
 if (alors < 2 || argv[alors + 1] == NULL || (sinon != 0 && argv[sinon + 1] == NULL)) {
  fprintf(stderr, "uso: %s condição alors comando [sinon comando]\n", argv[0]);
  return 2;
 }

 if (fork() == 0) {
  execvp(argv[1], argv + 1);
  perror("execvp");
  exit(127);
 }
 int status;
 wait(&status);

 if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
  execvp(argv[alors + 1], argv + alors + 1);
 else if (sinon != 0)
  execvp(argv[sinon + 1], argv + sinon + 1);
 else
  return 0;
 perror("execvp");
 return 1;
}
