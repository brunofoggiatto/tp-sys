#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static volatile sig_atomic_t n = 0;

static void handler(int sig) {
 n++;
 if (n == 5)
  _exit(0);
}

int main(void) {
 struct sigaction sa;
 sa.sa_handler = handler;
 sigemptyset(&sa.sa_mask);
 sa.sa_flags = 0;
 sigaction(SIGINT, &sa, NULL);

 printf("aperte Ctrl-C 5 vezes para sair\n");
 while (1) {
  pause();
  printf("Ctrl-C %d\n", (int)n);
 }
}
