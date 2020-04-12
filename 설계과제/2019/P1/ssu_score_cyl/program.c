#include "ssu_score.h"

/*					COPYRIGHT					*\
 	writer : CHOI YOUNG RIM,Soongsil University
\*												*/

int program_main(int std_idx,int ans_idx)
{
	int i,isExist,prob_idx;
	char ans_prob[STRLEN];
	char fstd_prog[EXTENDED_STRLEN];

	memset((char *)ans_prob,0,STRLEN);
	memset((char *)fstd_prog,0,EXTENDED_STRLEN);

	strncpy(ans_prob,flist_ans[ans_idx]->d_name,strlen(flist_ans[ans_idx]->d_name));
	sprintf(fstd_prog,"%s/%s/%s.c",fstd,students[std_idx].sid,flist_ans[ans_idx]->d_name);
	isExist = access(fstd_prog,F_OK); // 학생 폴더에 문제 파일이 존재X -> 0점
	if((prob_idx = find_prob_idx(ans_prob,std_idx)) < 0)
		error_message("invalid index returned");

	if(isExist == -1) // 학생의 파일이 존재하지 않음
	{
		students[std_idx].prob[prob_idx].score = WRONG;
		return 0;
	}

	student_compile(std_idx,ans_idx); // 학생파일 컴파일
	student_execute(std_idx,ans_idx); // 실행
	student_stdout(std_idx,ans_idx); // 실행결과 비교 및 채점
	return 0;
}

int find_prob_idx(const char * prob_name,int std_idx)
{
	int i;

	for(i = 0; i < num_ans; i++)
	{
		if(!strncmp(students[std_idx].prob[i].pid,prob_name,strlen(prob_name)))
			return i;
	}
	return -1;
}

void student_compile(int std_idx,int ans_idx)
{
	int i,prob_idx;
	char errtxt[EXTENDED_STRLEN]; // ERROR/2019XXXX/XX_error.txt
	char errpath[EXTENDED_STRLEN]; // ERROR/2019XXXX
	char gcc[EXTENDED_STRLEN]; // gcc or ./
	char fstd_prog[EXTENDED_STRLEN];
	FILE * temp; // 컴파일 에러를 저장할 임시 파일
	struct stat statbuf;

	memset((char *)gcc,0,EXTENDED_STRLEN);
	memset((char *)errpath,0,EXTENDED_STRLEN);
	memset((char *)fstd_prog,0,EXTENDED_STRLEN);
	
	if(options.opt_e) // error printing option
		sprintf(errpath,"%s/%s",ferr,students[std_idx].sid);
	if(access("temp.txt",F_OK) == 0)
		file_remove("temp.txt",FALSE);

	if((prob_idx = find_prob_idx(flist_ans[ans_idx]->d_name,std_idx)) < 0) // 학생 구조체의 문제 indx 파악
		error_message("invalid index returned");
	sprintf(fstd_prog,"%s/%s/%s.c",fstd,students[std_idx].sid,flist_ans[ans_idx]->d_name);

	if(options.opt_t) // -lpthread를 붙여야 함
	{
		bool check = FALSE;
		int limit_idx = option_limit_idx(options.idx_t);

		for(i = options.idx_t; i < limit_idx; i++)
		{
			if(!strncmp(flist_ans[ans_idx]->d_name,saved_argv[i],strlen(saved_argv[i])))
			{
				check = TRUE;
				sprintf(gcc,"gcc -o %s.exe %s -lpthread",flist_ans[ans_idx]->d_name,fstd_prog);
				break;
			}
		}
		if(!check)
			sprintf(gcc,"gcc -o %s.exe %s",flist_ans[ans_idx]->d_name,fstd_prog);
	}
	else
		sprintf(gcc,"gcc -o %s.exe %s",flist_ans[ans_idx]->d_name,fstd_prog);

	if(options.opt_e) // error 저장 옵션
	{
		if((temp = fopen("error.txt","w+")) == NULL)
			error_message("fopen() error for error text");
		fclose(temp);
		if(stat("error.txt",&statbuf) < 0)
			error_message("stat() error for error text");
		memset((char *)errtxt,0,EXTENDED_STRLEN);
		sprintf(errtxt,"%s/%s_error.txt",errpath,flist_ans[ans_idx]->d_name);
		save_stdout("error.txt",gcc,FALSE); // compile - stderr에 출력되는 내용을 회수
		if(find_error("error.txt",statbuf,std_idx,prob_idx))
			file_copy("error.txt",errtxt);
		if(access("error.txt",F_OK) == 0)
			file_remove("error.txt",FALSE);
	}
	else
	{
		if((temp = fopen("temp.txt","w+")) == NULL) // 임시로 compile 에러 메시지 저장
			error_message("fopen() error for temporary file");
		fclose(temp); // 만들고 바로 종료
		if(stat("temp.txt",&statbuf) < 0) // 파일 설정 저장
			error_message("stat() error for temporary file");
		save_stdout("temp.txt",gcc,FALSE); // compile - stderr에 출력되는 내용을 회수
		find_error("temp.txt",statbuf,std_idx,prob_idx);
	}
	if(access("temp.txt",F_OK) == 0)
		file_remove("temp.txt",FALSE);
}

