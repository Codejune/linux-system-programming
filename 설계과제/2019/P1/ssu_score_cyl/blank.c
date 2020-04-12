#include "ssu_score.h"

/*					 COPYRIGHT                          *\
  most of this C file : 2019 SoongSil University OS LAB
\*														*/

// 자료형의 종류 구분(DATATYPE_SIZE : 35, MINLEN : 64)
char datatype[DATATYPE_SIZE][MINLEN] = {"int", "char", "double", "float", "long"
			, "short", "ushort", "FILE", "DIR","pid"
			,"key_t", "ssize_t", "mode_t", "ino_t", "dev_t"
			, "nlink_t", "uid_t", "gid_t", "time_t", "blksize_t"
			, "blkcnt_t", "pid_t", "pthread_mutex_t", "pthread_cond_t", "pthread_t"
			, "void", "size_t", "unsigned", "sigset_t", "sigjmp_buf"
			, "rlim_t", "jmp_buf", "sig_atomic_t", "clock_t", "struct"};

// 연산자의 우선순위(OPERATOR_CNT : 24)
operator_precedence operators[OPERATOR_CNT] = {
	{"(", 0}, {")", 0} // 괄호가 최우선
	,{"->", 1}	// 포인터 구조체 인덱싱
	,{"*", 4}	,{"/", 3}	,{"%", 2} // 곱셈, 나눗셈, 나머지
	,{"+", 6}	,{"-", 5} // 덧셈 뺄셈
	,{"<", 7}	,{"<=", 7}	,{">", 7}	,{">=", 7} // 비교 연산자
	,{"==", 8}	,{"!=", 8} 
	,{"&", 9} // bit AND
	,{"^", 10} // 제곱
	,{"|", 11} // bit OR
	,{"&&", 12} // compare AND
	,{"||", 13} // compare OR
	,{"=", 14}	,{"+=", 14}	,{"-=", 14}	,{"&=", 14}	,{"|=", 14} // assignment operators
};

int blank_main(int std_idx,int ans_idx)
{
	int i,prob_idx;
	char fstd_txt[EXTENDED_STRLEN]; // X-X.txt
	char fans_txt[EXTENDED_STRLEN]; // 정답파일(절대경로)

	memset((char *)fstd_txt,0,EXTENDED_STRLEN);
	memset((char *)fans_txt,0,EXTENDED_STRLEN);

	if((prob_idx = find_prob_idx(flist_ans[ans_idx]->d_name,std_idx)) < 0)
		error_message("invalid index returned");

	sprintf(fstd_txt,"%s.txt",flist_ans[ans_idx]->d_name); // 학생 폴더의 XX-XX.txt
	sprintf(fans_txt,"%s/%s/%s",fans,flist_ans[ans_idx]->d_name,students[std_idx].prob[prob_idx].pid); // 정답 폴더의 절대 경로
 
	if(isMatched(fstd_txt,fans_txt)) 
		return TRUE;
	else
	{
		students[std_idx].prob[prob_idx].score = WRONG;
		return FALSE;
	}
}

/* 빈칸 비교 */
bool isMatched(const char * fstd_txt,const char * fans_txt)
{
	int idx, start;
	int fd_std, fd_ans;
	int result = TRUE;
	int has_semicolon = FALSE;
	char tokens[TOKEN_CNT][MINLEN];
	node *std_root = NULL, *ans_root = NULL;
	char s_answer[EXTENDED_STRLEN], a_answer[EXTENDED_STRLEN];

	if(access(fstd_txt,F_OK) == -1) // 학생 파일이 존재하지 않음
		return FALSE;
	if((fd_std = open(fstd_txt,O_RDONLY)) < 0)
		error_message("open() error for student file");
	strcpy(s_answer, get_answer(fd_std, s_answer));

	if(!strcmp(s_answer, ""))
	{
		close(fd_std);
		return FALSE;
	}
	if(!check_brackets(s_answer))
	{
		close(fd_std);
		return FALSE;
	}

	strcpy(s_answer, ltrim(rtrim(s_answer)));

	if(s_answer[strlen(s_answer) - 1] == ';') // ';'으로 끝난다면
	{
		has_semicolon = TRUE;
		s_answer[strlen(s_answer) - 1] = '\0';
	}

	if(!make_tokens(s_answer, tokens))
	{
		close(fd_std);
		return FALSE;
	}

	idx = 0;
	std_root = make_tree(std_root, tokens, &idx, 0);

	if((fd_ans = open(fans_txt,O_RDONLY)) < 0)
		error_message("open() error for answer file");

	while(1)
	{
		ans_root = NULL;
		result = TRUE;

		for(idx = 0; idx < TOKEN_CNT; idx++)
			memset(tokens[idx], 0, sizeof(tokens[idx]));

		strcpy(a_answer, get_answer(fd_ans, a_answer));

		if(!strcmp(a_answer, ""))
			break;

		strcpy(a_answer, ltrim(rtrim(a_answer)));

		if(has_semicolon == FALSE){
			if(a_answer[strlen(a_answer) -1] == ';')
				continue;
		}

		else if(has_semicolon == TRUE)
		{
			if(a_answer[strlen(a_answer) - 1] != ';')
				continue;
			else
				a_answer[strlen(a_answer) - 1] = '\0';
		}

		if(!make_tokens(a_answer, tokens))
			continue;

		idx = 0;
		ans_root = make_tree(ans_root, tokens, &idx, 0);

		compare_tree(std_root, ans_root, &result);

		if(result == TRUE)
		{
			close(fd_std);
			close(fd_ans);

			if(std_root != NULL)
				free_node(std_root);
			if(ans_root != NULL)
				free_node(ans_root);
			return TRUE;

		}
	}
	
	close(fd_std);
	close(fd_ans);

	if(std_root != NULL)
		free_node(std_root);
	if(ans_root != NULL)
		free_node(ans_root);

	return FALSE;
}

