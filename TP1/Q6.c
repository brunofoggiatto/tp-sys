#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

static pid_t mais_velho;

static void handler(int sig) {
 kill(mais_velho, SIGUSR2);
 _exit(0);
}

int main(void) {
 mais_velho = fork();
 if (mais_velho == 0) {
  while (1)
   pause();
 }

 // instalado antes do 2º fork: o caçula já nasce com o handler
 struct sigaction sa;
 sa.sa_handler = handler;
 sigemptyset(&sa.sa_mask);
 sa.sa_flags = 0;
 sigaction(SIGUSR1, &sa, NULL);

 pid_t cacula = fork();
 if (cacula == 0) {
  while (1)
   pause();
 }

 kill(cacula, SIGUSR1);

 int status;
 waitpid(mais_velho, &status, 0);
 if (WIFSIGNALED(status) && WTERMSIG(status) == SIGUSR2)
  printf("mais velho %d: terminado por SIGUSR2\n", mais_velho);
 waitpid(cacula, &status, 0);
 if (WIFEXITED(status))
  printf("caçula %d: enviou SIGUSR2 e terminou\n", cacula);
 return 0;
}