int option_limit_idx(int idx_opt)
{
	while(idx_opt < saved_argc && strstr(saved_argv[idx_opt],"-") == NULL)
		idx_opt++;
	return idx_opt;
}

bool find_error(const char * file,struct stat statbuf,int std_idx,int prob_idx)
{
	bool check = FALSE;
	char fstd_err[EXTENDED_STRLEN];

	memset((char *)fstd_err,0,EXTENDED_STRLEN);
	sprintf(fstd_err,"%s/%s",ferr,students[std_idx].sid);

	if(isModified(file,statbuf)) // 수정되었음
	{
		if(access(fstd_err,F_OK) == -1) // doesn't exist
			make_dir(fstd_err);
		FILE * fp;
		char data[STRLEN];
	
		if((fp = fopen(file,"r")) == NULL)
			error_message("fopen() error");
		memset((char *)data,0,STRLEN);
		while(fgets(data,STRLEN,fp) != NULL) // error 검출
		{
			if(strstr(data,"error:") != NULL) // error - WRONG
			{
				check = TRUE;
				students[std_idx].prob[prob_idx].score = WRONG;
				break;
			}
			else if(strstr(data,"warning:") != NULL) // warning - WARNING
			{
				check = TRUE;
				students[std_idx].prob[prob_idx].score -= WARNING;
			}
			memset((char *)data,0,STRLEN);
		}
		fclose(fp);
	}
	else
		file_remove(file,FALSE);
	return check;
}

void student_execute(int std_idx,int ans_idx)
{
	pthread_t t_id;
	int prob_idx;
	char fstd_exe[EXTENDED_STRLEN];
	char fstd_out[EXTENDED_STRLEN];
	char execute[EXTENDED_STRLEN];
	char process_kill[EXTENDED_STRLEN];
	char message[EXTENDED_STRLEN];

	memset((char *)fstd_exe,0,EXTENDED_STRLEN);
	memset((char *)fstd_out,0,EXTENDED_STRLEN);
	memset((char *)execute,0,EXTENDED_STRLEN);
	memset((char *)process_kill,0,EXTENDED_STRLEN);
	memset((char *)message,0,EXTENDED_STRLEN);

	if((prob_idx = find_prob_idx(flist_ans[ans_idx]->d_name,std_idx)) < 0) 
		error_message("invalid index returned");

	sprintf(fstd_exe,"%s.exe",flist_ans[ans_idx]->d_name);
	sprintf(fstd_out,"%s.stdout",flist_ans[ans_idx]->d_name);
	sprintf(execute,"./%s",fstd_exe);
	sprintf(process_kill,"killall %s", fstd_exe); // killall XX.exe

	if(pthread_create(&t_id,NULL,execute_process,(void *)execute) != 0)
	{
		sprintf(message,"pthread_create() error for %s",fstd_exe);
		error_message(message);
	}

	if(isTimeout(fstd_exe)) // timeout
	{
		students[std_idx].prob[prob_idx].score = WRONG;
		system(process_kill); // 시간초과 프로세스 kill
	}

	pthread_join(t_id,NULL); // thread 분리
}