/* 공백문자 제거 */
void remove_space(char * buf)
{
	int i = 0,j,len;

	len = strlen(buf);

	while(i < len && buf[i] != '\0')
	{
		if(buf[i] == ' ')
		{
			for(j = i; buf[j] != '\0'; j++)
				buf[j] = buf[j+1];
			continue;
		}
		i++;
	}
}

char * get_answer(int fd, char *result)
{
	char c;
	int idx = 0;

	memset((char *)result, 0, EXTENDED_STRLEN);
	// 한 글자씩 복사
	while(read(fd, &c, 1) > 0)
	{
		if(c == ':')
			break;
		
		result[idx++] = c;
	}
	if(result[strlen(result)-1] == '\n')
		result[strlen(result)-1] = '\0';
	return result;
}

// 트리 비교 함수
void compare_tree(node *root1,  node *root2, int *result)
{
	node *tmp;
	int cnt1, cnt2;

	// 비교할 트리 2개 중 하나라도 비어있는 트리라면 0 리턴
	if(root1 == NULL || root2 == NULL)
	{
		*result = FALSE;
		return;
	}

	if(!strcmp(root1->name, "<") || !strcmp(root1->name, ">") || !strcmp(root1->name, "<=") || !strcmp(root1->name, ">=")) // 비교 구문일 경우
	{
		if(strcmp(root1->name, root2->name))
		{
			if(!strncmp(root2->name, "<", 1))
				strncpy(root2->name, ">", 1);

			else if(!strncmp(root2->name, ">", 1))
				strncpy(root2->name, "<", 1);

			else if(!strncmp(root2->name, "<=", 2))
				strncpy(root2->name, ">=", 2);

			else if(!strncmp(root2->name, ">=", 2))
				strncpy(root2->name, "<=", 2);
			root2 = change_sibling(root2);
		}
	}
	if(strcmp(root1->name, root2->name) != 0)
	{
		*result = FALSE;
		return;
	}
	if((root1->child_head != NULL && root2->child_head == NULL)
		|| (root1->child_head == NULL && root2->child_head != NULL))
	{
		*result = FALSE;
		return;
	}
	else if(root1->child_head != NULL)
	{
		if(get_sibling_cnt(root1->child_head) != get_sibling_cnt(root2->child_head))
		{
			*result = FALSE;
			return;
		}
		if(!strcmp(root1->name, "==") || !strcmp(root1->name, "!="))
		{
			compare_tree(root1->child_head, root2->child_head, result);

			if(*result == FALSE)
			{
				*result = TRUE;
				root2 = change_sibling(root2);
				compare_tree(root1->child_head, root2->child_head, result);
			}
		}
		else if(!strcmp(root1->name, "+") || !strcmp(root1->name, "*")
				|| !strcmp(root1->name, "|") || !strcmp(root1->name, "&")
				|| !strcmp(root1->name, "||") || !strcmp(root1->name, "&&"))
		{
			if(get_sibling_cnt(root1->child_head) != get_sibling_cnt(root2->child_head)){
				*result = FALSE;
				return;
			}

			tmp = root2->child_head;

			while(tmp->prev != NULL)
				tmp = tmp->prev;

			while(tmp != NULL)
			{
				compare_tree(root1->child_head, tmp, result);
			
				if(*result == TRUE)
					break;
				else{
					if(tmp->next != NULL)
						*result = TRUE;
					tmp = tmp->next;
				}
			}
		}
		else
			compare_tree(root1->child_head, root2->child_head, result);
	}	
	if(root1->next != NULL){

		if(get_sibling_cnt(root1) != get_sibling_cnt(root2)){
			*result = FALSE;
			return;
		}

		if(*result == TRUE)
		{
			tmp = get_operator(root1);
	
			if(!strcmp(tmp->name, "+") || !strcmp(tmp->name, "*")
					|| !strcmp(tmp->name, "|") || !strcmp(tmp->name, "&")
					|| !strcmp(tmp->name, "||") || !strcmp(tmp->name, "&&"))
			{	
				tmp = root2;
	
				while(tmp->prev != NULL)
					tmp = tmp->prev;

				while(tmp != NULL)
				{
					compare_tree(root1->next, tmp, result);

					if(*result == TRUE)
						break;
					else{
						if(tmp->next != NULL)
							*result = TRUE;
						tmp = tmp->next;
					}
				}
			}
			else
				compare_tree(root1->next, root2->next, result);
		}
	}
}

