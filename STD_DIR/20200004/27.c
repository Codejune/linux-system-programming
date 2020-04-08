#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int count = 0;

int main(void)
{
    struct sigaction sig_act;

    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
    sig_act.sa_handler = ssu_signal_handler;
    sigaction(SIGALRM,&sig_act,NULL);

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