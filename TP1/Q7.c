#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void handler(int sig) {
}

int main(int argc, char *argv[]) {
 if (argc != 2) {
  fprintf(stderr, "uso: %s n\n", argv[0]);
  return 1;
 }
 int n = atoi(argv[1]);
 pid_t primeiro = getpid();

 struct sigaction sa;
 sa.sa_handler = handler;
 sigemptyset(&sa.sa_mask);
 sa.sa_flags = 0;
 sigaction(SIGUSR1, &sa, NULL);

 // SIGUSR1 fica bloqueado até o sigsuspend, senão o aviso pode chegar cedo e se perder
 sigset_t bloqueio, antiga;
 sigemptyset(&bloqueio);
 sigaddset(&bloqueio, SIGUSR1);
 sigprocmask(SIG_BLOCK, &bloqueio, &antiga);

 printf("processo %d, pai %d\n", getpid(), getppid());
 for (int i = 0; i < n; i++) {
  fflush(stdout);
  if (fork() != 0) {
   if (getpid() != primeiro)
    exit(0);
   wait(NULL);
   sigsuspend(&antiga);
   printf("pai inicial %d: último filho terminou\n", getpid());
   return 0;
  }
  printf("processo %d, pai %d\n", getpid(), getppid());
 }

 // último filho avisa o primeiro pai
 fflush(stdout);
 kill(primeiro, SIGUSR1);
 return 0;
}