// TOKEN_CNT : 50, MINLEN : 64
int make_tokens(char *str, char tokens[TOKEN_CNT][MINLEN])
{
	char *start, *end;
	char tmp[EXTENDED_STRLEN];
	char str2[EXTENDED_STRLEN];
	char *op = "(),;><=!|&^/+-*\""; 
	int row = 0;
	int i;
 	int isPointer;
	int lcount, rcount;
	int p_str;
	
	clear_tokens(tokens); // tokens를 초기화
	
	start = str; // 정답 문자열 pointing

	// is_typeStatement()의 return 값
	// 0 : 의미 없음
	// 1 : Casting과 같은 곳에서 사용
	// 2 : 자료형이 시작 단어임
	if(is_typeStatement(str) == 0) // 잘못된 사용
		return FALSE;	
	
	while(1)
	{
		// strpbrk() : 두 번째 인자 문자열의 문자들 중 하나라도 첫 번째 문자열에 존재한다면
		if((end = strpbrk(start, op)) == NULL) // start에서 op의 문자들 중 하나라도 없다면 break
			break;

		if(start == end)
		{
			if(!strncmp(start, "--", 2) || !strncmp(start, "++", 2))
			{
				if(!strncmp(start, "++++", 4) || !strncmp(start,"----",4))
					return FALSE;
				// ex) ++a
				if(is_character(*ltrim(start + 2)))
				{
					if(row > 0 && is_character(tokens[row - 1][strlen(tokens[row - 1])-1]))
						return FALSE; //ex) ++a++
					end = strpbrk(start + 2, op);
					if(end == NULL)
						end = &str[strlen(str)];
					strncpy(tokens[row], start, end - start);
					start += (end - start);
				}
				// ex) a++
				else if(row>0 && is_character(tokens[row - 1][strlen(tokens[row - 1])-1]))
				{
					if(strstr(tokens[row - 1], "++") != NULL || strstr(tokens[row - 1], "--") != NULL)	
						return FALSE;

					memset(tmp, 0, sizeof(tmp));
					strncpy(tmp, start, 2);
					strcat(tokens[row - 1], tmp);
					start += 2;
					row--;
				}
				else
				{
					memset(tmp, 0, sizeof(tmp));
					strncpy(tmp, start, 2);
					strcat(tokens[row], tmp);
					start += 2;
				}
			}
			else if(!strncmp(start, "==", 2) || !strncmp(start, "!=", 2) || !strncmp(start, "<=", 2)
				|| !strncmp(start, ">=", 2) || !strncmp(start, "||", 2) || !strncmp(start, "&&", 2) 
				|| !strncmp(start, "&=", 2) || !strncmp(start, "^=", 2) || !strncmp(start, "!=", 2) 
				|| !strncmp(start, "|=", 2) || !strncmp(start, "+=", 2)	|| !strncmp(start, "-=", 2) 
				|| !strncmp(start, "*=", 2) || !strncmp(start, "/=", 2))
			{

				strncpy(tokens[row], start, 2);
				start += 2;
			}
			else if(!strncmp(start, "->", 2))
			{
				end = strpbrk(start + 2, op);

				if(end == NULL)
					end = &str[strlen(str)];

				while(start < end)
				{
					if(*start != ' ')
						strncat(tokens[row - 1], start, 1);
					start++;
				}
				row--;
			}
			else if(*end == '&')
			{
				// ex) &a (address)
				if(row == 0 || (strpbrk(tokens[row - 1], op) != NULL))
				{
					end = strpbrk(start + 1, op);
					if(end == NULL)
						end = &str[strlen(str)];
					
					strncat(tokens[row], start, 1);
					start++;

					while(start < end){
						if(*start==' ' && tokens[row][strlen(tokens[row])-1] != '&')
							return FALSE;
						else if(*start != ' ')
							strncat(tokens[row], start, 1);
						start++;
					}
				}
				// ex) a & b (bit)
				else{
					strncpy(tokens[row], start, 1);
					start += 1;
				}
				
			}
		  	else if(*end == '*')
			{
				isPointer=0;

				if(row > 0)
				{
					//ex) char** (pointer)
					for(i = 0; i < DATATYPE_SIZE; i++) {
						if(strstr(tokens[row - 1], datatype[i]) != NULL){
							strcat(tokens[row - 1], "*");
							start += 1;	
							isPointer = 1;
							break;
						}
					}
					if(isPointer == 1)
						continue;
					if(*(start+1) !=0)
						end = start + 1;

					// ex) a * **b (multiply then pointer)
					if(row>1 && !strcmp(tokens[row - 2], "*") && (all_star(tokens[row - 1]) == 1)){
						strncat(tokens[row - 1], start, end - start);
						row--;
					}
					
					// ex) a*b(multiply)
					else if(is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1]) == 1){ 
						strncat(tokens[row], start, end - start);   
					}

					// ex) ,*b (pointer)
					else if(strpbrk(tokens[row - 1], op) != NULL){		
						strncat(tokens[row] , start, end - start); 
							
					}
					else
						strncat(tokens[row], start, end - start);

					start += (end - start);
				}

			 	else if(row == 0)
				{
					if((end = strpbrk(start + 1, op)) == NULL){
						strncpy(tokens[row], start, 1);
						start += 1;
					}
					else{
						strncpy(tokens[row], start, end - start);
						start += (end - start);
						
					}
				}
			}
			else if(*end == '(') 
			{
				lcount = 0;
				rcount = 0;
				if(row>0 && (strcmp(tokens[row - 1],"&") == 0 || strcmp(tokens[row - 1], "*") == 0)){
					while(*(end + lcount + 1) == '(')
						lcount++;
					start += lcount;

					end = strpbrk(start + 1, ")");

					if(end == NULL)
						return FALSE;
					else{
						while(*(end + rcount +1) == ')')
							rcount++;
						end += rcount;

						if(lcount != rcount)
							return FALSE;

						if( (row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1])) || row == 1){ 
							strncat(tokens[row - 1], start + 1, end - start - rcount - 1);
							row--;
							start = end + 1;
						}
						else{
							strncat(tokens[row], start, 1);
							start += 1;
						}
					}
						
				}
				else{
					strncat(tokens[row], start, 1);
					start += 1;
				}

			}
			else if(*end == '\"') 
			{
				end = strpbrk(start + 1, "\"");
				
				if(end == NULL)
					return FALSE;

				else{
					strncat(tokens[row], start, end - start + 1);
					start = end + 1;
				}

			}
			else
			{
				// ex) a++ ++ +b
				if(row > 0 && !strcmp(tokens[row - 1], "++"))
					return FALSE;

				// ex) a-- -- -b
				if(row > 0 && !strcmp(tokens[row - 1], "--"))
					return FALSE;
	
				strncat(tokens[row], start, 1);
				start += 1;
				
				// ex) -a or a, -b
				if(!strcmp(tokens[row], "-") || !strcmp(tokens[row], "+"))
				{
					// ex) -a or -a+b
					if(row == 0)
						row--;
					// ex) a+b = -c
					else if(!is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1]))
					{
						if(strstr(tokens[row - 1], "++") == NULL && strstr(tokens[row - 1], "--") == NULL)
							row--;
					}
				}
			}
		}
		else // start != end
		{ 
			if(row > 1 && all_star(tokens[row - 1]) && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1]))   
				row--;				
			if(all_star(tokens[row - 1]) && row == 1) // 첫 토큰이 '*'으로만 이뤄져 있음
				row--;	

			for(i = 0; i < end - start; i++)
			{
				if(start[i] == ' ')
				{
					while(start[i] == ' ')
						i++;
					break;
				}
			}

			if(start[0] == ' ')
			{
				start += i;
				continue;
			}
			strncat(tokens[row], start, i);
			start += i;
		}
			
		strcpy(tokens[row], ltrim(rtrim(tokens[row]))); // 좌우측 공백문자 제거 후 tokens[row]에 복사

		 if(row > 0 && is_character(tokens[row][strlen(tokens[row])-1]) 
				&& (is_typeStatement(tokens[row - 1]) == 2 // 자료형 선언임
					|| is_character(tokens[row - 1][strlen(tokens[row - 1])- 1])
					|| tokens[row - 1][strlen(tokens[row - 1]) - 1] == '.' ))
		 {

			if(row > 1 && strcmp(tokens[row - 2],"(") == 0)
			{
				if(strcmp(tokens[row - 1], "struct") != 0 && strcmp(tokens[row - 1],"unsigned") != 0)
					return FALSE;
			}
			else if(row == 1 && is_character(tokens[row][strlen(tokens[row]) - 1])) {
				if(strcmp(tokens[0], "extern") != 0 && strcmp(tokens[0], "unsigned") != 0 && is_typeStatement(tokens[0]) != 2)	
					return FALSE;
			}
			else if(row > 1 && is_typeStatement(tokens[row - 1]) == 2){
				if(strcmp(tokens[row - 2], "unsigned") != 0 && strcmp(tokens[row - 2], "extern")!=0)
					return FALSE;
			}
			
		}
		if((row == 0 && !strcmp(tokens[row], "gcc")) )
		{
			clear_tokens(tokens);
			strcpy(tokens[0], str);	
			return 1;
		} 
		row++;
	} // the end of while(1)

	if(all_star(tokens[row - 1]) && row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1]))  
		row--;				
	if(all_star(tokens[row - 1]) && row == 1)   
		row--;	

	for(i = 0; i < strlen(start); i++)   
	{
		if(start[i] == ' ')  
		{
			while(start[i] == ' ')
				i++;
			if(start[0]==' ')
			{
				start += i;
				i = 0;
			}
			else
				row++;
			
			i--;
		} 
		else
			strncat(tokens[row], start + i, 1);
		strcpy(tokens[row], ltrim(rtrim(tokens[row])));

		if(!strcmp(tokens[row], "lpthread") && row > 0 && !strcmp(tokens[row - 1], "-"))
		{ 
			strcat(tokens[row - 1], tokens[row]);
			memset(tokens[row], 0, sizeof(tokens[row]));
			row--;
		}
	 	else if(row > 0 && is_character(tokens[row][strlen(tokens[row])-1]) 
				&& (is_typeStatement(tokens[row - 1]) == 2 
					|| is_character(tokens[row-1][strlen(tokens[row - 1]) - 1])
					|| tokens[row - 1][strlen(tokens[row - 1]) - 1] == '.') )
		{
			
			if(row > 1 && strcmp(tokens[row-2],"(") == 0)
			{
				if(strcmp(tokens[row-1], "struct") != 0 && strcmp(tokens[row-1], "unsigned") != 0)
					return FALSE;
			}
			else if(row == 1 && is_character(tokens[row][strlen(tokens[row]) - 1])) {
				if(strcmp(tokens[0], "extern") != 0 && strcmp(tokens[0], "unsigned") != 0 && is_typeStatement(tokens[0]) != 2)	
					return FALSE;
			}
			else if(row > 1 && is_typeStatement(tokens[row - 1]) == 2){
				if(strcmp(tokens[row - 2], "unsigned") != 0 && strcmp(tokens[row - 2], "extern") != 0)
					return FALSE;
			}
		} 
	}
	if(row > 0)
	{
		// ex) #include <sys/types.h>
		if(strcmp(tokens[0], "#include") == 0 || strcmp(tokens[0], "include") == 0 || strcmp(tokens[0], "struct") == 0)
		{ 
			clear_tokens(tokens); 
			strcpy(tokens[0], remove_extraspace(str)); 
		}
	}
	if(is_typeStatement(tokens[0]) == 2 || strstr(tokens[0], "extern") != NULL)
	{
		for(i = 1; i < TOKEN_CNT; i++)
		{
			if(strcmp(tokens[i],"") == 0)  
				break;		       

			if(i != TOKEN_CNT -1 )
				strcat(tokens[0], " ");
			strcat(tokens[0], tokens[i]);
			memset(tokens[i], 0, sizeof(tokens[i]));
		}
	}
	
	//change ( ' char ' )' a  ->  (char)a
	while((p_str = find_typeSpecifier(tokens)) != -1)
	{ 
		if(!reset_tokens(p_str, tokens))
			return FALSE;
	}

	//change sizeof ' ( ' record ' ) '-> sizeof(record)
	while((p_str = find_typeSpecifier2(tokens)) != -1)
	{  
		if(!reset_tokens(p_str, tokens))
			return FALSE;
	}
	return TRUE;
}

