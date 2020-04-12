#include "ssu_score.h"

/*					COPYRIGHT					*\
 	writer : CHOI YOUNG RIM,Soongsil University
\*												*/

void ssu_runtime(struct timeval * begin_t,struct timeval * end_t)
{
	end_t->tv_sec -= begin_t->tv_sec; // 종료 시간에서 시작 시간을 초 단위로 빼기

	if(end_t->tv_usec < begin_t->tv_usec) // 만약 종료 마이크로 초가 시작 마이크로 초보다 작다면
	{
		end_t->tv_sec--; // 단위 내림
		end_t->tv_usec += SECOND_TO_MICRO;
	}

	end_t->tv_usec -= begin_t->tv_usec; // 종료 마이크로초 - 시작 마이크로초
	printf("Runtime: %ld:%06ld(sec:usec)\n",end_t->tv_sec,end_t->tv_usec);
}

void input_check(int argc,char *argv[])
{
	int i,opt;
	DIR *dir_std, *dir_ans;

	/* initialize */
	opterr = FALSE; // option error 출력 off
	saved_argc = argc;
	saved_argv = (char **)malloc(sizeof(char *)*saved_argc);
	isAppend = FALSE;

	for(i = 0; i < saved_argc; i++)
	{
		saved_argv[i] = (char *)malloc(sizeof(char)*STRLEN);
		memset((char *)saved_argv[i],0,STRLEN);
		strncpy(saved_argv[i],argv[i],strlen(argv[i]));
	}

	options.idx_e = options.idx_t = options.idx_c = options.idx_p = 0;
	options.opt_h = options.opt_e = options.opt_t = options.opt_c = options.opt_p = FALSE;

	if(argc < 2)
		option_help();

	/* getopt() : 옵션 처리 함수 */
	while((opt = getopt(argc,argv,"hpc:e:t:")) != EOF)
	{
		switch(opt)
		{
			case 'h' :
				options.opt_h = TRUE;
				break;
			case 'p' :
				options.opt_p = TRUE;
				break;
			case 'c' :
				options.opt_c = TRUE;
				options.idx_c = optind-1;
				break;
			case 'e' :
				options.opt_e = TRUE;
				options.idx_e = optind-1;
				break;
			case 't' :
				options.opt_t = TRUE;
				options.idx_t = optind-1;
				break;
			case '?' :
				option_help();
		}
	}

	if(argc > 2)
	{
		/* 초과된 매개변수 처리 함수 */
		out_of_arguments(options.opt_c,options.idx_c);
		out_of_arguments(options.opt_e,options.idx_e);
		out_of_arguments(options.opt_t,options.idx_t);

		if(options.opt_h)
			option_help();
		if(!(options.opt_c)) // no 'c' option
		{
			if((dir_std = opendir(saved_argv[1])) == NULL || (dir_ans = opendir(saved_argv[2])) == NULL)
				option_help();
			closedir(dir_std);
			closedir(dir_ans);
		}
		else // 'c' option exists
		{
			if(!strncmp(saved_argv[1],"-c",strlen("-c"))) // 'c' option만 존재
				option_c(TRUE);
			if((dir_std = opendir(saved_argv[1])) == NULL || (dir_ans = opendir(saved_argv[2])) == NULL)
				option_help();

			closedir(dir_std);
			closedir(dir_ans);
		}
		memset((char *)fpwd,0,STRLEN);
		memset((char *)fstd,0,STRLEN);
		memset((char *)fans,0,STRLEN);
		memset((char *)fcsv,0,STRLEN);
		memset((char *)fpnt,0,STRLEN);
		realpath(".",fpwd);
		realpath(saved_argv[1],fstd);
		realpath(saved_argv[2],fans);
	}
	else
		option_help();
}

