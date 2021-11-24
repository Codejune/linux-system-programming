#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syslog.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <fcntl.h>

int ssu_daemon_init(void);

int main(void)
{
    printf("daemon process initialization\n");

    if(ssu_daemon_init() < 0){
        fprintf(stderr, "ssu_daemon_init failed\n");
        exit(1);
    }
    openlog("ex8", 0, LOG_LPR);
    syslog(LOG_ERR, "My pid is %d", getpid());
    sleep(5);
    closelog();

    exit(0);
}

int ssu_daemon_init(void) {
    int fd, maxfd;
    pid_t pid;

    if((pid = fork()) < 0){
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if(pid != 0)
        exit(0);

    setsid();
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    maxfd = getdtablesize();
    for(fd = 0; fd < maxfd; fd++)
        close(fd);

    umask(0);
    chdir("/");

    fd = open("/dev/null", O_RDWR);
    dup(0);
    dup(0);

    return 0;
}