// 트리 생성 함수
// char (*tokens)[MINLEN] : 배열 포인터 -> 한 줄에 35개의 인덱스를 갖는 배열을 가리킴
node *make_tree(node *root, char (*tokens)[MINLEN], int *idx, int parentheses)
{
	node *cur = root;
	node *new;
	node *saved_operator;
	node *operator;
	int fstart;
	int i;

	while(1)	
	{
		if(!strcmp(tokens[*idx],"")) // idx번째 문자열이 NULL
			break;
	
		if(!strcmp(tokens[*idx],")")) // idx번째 문자열이 닫는 괄호
			return get_root(cur);

		else if(!strcmp(tokens[*idx],",")) // idx번째 문자열이 쉼표
			return get_root(cur);

		else if(!strcmp(tokens[*idx],"(")) // idx번째 문자열이 여는 괄호일 때
		{
			// 현재 index가 0보다 크면서 이전 index가 연산자,쉼표도 아님
			// 의미 : ? 
			if(*idx > 0 && !is_operator(tokens[*idx - 1]) && strcmp(tokens[*idx - 1],","))
			{
				fstart = TRUE;
				while(1)
				{
					*idx += 1;
					if(!strcmp(tokens[*idx],")")) // 닫는 괄호 발견시 break
						break;
				
					// 루트는 없음(NULL)
					// 괄호 안에 있는 트리 생성
					new = make_tree(NULL, tokens ,idx ,parentheses + 1);
					if(new != NULL)
					{
						if(fstart == TRUE)
						{
							cur->child_head = new;
							new->parent = cur;
							fstart = FALSE;
						}
						else
						{
							cur->next = new;
							new->prev = cur;
						}
						cur = new;
					}
					if(!strcmp(tokens[*idx],")")) // 닫는 괄호 발견시 break
						break;
				}
			}
			else
			{
				*idx += 1;
				new = make_tree(NULL, tokens,idx, parentheses + 1);

				if(cur == NULL)
					cur = new;

				else if(!strcmp(new->name,cur->name))
				{
					if(!strcmp(new->name, "|") || !strcmp(new->name, "||") 
						|| !strcmp(new->name, "&") || !strcmp(new->name, "&&"))
					{
						cur = get_last_child(cur);

						if(new->child_head != NULL)
						{
							new = new->child_head;
							new->parent->child_head = NULL;
							new->parent = NULL;
							new->prev = cur;
							cur->next = new;
						}
					}
					else if(!strcmp(new->name, "+") || !strcmp(new->name, "*"))
					{
						i = 0;

						while(1)
						{
							if(!strcmp(tokens[*idx + i],""))
								break;
							if(is_operator(tokens[*idx + i]) && strcmp(tokens[*idx + i], ")") != 0)
								break;
							i++;
						}
						if(get_precedence(tokens[*idx + i]) < get_precedence(new->name))
						{
							cur = get_last_child(cur);
							cur->next = new;
							new->prev = cur;
							cur = new;
						}
						else
						{
							cur = get_last_child(cur);

							if(new->child_head != NULL)
							{
								new = new->child_head;
								new->parent->child_head = NULL;
								new->parent = NULL;
								new->prev = cur;
								cur->next = new;
							}
						}
					}
					else
					{
						cur = get_last_child(cur);
						cur->next = new;
						new->prev = cur;
						cur = new;
					}
				}
				else
				{
					cur = get_last_child(cur);
					cur->next = new;
					new->prev = cur;
	
					cur = new;
				}
			}
		}
		else if(is_operator(tokens[*idx]))  
		{
			if(!strcmp(tokens[*idx], "||") || !strcmp(tokens[*idx], "&&")
					|| !strcmp(tokens[*idx], "|") || !strcmp(tokens[*idx], "&") 
					|| !strcmp(tokens[*idx], "+") || !strcmp(tokens[*idx], "*"))
			{
				if(is_operator(cur->name) == TRUE && !strcmp(cur->name, tokens[*idx]))
					operator = cur;
		
				else
				{
					new = create_node(tokens[*idx], parentheses);
					operator = get_most_high_precedence_node(cur, new);

					if(operator->parent == NULL && operator->prev == NULL){

						if(get_precedence(operator->name) < get_precedence(new->name)){
							cur = insert_node(operator, new);
						}

						else if(get_precedence(operator->name) > get_precedence(new->name))
						{
							if(operator->child_head != NULL){
								operator = get_last_child(operator);
								cur = insert_node(operator, new);
							}
						}
						else
						{
							operator = cur;
							while(1)
							{
								if(is_operator(operator->name) == TRUE && !strcmp(operator->name, tokens[*idx]))
									break;
						
								if(operator->prev != NULL)
									operator = operator->prev;
								else
									break;
							}

							if(strcmp(operator->name, tokens[*idx]) != 0)
								operator = operator->parent;

							if(operator != NULL){
								if(!strcmp(operator->name, tokens[*idx]))
									cur = operator;
							}
						}
					}
					else
						cur = insert_node(operator, new);
				}

			}
			else
			{
				new = create_node(tokens[*idx], parentheses);
				operator = get_most_high_precedence_node(cur, new);

				if(operator->parentheses > new->parentheses)
					cur = insert_node(operator, new);

				else if(operator->parent == NULL && operator->prev ==  NULL){
					
					if(get_precedence(operator->name) > get_precedence(new->name))
					{
						if(operator->child_head != NULL){
	
							operator = get_last_child(operator);
							cur = insert_node(operator, new);
						}
					}
					
					else	
						cur = insert_node(operator, new);
				}
				else
					cur = insert_node(operator, new);
			}
		}
		else 
		{
			new = create_node(tokens[*idx], parentheses);

			if(cur == NULL)
				cur = new;
			else if(cur->child_head == NULL)
			{
				cur->child_head = new;
				new->parent = cur;
				cur = new;
			}
			else
			{
				cur = get_last_child(cur);
				cur->next = new;
				new->prev = cur;
				cur = new;
			}
		}
		*idx += 1;
	}
	return get_root(cur); // 만들어진 트리의 root를 반환
}