void before_grading(void)
{
	FILE * fp_fpnt;
	char choice;
	int i,j = 0,isExist;
	double score;
	char ch,message[EXTENDED_STRLEN];

	memset((char *)message,0,EXTENDED_STRLEN);
	sprintf(fpnt,"%s/score_table.csv", fans);
	sprintf(fcsv,"%s/score.csv", fpwd);

	// versionsort -> _GNU_SOURCE
	if((count_std = scandir(fstd,&flist_std,0,versionsort)) < 0)
	{
		sprintf(message,"scandir error for %s", fstd);
		error_message(message);
	}
	if((count_ans = scandir(fans,&flist_ans,0,versionsort)) < 0)
	{
		sprintf(message,"scandir error for %s", fans);
		error_message(message);
	}

	/* 학생 구조체 동적할당 */

	num_std = 0;
	for(i = 0; i < count_std; i++)
		if(strstr(flist_std[i]->d_name,".") == NULL)
			num_std++;

	students = (Student *)malloc(sizeof(Student)*num_std);

	for(i = 0; i < count_std; i++)
	{
		if(strstr(flist_std[i]->d_name,".") == NULL)
		{
			memset((char *)students[j].sid,0,STRLEN);
			strncpy(students[j++].sid,flist_std[i]->d_name,strlen(flist_std[i]->d_name));
		}
	}

	num_ans = 0;
	for(i = 0; i < count_ans; i++)
		if(strstr(flist_ans[i]->d_name,".csv") == NULL && strstr(flist_ans[i]->d_name,".") == NULL)
			num_ans++;

	for(i = 0; i < num_std; i++)
		students[i].prob = (Problem *)malloc(sizeof(Problem)*num_ans);
	isAppend = TRUE;

	/* 채점표가 존재하는가? */

	isExist = access(fpnt,F_OK);
	if(isExist == -1)
	{
		fprintf(stderr,"score_table.csv file doesn't exist in %s directory!\n", fans);
		printf("1. input blank question and program's score ex) 0.5 1\n"
				"2. input all question's score ex) Input value of 1-1 : 0.1\n"
				"select type >> ");
		scanf("%c", &choice);
		make_score_table(choice); // 채점 기준표 만들기
	}

	/* 학생 구조체 배열에 데이터 input */

	if((fp_fpnt = fopen(fpnt,"r+")) == NULL)
		error_message("fopen() error for score_table.csv");

	j = 0;
	while(TRUE)
	{
		memset((char *)message,0,EXTENDED_STRLEN);
		fscanf(fp_fpnt,"%s",message);
		ch = fgetc(fp_fpnt);
		if(ch == EOF) break;
		fscanf(fp_fpnt,"%lf", &score);
		ch = fgetc(fp_fpnt);

		for(i = 0; i < num_std; i++)
		{
			strncpy(students[i].prob[j].pid,message,strlen(message));
			students[i].prob[j].score = score;
		}
		j++;
	}

	fclose(fp_fpnt);
}

void grading(void) // 채점
{
	int i,j;
	char fstd_id[EXTENDED_STRLEN]; // 각 학생의 학번(절대경로)
	char fstd_err[EXTENDED_STRLEN];

	printf("grading student's papers..\n");

	answer_compile(); // 정답파일 컴파일 및 실행결과 저장

	if(options.opt_e) // compile error 및 warning을 저장하는 옵션
	{
		memset((char *)ferr,0,STRLEN);
		sprintf(ferr,"%s/%s",fpwd,saved_argv[options.idx_e]);
		if(access(ferr,F_OK) != -1)
			file_remove(ferr,TRUE);
		make_dir(ferr);
	}

	for(i = 0; i < num_std; i++) // 학생 한명씩 순회
	{
		int count_std_id;
		struct dirent ** flist_std_id;

		memset((char *)fstd_id,0,EXTENDED_STRLEN);
		memset((char *)fstd_err,0,EXTENDED_STRLEN);
		sprintf(fstd_id,"%s/%s", fstd, students[i].sid);

		chdir(fstd_id); // 학생들 디렉토리 순회

		if((count_std_id = scandir(fstd_id,&flist_std_id,0,versionsort)) < 0)
			error_message("scandir() error for student's id");
		for(j = 0; j < count_ans; j++) // 답안 순회
		{
			if(strstr(flist_ans[j]->d_name,".") == NULL && strstr(flist_ans[j]->d_name,"csv") == NULL)
			{
				if(strstr(flist_ans[j]->d_name,"-") != NULL) // 빈칸 문제
					blank_main(i,j);
				else // 프로그램 문제
					program_main(i,j);
			}
		}
		if(options.opt_p)
		{
			students[i].sum = option_p(i);
			printf("%s is finished..score : %-.1lf\n", students[i].sid,students[i].sum);
		}
		else
			printf("%s is finished...\n", students[i].sid);
	}
	chdir(fpwd); // 원점 복귀
}

void answer_compile(void) // 정답파일 컴파일
{
	int i;
	char temp[STRLEN];
	char exe_out[STRLEN];
	char fans_dir[EXTENDED_STRLEN];
	char fans_prog[EXTENDED_STRLEN];
	char gcc[EXTENDED_STRLEN];

	for(i = 0; i < count_ans; i++)
	{
		memset((char *)temp,0,STRLEN);
		strncpy(temp,flist_ans[i]->d_name,strlen(flist_ans[i]->d_name));
		if(strstr(temp,".") == NULL && strstr(temp,".csv") == NULL && strstr(temp,"-") == NULL)
		{
			memset((char *)gcc,0,EXTENDED_STRLEN);
			memset((char *)fans_dir,0,EXTENDED_STRLEN);
			memset((char *)fans_prog,0,EXTENDED_STRLEN);
			sprintf(fans_dir,"%s/%s",fans,temp);
			sprintf(fans_prog,"%s/%s/%s.c",fans,temp,temp);
			sprintf(gcc,"gcc -o %s.exe %s -lpthread",temp,fans_prog);
			chdir(fans_dir);
			system(gcc); // compile
			memset((char *)exe_out,0,STRLEN);
			memset((char *)gcc,0,EXTENDED_STRLEN);
			sprintf(gcc,"./%s.exe",temp);
			sprintf(exe_out,"%s.stdout",temp);
			save_stdout(exe_out,gcc,TRUE); // 실행결과 저장
		}
	}
	chdir(fpwd);
}