void student_stdout(int std_idx,int ans_idx)
{
	int prob_idx;
	char fstd_out[EXTENDED_STRLEN]; // 학생의 실행결과
	char fans_out[EXTENDED_STRLEN]; // 정답의 실행결과
	char std_buf[STRLEN]; // 학생 파일 내용 읽어올 변수
	char ans_buf[STRLEN]; // 정답 파일 내용 읽어올 변수
	FILE * fp_std, * fp_ans;

	memset((char *)fstd_out,0,EXTENDED_STRLEN);
	memset((char *)fans_out,0,EXTENDED_STRLEN);
	memset((char *)std_buf,0,STRLEN);
	memset((char *)ans_buf,0,STRLEN);

	if((prob_idx = find_prob_idx(flist_ans[ans_idx]->d_name,std_idx)) < 0)
		error_message("invalid index returned");

	sprintf(fstd_out,"%s.stdout",students[std_idx].prob[prob_idx].pid); // XX.stdout
	sprintf(fans_out,"%s/%s/%s.stdout",fans,flist_ans[ans_idx]->d_name,flist_ans[ans_idx]->d_name);
	if((fp_ans = fopen(fans_out,"r+")) == NULL)
		error_message("fopen() error for answer_file");
	if((fp_std = fopen(fstd_out,"r+")) == NULL)
		error_message("fopen() error for student_file");

	while(fgets(ans_buf,STRLEN,fp_ans) != NULL) // 한줄씩 읽기
	{
		if(fgets(std_buf,STRLEN,fp_std) != NULL)
		{
			if(strncmp(ans_buf,std_buf,strlen(ans_buf)))
			{
				students[std_idx].prob[prob_idx].score = WRONG;
				break;
			}
		}
		else // 정답 파일이 다 안끝났는데 학생 파일을 전부 읽음 -> WRONG
		{
			students[std_idx].prob[prob_idx].score = WRONG;
			break;
		}
		memset((char *)std_buf,0,STRLEN);
		memset((char *)ans_buf,0,STRLEN);
	}

	fclose(fp_ans);
	fclose(fp_std);
}

void * execute_process(void * arg)
{
	int i=2;
	char buf[EXTENDED_STRLEN];
	char path_out[EXTENDED_STRLEN];

	memset((char *)buf,0,EXTENDED_STRLEN);
	memset((char *)path_out,0,EXTENDED_STRLEN);
	sprintf(buf,"%s",(char *)arg);

	while(i < strlen(arg) - 3)
	{
		path_out[i-2] = buf[i];
		i++;
	}

	sprintf(path_out,"%sstdout",path_out);
	sprintf(buf,"%s &",buf);

	save_stdout(path_out,buf,TRUE);
}

bool isTimeout(char * process)
{
	int i,fd;
	char buf[EXTENDED_STRLEN];
	char use[EXTENDED_STRLEN];

	for(i = 0; i < TIMEOUT; i++)
	{
		memset((char *)buf,0,EXTENDED_STRLEN);
		memset((char *)use,0,EXTENDED_STRLEN);

		sprintf(use,"ps -e | grep ");
		sprintf(use,"%s%s",use,process);

		FILE * fre_out = freopen("TEMPOUT.txt","w",stdout);
		FILE * fre_err = freopen("/dev/null","w",stderr);

		system(use);
		fflush(stdout);
		fflush(stderr);

		if((fd = open("TEMPOUT.txt",O_RDONLY)) < 0)
			error_message("open() error for isTimeout()");

		read(fd,buf,EXTENDED_STRLEN);

		if(!buf[0])
		{
			fre_out = freopen("/dev/tty","a",stdout);
			fre_err = freopen("/dev/tty","a",stderr);
			if(access("TEMPOUT.txt",F_OK) == 0)
				file_remove("TEMPOUT.txt",FALSE);
			close(fd);
			return FALSE;
		}
		if(strstr(buf,process) == NULL)
		{
			fre_out = freopen("/dev/tty","a",stdout);
			fre_err = freopen("/dev/tty","a",stderr);
			if(access("TEMPOUT.txt",F_OK) == 0)
				file_remove("TEMPOUT.txt",FALSE);
			close(fd);
			return FALSE;
		}

		fre_out = freopen("/dev/tty","a",stdout);
		fre_err = freopen("/dev/tty","a",stderr);
		if(access("TEMPOUT.txt",F_OK) == 0)
			file_remove("TEMPOUT.txt",FALSE);
		close(fd);
		sleep(1);
	}
	return TRUE;
}