// 자식 교체 함수
node *change_sibling(node *parent)
{
	node *tmp;
	
	tmp = parent->child_head;

	parent->child_head = parent->child_head->next;
	parent->child_head->parent = parent;
	parent->child_head->prev = NULL;

	parent->child_head->next = tmp;
	parent->child_head->next->prev = parent->child_head;
	parent->child_head->next->next = NULL;
	parent->child_head->next->parent = NULL;		

	return parent;
}

node *create_node(char *name, int parentheses)
{
	node *new;

	new = (node *)malloc(sizeof(node));
	new->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(new->name, name);

	new->parentheses = parentheses;
	new->parent = NULL;
	new->child_head = NULL;
	new->prev = NULL;
	new->next = NULL;

	return new;
}

int get_precedence(char *op)
{
	int i;

	for(i = 2; i < OPERATOR_CNT; i++){
		if(!strcmp(operators[i].operator, op))
			return operators[i].precedence;
	}
	return FALSE;
}

// 연산자인가?
int is_operator(char *op)
{
	int i;

	for(i = 0; i < OPERATOR_CNT; i++)
	{
		if(operators[i].operator == NULL) // 예외처리(Seg방지)
			break;
		if(!strcmp(operators[i].operator, op)) // 연산자 일치
			return TRUE;
	}
	return FALSE; // 못 찾음
}