void save_stdout(const char * exe_out,const char * execute,bool isStdout)
{
	FILE * save_file;

	if(isStdout) // stdout
	{
		save_file = freopen(exe_out,"w",stdout);
		system(execute);
		save_file = freopen("/dev/tty","a",stdout);
	}
	else // stderr
	{
		save_file = freopen(exe_out,"w",stderr);
		system(execute);
		save_file = freopen("/dev/tty","a",stderr);
	}

	//fclose(save_file);
}

bool isModified(const char * f,struct stat statbuf) // 파일이 수정되었는가?
{
	char message[STRLEN];
	struct stat new_stat;

	memset((char *)message,0,STRLEN);
	sprintf(message,"stat() error for %s",f);

	if(stat(f,&new_stat) < 0)
		error_message(message);

	if(statbuf.st_size != new_stat.st_size)
		return TRUE;
	return FALSE;
}

void make_dir(const char * path)
{
	char dir[EXTENDED_STRLEN];

	memset((char *)dir,0,EXTENDED_STRLEN);
	sprintf(dir,"mkdir %s",path);
	system(dir);
}

void file_copy(const char * from,const char * to) // file copy
{
	char copy[EXTENDED_STRLEN];

	memset((char *)copy,0,EXTENDED_STRLEN);
	sprintf(copy,"cp %s %s",from,to);
	system(copy);
}

void file_remove(const char * file,bool isDir)
{
	char remove[EXTENDED_STRLEN];

	memset((char *)remove,0,EXTENDED_STRLEN);
	if(isDir) // 디렉토리
		sprintf(remove,"rm -r %s",file);
	else // 일반 파일
		sprintf(remove,"rm %s",file);
	system(remove);
}

void make_score_table(char choice)
{
	int i,j;
	FILE * fp;
	double blank,program;
	char problem[EXTENDED_STRLEN];

	if((fp = fopen(fpnt,"w+")) == NULL)
		error_message("fopen() error for score_table.csv");

	if(choice == '1') // 통으로 input
	{
		printf("Input value of blank question : ");
		scanf("%lf", &blank);
		printf("Input value of program question : ");
		scanf("%lf", &program);

		for(i = 0; i < count_ans; i++) 
		{
			if(strstr(flist_ans[i]->d_name,".csv") == NULL && strstr(flist_ans[i]->d_name,".") == NULL)
			{
				memset((char *)problem,0,EXTENDED_STRLEN);
				if(strstr(flist_ans[i]->d_name,"-") != NULL) // blank problem
				{
					sprintf(problem,"%s.txt",flist_ans[i]->d_name);
					fprintf(fp,"%s\t%-.2lf\n",problem,blank);
				}
				else // program problem
				{
					strncpy(problem,flist_ans[i]->d_name,strlen(flist_ans[i]->d_name));
					fprintf(fp,"%s\t%-.2lf\n",problem,program);
				}
			}
		}
	}
	else if(choice == '2') // 하나하나 input
	{
		for(i = 0; i < count_ans; i++)
		{
			if(strstr(flist_ans[i]->d_name,".csv") == NULL && strstr(flist_ans[i]->d_name,".") == NULL)
			{
				memset((char *)problem,0,EXTENDED_STRLEN);
				if(strstr(flist_ans[i]->d_name,"-") != NULL)
					sprintf(problem,"%s.txt",flist_ans[i]->d_name);
				else
					sprintf(problem,"%s",flist_ans[i]->d_name);
				printf("Input of %s: ", problem);
				scanf("%lf", &blank);
				fprintf(fp,"%s\t%-.2lf\n",problem,blank);
			}
		}
	}
	else
		error_message("invalid select type");

	fclose(fp);
}

void out_of_arguments(bool option,int idx)
{
	if(option)
	{
		int count = 1;

		while(idx < saved_argc && strstr(saved_argv[idx],"-") == NULL)
		{
			if(count > OPTLEN)
				printf("Maximum Number of Argument Exceeded. :: %s\n", saved_argv[idx]);
			count++;
			idx++;
		}
	}
}

