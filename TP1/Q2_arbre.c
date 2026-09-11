#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void arbre(int n) {
 for (int i = 0; i < n; i++) {
  pid_t pid = fork();
  if (pid < 0) {
   perror("fork");
   exit(1);
  }
  if (pid == 0) {
   printf("Processus fils %d, pere %d\n", getpid(), getppid());
   exit(0); 
  }
 }
 for (int i = 0; i < n; i++) {
  wait(NULL);
 }
 printf("Processus pere %d\n", getpid());
}

int main(void) {
 arbre(5);
 exit(0);
}