void print(node *cur)
{
	if(cur->child_head != NULL)
	{
		print(cur->child_head);
		printf("\n");
	}

	if(cur->next != NULL)
	{
		print(cur->next);
		printf("\t");
	}
	printf("%s", cur->name);
}

node *get_operator(node *cur)
{
	if(cur == NULL)
		return cur;

	if(cur->prev != NULL)
		while(cur->prev != NULL)
			cur = cur->prev;
	return cur->parent;
}

// 주어진 노드의 root 노드를 찾는 함수(cur = root를 반환함)
node *get_root(node *cur)
{
	if(cur == NULL) // 예외 처리
		return cur;

	while(cur->prev != NULL)
		cur = cur->prev;

	if(cur->parent != NULL) 
		cur = get_root(cur->parent);
	return cur;
}

node *get_high_precedence_node(node *cur, node *new)
{
	if(is_operator(cur->name))
		if(get_precedence(cur->name) < get_precedence(new->name))
			return cur;

	if(cur->prev != NULL)
	{
		while(cur->prev != NULL)
		{
			cur = cur->prev;
			return get_high_precedence_node(cur, new);
		}

		if(cur->parent != NULL)
			return get_high_precedence_node(cur->parent, new);
	}
	if(cur->parent == NULL)
		return cur;
}

