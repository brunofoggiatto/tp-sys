#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
 for (int i = 0; i < 10; i++) {
  if (fork() == 0) {
   for (int j = 0; j < 10; j++)
    printf("%d", i);
   exit(0);
  }
 }
 for (int i = 0; i < 10; i++)
  wait(NULL);
 return 0;
}
