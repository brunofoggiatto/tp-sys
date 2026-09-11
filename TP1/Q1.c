# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>

int main(void){ 
    int counter = 0;
    for (int i = 0; i<10; i++){
        pid_t pid = fork();
        if (pid == 0){
            for (int j = 0; j<10; j++){
                printf("Number: %d\n", i);
                counter = counter + 1;
            }
            return 0;
        }
    }
    printf("Prints: %d\n", counter);
    return 0;
}