node *get_most_high_precedence_node(node *cur, node *new)
{
	node *operator = get_high_precedence_node(cur, new);
	node *saved_operator = operator;

	while(1)
	{
		if(saved_operator->parent == NULL)
			break;

		if(saved_operator->prev != NULL)
			operator = get_high_precedence_node(saved_operator->prev, new);

		else if(saved_operator->parent != NULL)
			operator = get_high_precedence_node(saved_operator->parent, new);

		saved_operator = operator;
	}
	
	return saved_operator;
}

node *insert_node(node *old, node *new)
{
	if(old->prev != NULL){
		new->prev = old->prev;
		old->prev->next = new;
		old->prev = NULL;
	}

	new->child_head = old;
	old->parent = new;

	return new;
}

node *get_last_child(node *cur)
{
	if(cur->child_head != NULL)
		cur = cur->child_head;

	while(cur->next != NULL)
		cur = cur->next;

	return cur;
}

int get_sibling_cnt(node *cur)
{
	int i = 0;

	while(cur->prev != NULL)
		cur = cur->prev;

	while(cur->next != NULL){
		cur = cur->next;
		i++;
	}

	return i;
}

void free_node(node *cur)
{
	if(cur->child_head != NULL)
		free_node(cur->child_head);

	if(cur->next != NULL)
		free_node(cur->next);

	if(cur != NULL){
		cur->prev = NULL;
		cur->next = NULL;
		cur->parent = NULL;
		cur->child_head = NULL;
		free(cur);
	}
}

