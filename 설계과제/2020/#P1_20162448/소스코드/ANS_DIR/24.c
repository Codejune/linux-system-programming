#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

void main()
{
    char *fname = "ssu_line.txt";
    char *frname= "ssu_blank.txt";
    int fd_w, fd_r;	        //쓰기 파일 디스크립터, 읽을 파일 디스크립터
    size_t length;	        //read()의 리턴값
    int wordcnt = 1;      //읽은 파일의 문자의 개수
    char buf[50];	        //read()에서 읽는 버퍼
    int i = 0;		//for(),while() 문에 조건문 사용

    if((fd_w =open(fname,O_WRONLY|O_CREAT, 0666)) < 0){
        	fprintf(stderr,"creat error for %s \n", fname);
        	exit(1);
    }

    if((fd_r= open(frname,O_RDONLY |O_CREAT, 0666)) < 0){
        fprintf(stderr,"open error for %s \n",frname);
        exit(1);
    }

    setbuf(stdout,NULL);
    if ((read(fd_r,buf,50)) < 0){
        fprintf(stderr,"read range over");
        exit(1);
    }

    close(fd_r);
    dup2(1,fd_r);
    dup2(fd_w,1);	
    close(fd_w);

    while(1)
    {
        printf("%c",buf[i]);
        if(buf[i]==' '){    
            printf("\n");	
            wordcnt++;
        }
        if(buf[i]=='\0')
            break;
        i++;
    }

    dup2(fd_r,1);
    printf("wordcount = %d \n",wordcnt);
}
