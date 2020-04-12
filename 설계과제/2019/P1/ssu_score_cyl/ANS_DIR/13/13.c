#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo);

int count = 0;

int main(void)
{
    signal(SIGALRM, ssu_signal_handler);
    alarm(1);
    
    while(1);    

    exit(0);
}

void ssu_signal_handler(int signo) {
    printf("alarm %d\n", count++);
    alarm(1);

    if(count > 3)
        exit(0);
}