// 숫자? 알파벳?
int is_character(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// 자료형 선언 판독 함수
// 변수 선언이 맞다면 returns 2
// 단순 자료형이 포함된 문자열이라면 returns 0
// 아무것도 아니면 returns 1
int is_typeStatement(char *str)
{ 
	char *start;
	char str2[EXTENDED_STRLEN] = {0}; 
	char tmp[EXTENDED_STRLEN] = {0}; 
	char tmp2[EXTENDED_STRLEN] = {0}; 
	int i;	 
	
	start = str;
	strncpy(str2,str,strlen(str)); // str2에 str 복사
	remove_space(str2); // 공백 제거

	while(start[0] == ' ') // 공백문자 무시
		start += 1;

	if(strstr(str2, "gcc") != NULL) // gcc가 포함되어 있다면
	{
		strncpy(tmp2, start, strlen("gcc"));
		if(strcmp(tmp2,"gcc")) // 
			return 0;
		else 
			return 2;
	}
	
	for(i = 0; i < DATATYPE_SIZE; i++)
	{
		if(strstr(str2,datatype[i]) != NULL) // 특정data type 이 존재한다면
		{	
			strncpy(tmp, str2, strlen(datatype[i])); 
			strncpy(tmp2, start, strlen(datatype[i])); 
			
			if(!strcmp(tmp, datatype[i])) // str이 특정 자료형으로 시작한다면
				if(strcmp(tmp, tmp2)) // 
				{
					printf("[TEST] return 0!\n");
					printf("[TEST] str : %s\n", str);
					return 0;  
				}
				else // 자료형으로 시작하는 문장임
					return 2;
		}
	}
	return 1; // casting같은 곳에서 사용됨
}

int find_typeSpecifier(char tokens[TOKEN_CNT][MINLEN]) 
{
	int i, j;

	for(i = 0; i < TOKEN_CNT; i++)
	{
		for(j = 0; j < DATATYPE_SIZE; j++)
		{
			if(strstr(tokens[i], datatype[j]) != NULL && i > 0)
			{
				if(!strcmp(tokens[i - 1], "(") && !strcmp(tokens[i + 1], ")") 
						&& (tokens[i + 2][0] == '&' || tokens[i + 2][0] == '*' ||tokens[i + 2][0] == ')' || tokens[i + 2][0] == '(' || tokens[i + 2][0] == '-' || tokens[i + 2][0] == '+' || is_character(tokens[i + 2][0])))  
					return i;
			}
		}
	}
	return -1;
}

int find_typeSpecifier2(char tokens[TOKEN_CNT][MINLEN]) 
{
    int i, j;

   
    for(i = 0; i < TOKEN_CNT; i++)
    {
        for(j = 0; j < DATATYPE_SIZE; j++)
        {
            if(!strcmp(tokens[i], "struct") && (i+1)<=TOKEN_CNT && is_character(tokens[i+1][strlen(tokens[i+1]) - 1]))  
                    return i;
        }
    }
    return -1;
}

// str이 '*'를 포함하고 있다면 1 return, 아니면 0 return
int all_star(char *str)
{
	int i;
	int length= strlen(str);
	
 	if(length == 0)	// exception handling
		return 0;
	
	for(i = 0; i < length; i++)
		if(str[i] != '*')
			return 0; // if str has no stars, return 0
	return 1; // if str has stars, returns 1

}

// str이 전부 문자로 되어있다면 1 return, 아니면 0 return
int all_character(char *str)
{
	int i;

	for(i = 0; i < strlen(str); i++)
		if(is_character(str[i]))
			return 1;
	return 0;
	
}

int reset_tokens(int start, char tokens[TOKEN_CNT][MINLEN]) 
{
	int i;
	int j = start - 1;
	int lcount = 0, rcount = 0;
	int sub_lcount = 0, sub_rcount = 0;

	if(start > -1){
		if(!strcmp(tokens[start], "struct")) {		
			strcat(tokens[start], " ");
			strcat(tokens[start], tokens[start+1]);	     

			for(i = start + 1; i < TOKEN_CNT - 1; i++){
				strcpy(tokens[i], tokens[i + 1]);
				memset(tokens[i + 1], 0, sizeof(tokens[0]));
			}
		}

		else if(!strcmp(tokens[start], "unsigned") && strcmp(tokens[start+1], ")") != 0) {		
			strcat(tokens[start], " ");
			strcat(tokens[start], tokens[start + 1]);	     
			strcat(tokens[start], tokens[start + 2]);

			for(i = start + 1; i < TOKEN_CNT - 1; i++){
				strcpy(tokens[i], tokens[i + 1]);
				memset(tokens[i + 1], 0, sizeof(tokens[0]));
			}
		}

     		j = start + 1;           
        	while(!strcmp(tokens[j], ")")){
                	rcount ++;
                	if(j==TOKEN_CNT)
                        	break;
                	j++;
        	}
	
		j = start - 1;
		while(!strcmp(tokens[j], "(")){
        	        lcount ++;
                	if(j == 0)
                        	break;
               		j--;
		}
		if( (j!=0 && is_character(tokens[j][strlen(tokens[j])-1]) ) || j==0)
			lcount = rcount;

		if(lcount != rcount )
			return FALSE;

		if( (start - lcount) >0 && !strcmp(tokens[start - lcount - 1], "sizeof")){
			return TRUE; 
		}
		
		else if((!strcmp(tokens[start], "unsigned") || !strcmp(tokens[start], "struct")) && strcmp(tokens[start+1], ")")) {		
			strcat(tokens[start - lcount], tokens[start]);
			strcat(tokens[start - lcount], tokens[start + 1]);
			strcpy(tokens[start - lcount + 1], tokens[start + rcount]);
		 
			for(int i = start - lcount + 1; i < TOKEN_CNT - lcount -rcount; i++) {
				strcpy(tokens[i], tokens[i + lcount + rcount]);
				memset(tokens[i + lcount + rcount], 0, sizeof(tokens[0]));
			}


		}
 		else{
			if(tokens[start + 2][0] == '('){
				j = start + 2;
				while(!strcmp(tokens[j], "(")){
					sub_lcount++;
					j++;
				} 	
				if(!strcmp(tokens[j+1],")")){
					j = j + 1;
					while(!strcmp(tokens[j], ")")){
						sub_rcount++;
						j++;
					}
				}
				else 
					return FALSE;

				if(sub_lcount != sub_rcount)
					return FALSE;
				
				strcpy(tokens[start + 2], tokens[start + 2 + sub_lcount]);	
				for(int i = start + 3; i<TOKEN_CNT; i++)
					memset(tokens[i], 0, sizeof(tokens[0]));

			}
			strcat(tokens[start - lcount], tokens[start]);
			strcat(tokens[start - lcount], tokens[start + 1]);
			strcat(tokens[start - lcount], tokens[start + rcount + 1]);
		 
			for(int i = start - lcount + 1; i < TOKEN_CNT - lcount -rcount -1; i++) {
				strcpy(tokens[i], tokens[i + lcount + rcount +1]);
				memset(tokens[i + lcount + rcount + 1], 0, sizeof(tokens[0]));

			}
		}
	}
	return TRUE;
}

void clear_tokens(char tokens[TOKEN_CNT][MINLEN])
{
	int i;

	for(i = 0; i < TOKEN_CNT; i++)
		memset(tokens[i], 0, sizeof(tokens[i]));
}

char *rtrim(char *_str)
{
	char tmp[EXTENDED_STRLEN];
	char *end;

	strcpy(tmp, _str);
	end = tmp + strlen(tmp) - 1;
	while(end != _str && isspace(*end)) // 우측 공백문자 거르기
		--end;

	*(end + 1) = '\0';
	_str = tmp;
	return _str;
}

char *ltrim(char *_str)
{
	char *start = _str;

	while(*start != '\0' && isspace(*start)) // 좌측 공백문자 거르기
		++start;
	_str = start;
	return _str;
}

char* remove_extraspace(char *str)
{
	int i;
	char *str2 = (char*)malloc(sizeof(char) * EXTENDED_STRLEN);
	
	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] ==' ')
		{
			if(i == 0 && str[0] ==' ')
				while(str[i + 1] == ' ')
					i++;	
			else{
				if(i > 0 && str[i - 1] != ' ')
					str2[strlen(str2)] = str[i];
				while(str[i + 1] == ' ')
					i++;
			} 
		}
		else
			str2[strlen(str2)] = str[i];
	}

	return str2;
}

int check_brackets(char *str)
{
	char *start = str;
	int lcount = 0, rcount = 0;
	
	while(1)
	{
		if((start = strpbrk(start, "()")) != NULL)
		{
			if(*(start) == '(')
				lcount++;
			else
				rcount++;

			start += 1; 		
		}
		else
			break;
	}
	if(lcount != rcount) // 괄호 개수가 맞지않다면 return 0
		return 0; 
	else  // 괄호 개수가 맞다면 return 1
		return 1;
}

int get_token_cnt(char tokens[TOKEN_CNT][MINLEN])
{
	int i;
	
	for(i = 0; i < TOKEN_CNT; i++)
		if(!strcmp(tokens[i], ""))
			break;
	return i;
}
