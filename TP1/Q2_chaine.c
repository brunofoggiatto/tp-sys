#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void chaine(int n) {
 for (int i = 0; i < n; i++) {
  pid_t pid = fork();
  if (pid == 0) {
   printf("Processus fils %d, pere %d\n", getpid(), getppid());
  } else {
   break;
  }
 }
}

int main(void) {
 chaine(5);
 exit(0);
}