void option_help(void)
{
	printf("Usage : ssu_score <STUDENT_DIR> <ANSWER_DIR> [OPTION]\n");
	printf("Option :\n");
	printf(" -e  <DIR_NAME> \tprint error on 'DIR_NAME/ID/qname_error.txt' file\n");
	printf(" -t  <QNAMES>   \tcompile QNAME.c wit -lpthread option\n");
	printf(" -h				\tprint usage\n");
	printf(" -p				\tprint student's score and total average\n");
	printf(" -c <IDS>		\tprint ID's score\n");

	free_allocated_mem();
	gettimeofday(&end,0);
	ssu_runtime(&begin,&end);
	exit(EXIT_SUCCESS);
}

void option_c(bool check)
{
	int i,j,limit,e;
	int limit_idx = option_limit_idx(options.idx_c);
	char std_id[EXTENDED_STRLEN];
	char temp[EXTENDED_STRLEN];
	double score;
	FILE * fp;
	bool isChecked = FALSE;

	if(check)
	{
		memset((char *)fcsv,0,STRLEN);
		sprintf(fcsv,"score.csv");
	}
	if(access(fcsv,F_OK) == -1)
		error_message("score.csv does not exist");
	if((fp = fopen(fcsv,"r")) == NULL)
		error_message("fopen() error for score.csv");
	fseek(fp,0,SEEK_END);
	e = ftell(fp);
	fseek(fp,0,SEEK_SET);
	fscanf(fp,"\t");
	limit = 0;

	while(strstr(temp,"sum") == NULL)
	{
		memset((char *)temp,0,EXTENDED_STRLEN);
		fscanf(fp,"%s\t",temp);
		limit++;
	}
	fscanf(fp,"\n");
	memset((char *)temp,0,EXTENDED_STRLEN);

	for(i = options.idx_c; i < limit_idx; i++)
	{
		if(!isChecked)
		{
			fscanf(fp,"%s",temp);
			fscanf(fp,"\t");
		}
		memset((char *)std_id,0,EXTENDED_STRLEN);
		strncpy(std_id,saved_argv[i],strlen(saved_argv[i]));
	
		if(!strcmp(std_id,temp))
		{
			isChecked = FALSE;
			j = 1;
			while(j < limit)
			{
				fscanf(fp,"%lf",&score);
				fscanf(fp,"\t");
				j++;
			}
			fscanf(fp,"%lf",&score);
			fscanf(fp,"\n");
			printf("%s's score : %lf\n",std_id,score);
		}
		else
		{
			int cur = ftell(fp);

			while(strcmp(std_id,temp))
			{
				if(cur == e)
					break;
				j = 1;
				while(j < limit)
				{
					fscanf(fp,"%lf",&score);
					fscanf(fp,"\t");
					j++;
				}
				fscanf(fp,"%lf",&score);
				fscanf(fp,"\n");
				memset((char *)temp,0,EXTENDED_STRLEN);
				fscanf(fp,"%s",temp);
				fscanf(fp,"\t");
				cur = ftell(fp);
			}
			if(!strcmp(std_id,temp))
			{
				isChecked = TRUE;
				i--;
			}
		}
	}
	if(check)
	{
		free_allocated_mem();
		gettimeofday(&end,NULL);
		ssu_runtime(&begin,&end);
		fclose(fp);
		exit(EXIT_SUCCESS);
	}
	fclose(fp);
}

void make_result(void)
{
	int i,j;
	FILE * fp;
	double sum;

	if((fp = fopen(fcsv,"w+")) == NULL)
		error_message("fopen() error for score.csv");

	i = 0;
	fseek(fp,0,SEEK_SET);
	fprintf(fp,"\t");

	while(i < num_ans)
	{
		fprintf(fp,"%s\t",students[0].prob[i].pid);
		i++;
	}
	fprintf(fp,"sum\n");

	i = 0;
	while(i < num_std)
	{
		j = 0;
		sum = option_p(i);
		fprintf(fp,"%s\t",students[i].sid);
		while(j < num_ans)
		{
			fprintf(fp,"%lf",students[i].prob[j].score);
			fprintf(fp,"\t");
			j++;
		}
		fprintf(fp,"%lf",sum);
		fprintf(fp,"\n");
		i++;
	}
	fclose(fp);
}

double option_p(int std_idx)
{
	int i;
	double sum = 0;

	for(i = 0; i < num_ans; i++)
		sum += students[std_idx].prob[i].score;
	return sum;
}

void free_allocated_mem(void)
{
	int i;

	for(i = 0; i < saved_argc; i++)
		free(saved_argv[i]);
	free(saved_argv);

	if(isAppend) // 학생 구조체 배열까지 동적할당됐을 때
	{
		for(i = 0; i < num_std; i++)
			free(students[i].prob);
		free(students);
	}
}

void error_message(char * message)
{
	fprintf(stderr,"%s\n", message);
	free_allocated_mem();
	gettimeofday(&end,NULL);
	ssu_runtime(&begin,&end);
	exit(EXIT_FAILURE);
}
