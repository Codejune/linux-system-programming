#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "blank.h"

// 자료형의 종류 구분, DATATYPE_SIZE:35, MINLEN:64
char datatype[DATATYPE_SIZE][MINLEN] = {"int", "char", "double", "float", "long"
			, "short", "ushort", "FILE", "DIR","pid"
			,"key_t", "ssize_t", "mode_t", "ino_t", "dev_t"
			, "nlink_t", "uid_t", "gid_t", "time_t", "blksize_t"
			, "blkcnt_t", "pid_t", "pthread_mutex_t", "pthread_cond_t", "pthread_t"
			, "void", "size_t", "unsigned", "sigset_t", "sigjmp_buf"
			, "rlim_t", "jmp_buf", "sig_atomic_t", "clock_t", "struct"};

// 연산자 우선순위, OPERATOR_CNT:24
operator_precedence operators[OPERATOR_CNT] = {
	{"(", 0}, {")", 0}
	,{"->", 1}	
	,{"*", 4}	,{"/", 3}	,{"%", 2}	
	,{"+", 6}	,{"-", 5}	
	,{"<", 7}	,{"<=", 7}	,{">", 7}	,{">=", 7}
	,{"==", 8}	,{"!=", 8}
	,{"&", 9}
	,{"^", 10}
	,{"|", 11}
	,{"&&", 12}
	,{"||", 13}
	,{"=", 14}	,{"+=", 14}	,{"-=", 14}	,{"&=", 14}	,{"|=", 14}
};

void compare_tree(node *root1,  node *root2, int *result) // std_root, ans_root 파스트리 비교, 같음:1, 다름:0
{
	node *tmp;
	int cnt1, cnt2;

	if(root1 == NULL || root2 == NULL){ // 비교할 트리 중 하나라도 트리가 존재하지 않을 경우
		*result = false; 
		return;
	}

	if(!strcmp(root1->name, "<") || !strcmp(root1->name, ">") || !strcmp(root1->name, "<=") || !strcmp(root1->name, ">=")) {  
		if(strcmp(root1->name, root2->name) != 0){ // 학생 문자와 정답 문자가 같지 않을 경우

			if(!strncmp(root2->name, "<", 1)) // 정답 문자가 '<' 
				strncpy(root2->name, ">", 1); // 정답 문자에 '>' 복사

			else if(!strncmp(root2->name, ">", 1)) // 정답 문자가 '>'
				strncpy(root2->name, "<", 1); // 정답 문자에 '<' 복사

			else if(!strncmp(root2->name, "<=", 2)) // 정답 문자가 "<="
				strncpy(root2->name, ">=", 2); // 정답 문자에 ">=" 복사

			else if(!strncmp(root2->name, ">=", 2)) // 정답 문자가 ">="
				strncpy(root2->name, "<=", 2); // 정답 문자에 "<=" 복사

			root2 = change_sibling(root2); // 다음 형제 노드 이동
		}
	}

	if(strcmp(root1->name, root2->name) != 0){ // 학생 문자와 정답 문자가 같지 않을 경우
		*result = false; 
		return;
	}

	if((root1->child_head != NULL && root2->child_head == NULL) 
		|| (root1->child_head == NULL && root2->child_head != NULL)){ // 비교할 자식 노드가 하나라도 존재하지 않을 경우
		*result = false;
		return;
	}

	else if(root1->child_head != NULL){ // 학생 답안의 자식이 존재 할 경우
		if(get_sibling_cnt(root1->child_head) != get_sibling_cnt(root2->child_head)){ // 형제 개수가 같지 않으면
			*result = false;
			return;
		}

		if(!strcmp(root1->name, "==") || !strcmp(root1->name, "!=")) 
		{
			compare_tree(root1->child_head, root2->child_head, result); // 학생 답안과 정답 답안 트리 비교(재귀)

			if(*result == false) // 여러개의 답의 경우, 정답 답안 형제 트리 순환 비교
			{
				*result = true;
				root2 = change_sibling(root2);
				compare_tree(root1->child_head, root2->child_head, result);
			}
		}
		else if(!strcmp(root1->name, "+") || !strcmp(root1->name, "*")
				|| !strcmp(root1->name, "|") || !strcmp(root1->name, "&")
				|| !strcmp(root1->name, "||") || !strcmp(root1->name, "&&")) 
		{
			if(get_sibling_cnt(root1->child_head) != get_sibling_cnt(root2->child_head)){ // 형제 개수가 같지 않으면
				*result = false;
				return;
			}

			tmp = root2->child_head;

			while(tmp->prev != NULL) // 정답의 이전 형제가 존재할 경우
				tmp = tmp->prev; 

			while(tmp != NULL) // 여러개의 정답 트리 중 처음부터 학생 트리와 순회 비교
			{
				compare_tree(root1->child_head, tmp, result);
			
				if(*result == true)
					break;
				else{
					if(tmp->next != NULL)
						*result = true;
					tmp = tmp->next;
				}
			}
		}
		else compare_tree(root1->child_head, root2->child_head, result);
	}	


	if(root1->next != NULL){ // 학생 답안 트리의 형제가 존재하지 않을 경우

		if(get_sibling_cnt(root1) != get_sibling_cnt(root2)){ // 형제의 개수가 같지 않을 경우
			*result = false;
			return;
		}

		if(*result == true) // 답안이 재귀로 다시 들어오면
		{
			tmp = get_operator(root1); // 학생 답안의 노드 부모 참조
	
			if(!strcmp(tmp->name, "+") || !strcmp(tmp->name, "*") // 부모노드가 연산자일 경우
					|| !strcmp(tmp->name, "|") || !strcmp(tmp->name, "&")
					|| !strcmp(tmp->name, "||") || !strcmp(tmp->name, "&&"))
			{	
				tmp = root2;
	
				while(tmp->prev != NULL) // 형제노드가 존재할 경우
					tmp = tmp->prev;

				while(tmp != NULL) // 형제노드가 null이 아닐경우
				{
					compare_tree(root1->next, tmp, result); // 형제들과 비교

					if(*result == true) 
						break;
					else{
						if(tmp->next != NULL)
							*result = true;
						tmp = tmp->next;
					}
				}
			}

			else
				compare_tree(root1->next, root2->next, result);
		}
	}
}

int make_tokens(char *str, char tokens[TOKEN_CNT][MINLEN]) // 주어진 문자열의 토큰화, TOKEN_CNT:50, MINLEN:64
{
	char *start, *end;
	char tmp[BUFLEN];
	char str2[BUFLEN];
	char *op = "(),;><=!|&^/+-*\""; 
	int row = 0;
	int i;
 	int isPointer;
	int lcount, rcount;
	int p_str;
	
	clear_tokens(tokens); // 토큰 초기화

	start = str; // 정답 문자열 시작 포인터 지정
	
	/*
	if(is_typeStatement(str) == 0) // 잘못된 작성일 경우, gcc 체크
		return false;	
	*/

	while(1)
	{
		// strpbrk(): 두번째 인자 문자열의 문자들 중 하나라도 첫번째 문자열에 존재한다면 해당 문자가 위치한 두번째의 주소 리턴
		if((end = strpbrk(start, op)) == NULL) // start에서 op의 문자들 중 하나라도 없다면 break
			break;

		if(start == end){ // 시작과 끝의 문자가 같다면
 
			if(!strncmp(start, "--", 2) || !strncmp(start, "++", 2)) { // 앞에 두글자가 전위 증감연산자일 경우
				if(!strncmp(start, "++++", 4)||!strncmp(start,"----",4)) // 잘못된 증감연산자 사용
					return false;

				if(is_character(*ltrim(start + 2))){ // 왼쪽 공백 제거 후, 전위 증감연산일 때 문자가 왔다면
					if(row > 0 && is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])) // ex. abc-- abc;와 같은 경우
						return false; 

					end = strpbrk(start + 2, op); // 다음 연산자까지 토큰 탐색
					if(end == NULL) // 토큰의 끝을 탐색하지 못했을 경우
						end = &str[strlen(str)];
					while(start < end) { //  현재 연산자부터 다음 연산자까지
						if(*(start - 1) == ' ' && is_character(tokens[row][strlen(tokens[row]) - 1])) // start 전 문자가 공백이고 현재 토큰의 마지막 인자가 문자일 경우
							return false;
						else if(*start != ' ') // 공백이 아니면 토큰에 붙임
							strncat(tokens[row], start, 1); // 현재 토큰에 1바이트를 붙임
						start++;	
					}
				}
				
				else if(row>0 && is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])){ // 앞 토큰의 마지막 인자가 문자일 경우
					if(strstr(tokens[row - 1], "++") != NULL || strstr(tokens[row - 1], "--") != NULL) // 앞 토큰이 전위 연산자일 경우
						return false;

					memset(tmp, 0, sizeof(tmp));
					strncpy(tmp, start, 2); // tmp에 start내용 2바이트 복사
					strcat(tokens[row - 1], tmp); // tmp에 앞 토큰 내용 복사
					start += 2; // start의 인덱스를 2바이트 이동
					row--; //현재 토큰 개수 감소
				}
				else{ // 그 외의 경우
					memset(tmp, 0, sizeof(tmp)); // 토큰에 2바이트만 저장
					strncpy(tmp, start, 2);
					strcat(tokens[row], tmp);
					start += 2;
				}
			}

			else if(!strncmp(start, "==", 2) || !strncmp(start, "!=", 2) || !strncmp(start, "<=", 2) 
				|| !strncmp(start, ">=", 2) || !strncmp(start, "||", 2) || !strncmp(start, "&&", 2) 
				|| !strncmp(start, "&=", 2) || !strncmp(start, "^=", 2) || !strncmp(start, "!=", 2) 
				|| !strncmp(start, "|=", 2) || !strncmp(start, "+=", 2)	|| !strncmp(start, "-=", 2) 
				|| !strncmp(start, "*=", 2) || !strncmp(start, "/=", 2)){ // 비교 연산자일 경우

				strncpy(tokens[row], start, 2); // 연산자 문자크기(2바이트) 만큼 저장
				start += 2; // 2바이트 이동
			}
			else if(!strncmp(start, "->", 2)) // 구조체 포인터 변수 접근자일 경우
			{
				end = strpbrk(start + 2, op); // 다음 연산자가 나올때 까지 탐색

				if(end == NULL) // 연산자가 나오지 않았을 경우
					end = &str[strlen(str)]; // str의 마지막을 끝으로 지정

				while(start < end){ // 다음 연산자까지 탐색
					if(*start != ' ') // 공백이 아니면
						strncat(tokens[row - 1], start, 1); // 앞 토큰에 이어붙임
					start++;
				}
				row--; // 현재 토큰 개수 감소
			}
			else if(*end == '&') // 주소 포인터 접근자일 경우
			{
				
				if(row == 0 || (strpbrk(tokens[row - 1], op) != NULL)){ // 첫 토큰이거나 앞 토큰에 연산자가 없을 경우
					end = strpbrk(start + 1, op); // 다음 연산자 탐색
					if(end == NULL) // 다음 연산자가 없으면
						end = &str[strlen(str)]; // str의 마지막을 end로 지정
					
					strncat(tokens[row], start, 1); // 현재 토큰에 start의 1바이트('&')를 붙임
					start++; // 다음 문자이동

					while(start < end){ // 다음 연산자가 나올때 까지 탐색
						if(*(start - 1) == ' ' && tokens[row][strlen(tokens[row]) - 1] != '&') // start 1바이트 전이 공백이고 현재 토큰의 마지막 문자가 '&'일 경우
							return false;
						else if(*start != ' ') // 공백이 아닐 경우
							strncat(tokens[row], start, 1); // 1바이트를 현재 토큰에 이어 붙임
						start++; // 다음 문자이동
					}
				}
				
				else{
					strncpy(tokens[row], start, 1); // 1바이트 이어붙이기
					start += 1;
				}
				
			}
		  	else if(*end == '*') // '*' 이면
			{
				isPointer=0; // isPointer 초기화, '*'이 무조건 포인터가 아닐 수도 있으므로

				if(row > 0) // 토큰이 한개 이상이면
				{
					
					for(i = 0; i < DATATYPE_SIZE; i++) { // 데이어타입 개수만큼
						if(strstr(tokens[row - 1], datatype[i]) != NULL){ // 앞 토큰에 데이터타입이 있으면
							strcat(tokens[row - 1], "*"); // 앞 토큰에 '*'삽입 (데이터 포인터)
							start += 1; // 다음 문자 이동	
							isPointer = 1; // 포인터로 체크
							break;
						}
					}
					if(isPointer == 1) // 포인터일 경우
						continue;
					if(*(start+1) !=0) // 다음 인자가 널문자가 아니면
						end = start + 1; // end를 한칸 이동

					// 더블 포인터?
					if(row>1 && !strcmp(tokens[row - 2], "*") && (all_star(tokens[row - 1]) == 1)){ // 토큰이 2개 이상이고 2개 앞 토큰이 '*'이며, 앞 토큰이 모두 '*'이면
						strncat(tokens[row - 1], start, end - start); // 앞 토큰에
						row--; // 토큰 개수 감소
					}
					
					
					else if(is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1]) == 1){  // 앞 토큰의 마지막 인자가 문자면
						strncat(tokens[row], start, end - start); // 현재 토큰에 다음 연산자 전까지 모두 입력
					}

					
					else if(strpbrk(tokens[row - 1], op) != NULL){ // 앞 토큰에 연산자가 있으면
						strncat(tokens[row] , start, end - start); // 현재 토큰에 다음 연산자 전까지 모두 입력
							
					}
					else // 그외의 경우
						strncat(tokens[row], start, end - start); // 현재 토큰에 다음 연산자 전까지 모두 입력

					start += (end - start); // 다음 연산자까지 문자 이동
				}

			 	else if(row == 0) // 첫 토큰일 경우
				{
					if((end = strpbrk(start + 1, op)) == NULL){ // 다음 연산자가 없으면
						strncat(tokens[row], start, 1); // 토큰에 1바이트('*')을 붙이고
						start += 1; // 다음 문자 이동
					}
					else{ // 연산자가 있으면
						while(start < end){ // 다음 연산자까지
							if(*(start - 1) == ' ' && is_character(tokens[row][strlen(tokens[row]) - 1])) // 이전 문자가 공백이고 현재 토큰의 마지막 인자가 문자일 경우
								return false;
							else if(*start != ' ') // 공백일 경우
								strncat(tokens[row], start, 1); // 현재 토큰에 내용 이어붙임 
							start++; // 다음 문자 이동	
						}
						if(all_star(tokens[row])) // 현재 토큰이 모두 '*'문자로 이루어져 있을 경우 토큰 개수 감소
							row--;
						
					}
				}
			}
			else if(*end == '(') // 여는 괄호의 경우
			{
				lcount = 0;
				rcount = 0;
				if(row>0 && (strcmp(tokens[row - 1],"&") == 0 || strcmp(tokens[row - 1], "*") == 0)){ // 토큰이 1개 이상이고, 앞 토큰이 '&' | '*' 일 경우 
					while(*(end + lcount + 1) == '(') // 괄호 내부 수식에 여는 괄호가 또 있을 경우
						lcount++; 
					start += lcount; // 가장 안쪽 여는 괄호로 시작이동

					end = strpbrk(start + 1, ")"); // 닫는 괄호가 나오기 전까지 탐색

					if(end == NULL) // 닫는 괄호가 없는 경우
						return false;
					else{
						while(*(end + rcount +1) == ')') // 닫는 괄호 개수 체크
							rcount++;
						end += rcount;

						if(lcount != rcount) // 괄호의 짝이 안맞을 경우
							return false;

						if( (row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1])) || row == 1){  // 토큰이 2개 이상이고, 2개 앞 토큰의 마지막 인자가 문자가 아니거나 토큰이 1개일 경우
							strncat(tokens[row - 1], start + 1, end - start - rcount - 1); // 앞 토큰에 닫는 괄호 전까지 모두 이어붙임
							row--; // 토큰 개수 감소
							start = end + 1; //가장 바깥쪽 닫는 괄호 다음으로 이동
						}
						else{ // 현재 토큰에 1바이트 이어붙임
							strncat(tokens[row], start, 1);
							start += 1;
						}
					}
						
				}
				else{ // 그 외엔 '(' 붙임
					strncat(tokens[row], start, 1);
					start += 1;
				}

			}
			else if(*end == '\"') // 따옴표면
			{
				end = strpbrk(start + 1, "\""); // 다음 따옴표의 위치를 탐색
				
				if(end == NULL) // 다음 따옴표가 없을 경우
					return false;

				else{ // 현재 토큰에 따옴표 사이의 내용을 붙임
					strncat(tokens[row], start, end - start + 1);
					start = end + 1;
				}

			}

			else{ // 그 외의 경우면
				
				if(row > 0 && !strcmp(tokens[row - 1], "++")) // 토큰이 1개 이상이고 앞 토큰이 ++일 경우
					return false;

				
				if(row > 0 && !strcmp(tokens[row - 1], "--")) // 토큰이 1개 이상이고 앞 토큰이 --일 경우
					return false;
	
				strncat(tokens[row], start, 1); // 현재 토큰에 1바이트를 붙임
				start += 1; // 문자 이동
				
			
				if(!strcmp(tokens[row], "-") || !strcmp(tokens[row], "+") || !strcmp(tokens[row], "--") || !strcmp(tokens[row], "++")){ // 현재 토큰이 증감 연산자일 경우

					if(row == 0) // 첫 토큰이면 row 감소
						row--; 

					
					else if(!is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])){ // 앞 토큰의 마지막 인자가 문자가 아닐 경우
					
						if(strstr(tokens[row - 1], "++") == NULL && strstr(tokens[row - 1], "--") == NULL) // 앞 토큰에 전위 증감 연산자가 없으면 row 감소
							row--;
					}
				}
			}
		}
		else{ // start와 end가 다르면(단어의 경우)
			if(all_star(tokens[row - 1]) && row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1])) //포인터인지 검사(row > 2)
				row--;				

			if(all_star(tokens[row - 1]) && row == 1) // 포인터인지 검사(row = 1)
				row--;	

			for(i = 0; i < end - start; i++){ // 다음 토큰 문자가 오기 전까지 탐색
				if(i > 0 && *(start + i) == '.'){ // 구조체 접근이면
					strncat(tokens[row], start + i, 1); // '.'을 바로 붙여줌

					while( *(start + i +1) == ' ' && i< end - start ) 
						i++; 
				}
				else if(start[i] == ' '){ // 공백일 경우 생략
					while(start[i] == ' ')
						i++;
					break;
				}
				else // 그 외면 토큰에 붙임
					strncat(tokens[row], start + i, 1);
			}

			if(start[0] == ' '){ // 공백일 경우 생략
				start += i;
				continue;
			}
			start += i; // 다음 토큰 시작 문자까지 인덱스 증가
		}
			
		strcpy(tokens[row], ltrim(rtrim(tokens[row]))); // 좌우 공백 제거 후 다시 저장

		// 토큰이 1개 이상이고 현재 토큰의 마지막이 문자이고 앞 토큰이 데이터 타입이거나(변수 선언)
		// 앞 토큰의 마지막 인자가 문자이거나 앞 토큰의 마지막 문자가 '.'이면
		 if(row > 0 && is_character(tokens[row][strlen(tokens[row]) - 1]) 
				&& (is_typeStatement(tokens[row - 1]) == 2 
					|| is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])
					|| tokens[row - 1][strlen(tokens[row - 1]) - 1] == '.' ) ){

			if(row > 1 && strcmp(tokens[row - 2],"(") == 0) // 토큰이 2개 이상 있고 2개 앞  토큰이 여는 괄호일 때
			{
				if(strcmp(tokens[row - 1], "struct") != 0 && strcmp(tokens[row - 1],"unsigned") != 0) // 앞 토큰이 struct와 unsigned가 아닐 경우
					return false; 
			}
			else if(row == 1 && is_character(tokens[row][strlen(tokens[row]) - 1])) { // 토큰이 한개이고, 현재 토큰의 마지막 인자가 문자일 경우
				if(strcmp(tokens[0], "extern") != 0 && strcmp(tokens[0], "unsigned") != 0 && is_typeStatement(tokens[0]) != 2) // 첫 토큰이 extern이 아니고, unsigned가 아니고, 데이터터타입이 아닐 경우
					return false;
			} 
			else if(row > 1 && is_typeStatement(tokens[row - 1]) == 2){ // 토큰이 2개 이상 있고, 앞의 토큰이 데이터타입일 경우
				if(strcmp(tokens[row - 2], "unsigned") != 0 && strcmp(tokens[row - 2], "extern") != 0)
					return false;
			}
			
		}

		if((row == 0 && !strcmp(tokens[row], "gcc")) ){ // 첫 토큰이 gcc일 경우
			clear_tokens(tokens); // 토큰을 비우고
			strcpy(tokens[0], str);	// 첫 토큰에 문장 전체를 넣어주고 종료
			return 1;
		} 

		row++;
	} // 반복문 종료

	if(all_star(tokens[row - 1]) && row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1])) // 앞 토큰이 별이고 2개 앞 토큰 마지막이 문자가 아닐 경우
		row--;				
	if(all_star(tokens[row - 1]) && row == 1) // 앞 토큰이 '*'이면
		row--;	

	for(i = 0; i < strlen(start); i++) // start에 남은 문자 처리
	{
		if(start[i] == ' ')  // 공백일 경우
		{
			while(start[i] == ' ') // 공백이 아닐때까지 i 증가
				i++;
			if(start[0]==' ') { // 첫 인자가 공백이면
				start += i; // start의 시작 위치 i까지 옮기고
				i = 0; // i 초기화
			}
			else
				row++; // 토큰 개수 증가
			
			i--;
		} 
		else
		{
			strncat(tokens[row], start + i, 1); // 현재 토큰에 1바이트를 붙임
			if( start[i] == '.' && i<strlen(start)){ // 만약 '.'이고 위치가 start를 넘지 않았으면
				while(start[i + 1] == ' ' && i < strlen(start)) // 다음 문자가 공백이 아닐때까지 i증가
					i++;

			}
		}
		strcpy(tokens[row], ltrim(rtrim(tokens[row]))); // 앞뒤 공백 제거 후 토큰에 추가

		if(!strcmp(tokens[row], "lpthread") && row > 0 && !strcmp(tokens[row - 1], "-")){  // -lpthread 옵션이 있으면
			strcat(tokens[row - 1], tokens[row]); // 두개를 붙여서 한 토큰에 넣고
			memset(tokens[row], 0, sizeof(tokens[row]));
			row--; // 토큰 개수 감소
		}
	 	else if(row > 0 && is_character(tokens[row][strlen(tokens[row]) - 1]) 
				&& (is_typeStatement(tokens[row - 1]) == 2 
					|| is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])
					|| tokens[row - 1][strlen(tokens[row - 1]) - 1] == '.') ){ // 토큰이 1개 이상이고 현재 토큰의 마지막 인자가 문자이고 데이터 타입이거나 앞 토큰의 마지막 인자가 문자이거나 '.'
			
			if(row > 1 && strcmp(tokens[row-2],"(") == 0) // 토큰이 2개이상이고 2개 앞 토큰이 여는 괄호일 경우
			{
				if(strcmp(tokens[row-1], "struct") != 0 && strcmp(tokens[row-1], "unsigned") != 0) // 앞 토큰이 struct가 아니거나 unsigned가 아닐 경우
					return false;
			}
			else if(row == 1 && is_character(tokens[row][strlen(tokens[row]) - 1])) { // 토큰이 2개이고 현재 토큰의 마지막 인자가 문자일 경우
				if(strcmp(tokens[0], "extern") != 0 && strcmp(tokens[0], "unsigned") != 0 && is_typeStatement(tokens[0]) != 2) // 0번째 토큰이 extern이 아니거나 unsigned가 아니거나 데이터타입이 아닐 경우
					return false;
			}
			else if(row > 1 && is_typeStatement(tokens[row - 1]) == 2){ // 토큰이 2개 이상이고 앞 토큰이 데이터타입일 경우
				if(strcmp(tokens[row - 2], "unsigned") != 0 && strcmp(tokens[row - 2], "extern") != 0) // 두개 앞 토큰이 unsigned가 아니고 extern도 아닐 경우
					return false;
			}
		} 
	}


	if(row > 0) // 토큰이 1개 이상일 경우
	{

		
		if(strcmp(tokens[0], "#include") == 0 || strcmp(tokens[0], "include") == 0 || strcmp(tokens[0], "struct") == 0){  // 첫번째 토큰이 #include이거나 include이거나 struct일 경우 
			clear_tokens(tokens); // 토큰을 정리하고
			strcpy(tokens[0], remove_extraspace(str)); // 과도한 공백 제거
		}
	}

	if(is_typeStatement(tokens[0]) == 2 || strstr(tokens[0], "extern") != NULL){ // 첫 토큰이 데이터타입이거나 extern일 경ㅇ
		for(i = 1; i < TOKEN_CNT; i++){ // 토큰 개수만큼 순회
			if(strcmp(tokens[i],"") == 0)  // 토큰내에 정보가 없을 경우
				break;		       

			if(i != TOKEN_CNT -1 ) // 토큰의 마지막이 아닐경우
				strcat(tokens[0], " "); // 첫 토큰에 공백을 붙임
			strcat(tokens[0], tokens[i]); // 첫 토큰에 i번째 토큰을 붙이고
			memset(tokens[i], 0, sizeof(tokens[i])); // i본째 토큰 메모리 초기화
		}
	}
	
	
	while((p_str = find_typeSpecifier(tokens)) != -1){ 
		if(!reset_tokens(p_str, tokens))
			return false;
	}


	while((p_str = find_typeSpecifier2(tokens)) != -1){  
		if(!reset_tokens(p_str, tokens))
			return false;
	}
	
	return true;
}

// char(*tokens)[MINLEN] : 배열 포인터 -> 한줄에 35개의 인덱스를 갖는 배열을 가리킴
node *make_tree(node *root, char (*tokens)[MINLEN], int *idx, int parentheses) // 주어진 노드의 파스트리 생성, 초기 idx:0, parentheses:괄호 짝 개수
{
	node *cur = root; // 노드 커서
	node *new;
	node *saved_operator;
	node *operator;
	int fstart;
	int i;

	while(1) // 토큰 탐색
	{
		if(strcmp(tokens[*idx], "") == 0) // 1. 토큰 배열이 비어있을 경우
			break;
	
		if(!strcmp(tokens[*idx], ")")) // 2. 토큰 배열에서 ')'를 만나면 괄호안의 문장을 마무리, 괄호내용을 포함하고 있는 부모 돌아감
			return get_root(cur);

		else if(!strcmp(tokens[*idx], ",")) // 3. 토큰 배열에서 ","를 만나면 부모로 돌아감
			return get_root(cur);

		else if(!strcmp(tokens[*idx], "(")) // 4. 토큰 배열에서 "("를 만날 경우
		{
			
			if(*idx > 0 && !is_operator(tokens[*idx - 1]) && strcmp(tokens[*idx - 1], ",") != 0){ // a. 앞의 문자가 연산자나 ","가 아닐 경우
				fstart = true; // create^(fd, O_RDONLY) 와 같은 케이스, 

				while(1)
				{
					*idx += 1; // 다음 토큰 이동

					if(!strcmp(tokens[*idx], ")")) // 토큰이 ")"일 경우
						break;
					
					new = make_tree(NULL, tokens, idx, parentheses + 1); // 자식 트리 생성
					
					if(new != NULL){ // 자식 트리가 생성되었을 경우
						if(fstart == true){
							cur->child_head = new; // 현재 노드의 자식 노드 연결
							new->parent = cur; // 자식 노드의 부모를 현재 노드로 연결
	
							fstart = false; 
						}
						else{ // 함수 인자문이 아닐 때, 형제로 인식
							cur->next = new; // 현재 노드의 다음 노드로 연결
							new->prev = cur; // 다음 노드의 이전 노드를 현재 노드로 연결
						}

						cur = new; // 새로 만든 트리 노드로 이동
					}

					if(!strcmp(tokens[*idx], ")")) // 토큰이 ")" 일 경우
						break;
				}
			}
			else{ // b. add(a,^(b+c)) | a +^(a+b) | (fd, O_RDONLY) 와 같은 케이스, 기본 괄호 트리 생성 유형
				*idx += 1; // 다음 토큰 이동
	
				new = make_tree(NULL, tokens, idx, parentheses + 1); // 새로운 자식 트리 생성(재귀)

				if(cur == NULL) // 현재 위치가 최상위 부모 노드일 경우
					cur = new; // 자식 트리로 이동

				else if(!strcmp(new->name, cur->name)){ // 자식 토큰과 현재의 토큰이 같다면 (++, --, &&, ||)
					if(!strcmp(new->name, "|") || !strcmp(new->name, "||")  // 자식의 토큰이 논리 연산자일 경우, (||, , (|, (&&, (&,
						|| !strcmp(new->name, "&") || !strcmp(new->name, "&&"))
					{
						cur = get_last_child(cur); // 자식중 가장 막내 노드로 이동

						if(new->child_head != NULL){ // 자식노드의 자식트리가 존재할 경우
							new = new->child_head; // 자식 노드도 최하위 자식의 마지막 형제로 이동

							new->parent->child_head = NULL; // 자식관계가 아닌 형제관계로 변경
							new->parent = NULL; // 즉 비교 연산자 다음에 오는 토큰을 자식 관계로 생성되었지만 형제관계로 변경
							new->prev = cur;
							cur->next = new;
						}
					}
					else if(!strcmp(new->name, "+") || !strcmp(new->name, "*")) // 자식의 토큰이 수식 연산자 일 경우, + ( +,  * ( *
					{
						i = 0; // 괄호 안 수식 내용 중 연산자 인덱스

						while(1)
						{
							if(!strcmp(tokens[*idx + i], "")) // 토큰이 비었을 경우
								break;

							if(is_operator(tokens[*idx + i]) && strcmp(tokens[*idx + i], ")") != 0) // 해당 토큰이 연산자에 포함되고, ')'가 아닐 경우
								break; 

							i++; 
						}
						
						if(get_precedence(tokens[*idx + i]) < get_precedence(new->name)) // 새로운 연산자와 기존 연산자 우선순위 비교, 새로운 연산자가 더 우선일경우
						{
							cur = get_last_child(cur); // 자식중 가장 막내 노드로 이동 
							cur->next = new; // 막내 노드 갱신(new)
							new->prev = cur; 
							cur = new;
						}
						else // 기존 연산자가 더 우선일 경우
						{
							cur = get_last_child(cur); // 자식중 가장 막내 노드로 이동 

							if(new->child_head != NULL){ 
								new = new->child_head;

								new->parent->child_head = NULL; // 새로운 연산자의 자식노드를 기존 연산자의 다음 형제로 이동
								new->parent = NULL;
								new->prev = cur;
								cur->next = new;
							}
						}
					}
					else{ 
						cur = get_last_child(cur); // 자식 중 가장 막내 노드로 이동 
						cur->next = new; // 막내 노드 갱신(new)
						new->prev = cur;
						cur = new;
					}
				}
	
				else
				{
					cur = get_last_child(cur); // 자식중 가장 막내 노드로 이동

					cur->next = new;
					new->prev = cur;
	
					cur = new;
				}
			}
		}
		else if(is_operator(tokens[*idx])) // 5. 토큰 배열에서 연산자를 만날 경우
		{
			if(!strcmp(tokens[*idx], "||") || !strcmp(tokens[*idx], "&&")
					|| !strcmp(tokens[*idx], "|") || !strcmp(tokens[*idx], "&") 
					|| !strcmp(tokens[*idx], "+") || !strcmp(tokens[*idx], "*"))
			{  // c. 논리 연산자의 경우
				if(is_operator(cur->name) == true && !strcmp(cur->name, tokens[*idx])) // 현재 노드의 토큰이 연산자이고, 현재 노드의 토큰과 다음 토큰이 같은 연산자인 경우
					operator = cur; // 연산자로 현재 노드를 지정
		
				else // 아닐 경우
				{
					new = create_node(tokens[*idx], parentheses); // 새로운 노드 생성
					operator = get_most_high_precedence_node(cur, new); // 기존 최우선 연산자와, 새로운 연산자의 우선순위 비교 후 연산자로 지정

					if(operator->parent == NULL && operator->prev == NULL){ // 우선순위인 연산자 노드가 부모가 없고, 형제도 없으면

						if(get_precedence(operator->name) < get_precedence(new->name)){ // 기존 연산자가 새로운 연산자보다 우선순위가 높을 경우
							cur = insert_node(operator, new); // 새로운 연산자를 기존 우선순위 연산자의 부모로 둠(트리의 잎에 가까울수록 우선순위 높음)
						}

						else if(get_precedence(operator->name) > get_precedence(new->name)) // 새로운 연산자가 기존연산자보다 우선순위가 높을 경우
						{
							if(operator->child_head != NULL){ // 기존 연산자 노드의 자식이 존재하면 새로운 연산자 노드를 자식 연산자 노드들의 막내 자리에 두고, 막내노드가 새로운 연산자노드의 자식이 됨
								operator = get_last_child(operator);   
								cur = insert_node(operator, new); 
							}
						}
						else // 최우선 연산자랑 우선순위가 같거나 지정되어있지 않을 경우
						{
							operator = cur; // 최우선 연산자 지정
	
							while(1)
							{
								if(is_operator(operator->name) == true && !strcmp(operator->name, tokens[*idx])) // 기존 연산자가 새로운 토큰이랑 같을 때
									break;
						
								if(operator->prev != NULL) // 기존 연산자의 형노드가 있을 경우
									operator = operator->prev; // 기존 연산자의 형노드로 이동
								else // 없을 경우
									break;
							}

							if(strcmp(operator->name, tokens[*idx]) != 0) // 기존 연산자와 새로운 토큰이 다를 경우 
								operator = operator->parent; 

							if(operator != NULL){ // 연산자 노드가 지정되어있을 경우
								if(!strcmp(operator->name, tokens[*idx])) // 기존 연산자와 새로운 토큰이 같을 경우
									cur = operator; // 현재 노드는 연산자 노드로 지정
							}
						}
					}

					else
						cur = insert_node(operator, new);
				}

			}
			else // d. 그 외에 연산자(<, >, /, -, <=, >=< ==, !=, ^, = ...)
			{
				new = create_node(tokens[*idx], parentheses); // 해당 토큰 노드 생성

				if(cur == NULL) // 현재 가리키고 있는 노드가 없을 경우
					cur = new; // 노드 생성 후 포인팅

				else // 가리키고 있는 노드가 존재할 경우
				{
					operator = get_most_high_precedence_node(cur, new); // 현재 가리키는 노드와 연산자 우선순위 비교 후 선 순위 노드 할당, 우선순위(연산자 < 단어)

					if(operator->parentheses > new->parentheses) // 기존의 연산자가 더 많은 괄호 안에 존재할 때 (new ((operator)))
						cur = insert_node(operator, new); // 새로운 연산자를 기존의 연산자 앞에 둠

					else if(operator->parent == NULL && operator->prev ==  NULL){ // 우선 연산자가 최상위 노드일 경우(단어 일 경우 ?)
					
						if(get_precedence(operator->name) > get_precedence(new->name)) // 기존연산자(단어) > 새 연산자
						{
							if(operator->child_head != NULL){ // 단어의 자식이 존재할 경우
	
								operator = get_last_child(operator); // 자식의 막내의 형노드로 삽입
								cur = insert_node(operator, new);
							}
						}
					
						else	
							cur = insert_node(operator, new); // 기존 연산자 < 새 연산자, 기존연산자의 부모로 삽입
					}
	
					else
						cur = insert_node(operator, new);
				}
			}
		}
		else  // 6. 일반적인 단어일 경우
		{
			new = create_node(tokens[*idx], parentheses); // 노드 생성 

			if(cur == NULL) // 루트노드 시작이면 생성한 노드를 시작 노드로 지정
				cur = new;

			else if(cur->child_head == NULL){ // 자식이 없다면
				cur->child_head = new; // 자식 노드로 지정
				new->parent = cur;

				cur = new;
			}
			else{ // 자식이 존재할 경우 

				cur = get_last_child(cur); // cur = 자식중의 막내노드 

				cur->next = new; // 막내 갱신
				new->prev = cur;

				cur = new;
			}
		}

		*idx += 1; // 인덱스 증가
	}

	return get_root(cur); // 만들어진 트리의 루트노드를 반환
}

node *change_sibling(node *parent) // 자식의 형제 노드들을 Swap (순서가 바뀐 동일 연산식 처리), 반환:parent
{
	node *tmp;
	
	tmp = parent->child_head;

	// 자식 노드 포인팅
	parent->child_head = parent->child_head->next;
	parent->child_head->parent = parent;
	parent->child_head->prev = NULL;

	// prev, next 유동 변경
	parent->child_head->next = tmp; 
	parent->child_head->next->prev = parent->child_head;
	parent->child_head->next->next = NULL;
	parent->child_head->next->parent = NULL;		

	return parent;
}

node *create_node(char *name, int parentheses) // 토큰 내용, 괄호개수 정보를 가진 노드 생성, 반환:new node
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

int get_precedence(char *op) // 연산자의 우선순위 반환, 반환:우선순위
{
	int i;

	for(i = 2; i < OPERATOR_CNT; i++){
		if(!strcmp(operators[i].operator, op))
			return operators[i].precedence;
	}
	return false;
}

int is_operator(char *op) // 연산자 체크, 연산자:true, 그외:false
{
	int i;

	for(i = 0; i < OPERATOR_CNT; i++)
	{
		if(operators[i].operator == NULL)
			break;
		if(!strcmp(operators[i].operator, op)){
			return true;
		}
	}

	return false;
}

void print(node *cur) // 트리 출력
{
	if(cur->child_head != NULL){ // 자식이 존재할 경우 자식을 출력하고 개행
		print(cur->child_head); 
		printf("\n");
	}

	if(cur->next != NULL){ // 형제의 경우 형제를 출력하고 띄어쓰기
		print(cur->next);
		printf("\t");
	}
	printf("%s", cur->name); // 토큰 출력
}

node *get_operator(node *cur) // 형제들의 부모 노드를 반환, 반환:parent 
{
	if(cur == NULL) 
		return cur;

	if(cur->prev != NULL)
		while(cur->prev != NULL)
			cur = cur->prev;

	return cur->parent;
}

node *get_root(node *cur) // 트리의 최상위 노드를 반환, 반환:root
{
	if(cur == NULL)
		return cur;

	while(cur->prev != NULL)
		cur = cur->prev;

	if(cur->parent != NULL)
		cur = get_root(cur->parent);

	return cur;
}

node *get_high_precedence_node(node *cur, node *new) // 두 노드 중 우선순위가 가장 높은 연산자 노드를 반환(기존 연산자만 리턴)
{
	if(is_operator(cur->name)) // 일단 비교 후, 기존 연산자 노드가 우선순위가 높으면 
		if(get_precedence(cur->name) < get_precedence(new->name))
			return cur; // 기존 연산자 노드 리턴

	if(cur->prev != NULL){ // 그런데 새로운 연산자 노드가 우선순위가 높을 경우, 기존 연산자 노드의 형제들 탐색
		while(cur->prev != NULL){ // 형제중 맏형 노드로 이동 후
			cur = cur->prev;
			
			return get_high_precedence_node(cur, new); // 맏형노드와 우선순위 비교
		}


		if(cur->parent != NULL) // 맏형 노드와도 비교 후 새 연산자 노드가 더 클 경우, 부모 노드 호출
			return get_high_precedence_node(cur->parent, new); // 부모 노드와 우선순위 비교
	}

	if(cur->parent == NULL) // 더이상 부모 노드가 없을경우
		return cur; // 현재 노드 반환
}

node *get_most_high_precedence_node(node *cur, node *new) // 새 연산자와 기존 연산자 노드,형제,부모 비교, 새연산자 노드가 우선순위가 가장 높을경우 새 연산자 노드도 리턴
{
	node *operator = get_high_precedence_node(cur, new); // 기존연산자(형제, 부모 포함)와 새 연산자 우선순위 비교
	node *saved_operator = operator; 

	while(1) // 가장 우선순위가 높은 연산자가 판별되고
	{
		if(saved_operator->parent == NULL) // 가장 우선순위의 노드의 부모가 존재하지 않으면 (새 연산자 혹은 기존 최우선 연산자), 더이상 비교 불가
			break; 

		if(saved_operator->prev != NULL) // 가장 우선순위 노드의 형제들이 존재하면
			operator = get_high_precedence_node(saved_operator->prev, new); // 형제노드들과 우선순위 비교

		else if(saved_operator->parent != NULL) // 가장 우선순위 노드의 부모 노드가 존재할 경우
			operator = get_high_precedence_node(saved_operator->parent, new); // 부모노드와 우선순위 비교

		saved_operator = operator; // 형제 노드도 없고, 부모노드도 없을 경우 
	}
	
	return saved_operator; // 결정된 최우선 노드 반환
}

node *insert_node(node *old, node *new) // 노드를 삽입, 삽입한 위치이후 노드는 자식관계로 전환, 삽입한 노드 반환
{
	if(old->prev != NULL){ // 형제가 존재할 경우 삽입할 노드와 형제관계로 이어줌
		new->prev = old->prev; // 기존 노드의 연결을 새노드가 물려받음
		old->prev->next = new;
		old->prev = NULL;
	}

	new->child_head = old; // 새 노드의 자식을 기존 노드로 변경
	old->parent = new; // 기존 노드의 부모를 새 노드로 변경

	return new;
}

node *get_last_child(node *cur) // 인자로 주어진 노드의 자식 중 막내를 리턴
{
	if(cur->child_head != NULL)
		cur = cur->child_head;

	while(cur->next != NULL)
		cur = cur->next;

	return cur;
}

int get_sibling_cnt(node *cur) // 인자로 주어진 노드의 형제 개수 반환, 반환:형제 수(자기자신 미포함)
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

void free_node(node *cur) // 노드 연결 해제
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


int is_character(char c) // 문자인지 체크하는 함수, 문자(숫자, 알파벳):1, 그외:0
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_typeStatement(char *str) // 타입정의문 유무 확인
{ 
	char *start;
	char str2[BUFLEN] = {0}; 
	char tmp[BUFLEN] = {0}; 
	char tmp2[BUFLEN] = {0}; 
	int i;	 
	
	start = str;
	strncpy(str2,str,strlen(str));
	remove_space(str2);

	while(start[0] == ' ')
		start += 1;
/*
	if(strstr(str2, "gcc") != NULL) 
	{
		strncpy(tmp2, start, strlen("gcc"));
		if(strcmp(tmp2,"gcc") != 0)
			return 0;
		else
			return 2;
	}
*/	
	for(i = 0; i < DATATYPE_SIZE; i++) // 데이터 타입 리스트중
	{
		if(strstr(str2,datatype[i]) != NULL) // 해당하는 데이터 타입이 문자열에 존재할 경우
		{	
			strncpy(tmp, str2, strlen(datatype[i])); // 데이터타입을 임시 배열에 저장
			strncpy(tmp2, start, strlen(datatype[i])); 
			
			if(strcmp(tmp, datatype[i]) == 0) // str이 특정 자료형으로 시작한다면
				if(strcmp(tmp, tmp2) != 0)
					return 0;  // 단순 자료형이 포함된 문자열 일 경우
				else
					return 2; // 변수 선언일 경우(자료형으로 시작하는 문장)
		}

	}
	return 1; // 아무것도 아닐 경우(캐스팅의 경우)

}

int find_typeSpecifier(char tokens[TOKEN_CNT][MINLEN]) // 형변환을 확인하고 그에 해당하는 토큰 넘버 반환, 오류: -1
{
	int i, j;

	for(i = 0; i < TOKEN_CNT; i++) // 데이터 타입 탐색
	{
		for(j = 0; j < DATATYPE_SIZE; j++)
		{
			if(strstr(tokens[i], datatype[j]) != NULL && i > 0)
			{
				if(!strcmp(tokens[i - 1], "(") && !strcmp(tokens[i + 1], ")") 
						&& (tokens[i + 2][0] == '&' || tokens[i + 2][0] == '*' 
							|| tokens[i + 2][0] == ')' || tokens[i + 2][0] == '(' 
							|| tokens[i + 2][0] == '-' || tokens[i + 2][0] == '+' 
							|| is_character(tokens[i + 2][0]))) // 캐스팅처럼 (데이터타입) 이후 문자 혹은 연산자가 오면 해당 토큰 넘버 반환 
					return i;
			}
		}
	}
	return -1;
}

int find_typeSpecifier2(char tokens[TOKEN_CNT][MINLEN]) // struct 구조체 사용시 토큰 넘버 반환, 오류: -1
{
    int i, j;

   
    for(i = 0; i < TOKEN_CNT; i++)
    {
        for(j = 0; j < DATATYPE_SIZE; j++)
        {
            if(!strcmp(tokens[i], "struct") && (i+1) <= TOKEN_CNT && is_character(tokens[i + 1][strlen(tokens[i + 1]) - 1]))  // 구조체 선언일 경우
                    return i; // 해당 토큰 넘버 반환
        }
    }
    return -1;
}

int all_star(char *str) // str이 모두 '*'일 경우:1 아닐경우: 0
{
	int i;
	int length= strlen(str);
	
 	if(length == 0)	
		return 0;
	
	for(i = 0; i < length; i++)
		if(str[i] != '*')
			return 0;
	return 1;

}

int all_character(char *str) // 받아온 문자열 중에 알파벳 혹은 숫자가 들어있으면 1, 아니면 0 
{
	int i;

	for(i = 0; i < strlen(str); i++)
		if(is_character(str[i]))
			return 1;
	return 0;
	
}

int reset_tokens(int start, char tokens[TOKEN_CNT][MINLEN]) // 형변환, struct와 같은 토큰을 읽었을 경우 문법 확인 후 결과 리턴, start = 형변환 | struct가 위치한 토큰 넘버
{
	int i;
	int j = start - 1; // 커서역할
	int lcount = 0, rcount = 0;
	int sub_lcount = 0, sub_rcount = 0;

	if(start > -1){
		if(!strcmp(tokens[start], "struct")) {	// 불러온 토큰이 struct 라면	
			strcat(tokens[start], " ");
			strcat(tokens[start], tokens[start+1]); // struct + ' ' + 변수명과 같은 형태로 문자열 합침

			for(i = start + 1; i < TOKEN_CNT - 1; i++){ // 토큰 배열 재정리
				strcpy(tokens[i], tokens[i + 1]);
				memset(tokens[i + 1], 0, sizeof(tokens[0]));
			}
		}

		else if(!strcmp(tokens[start], "unsigned") && strcmp(tokens[start+1], ")") != 0) { // 현재 토큰이 unsigned이고 다음 토큰이 ')'일 경우
			strcat(tokens[start], " "); // unsigned + ' ' + 그 다음 토큰 두개를 하나의 토큰으로 합침(ex. unsigned char i)
			strcat(tokens[start], tokens[start + 1]);	     
			strcat(tokens[start], tokens[start + 2]);

			for(i = start + 1; i < TOKEN_CNT - 1; i++){ // 토큰 배열 재정리
				strcpy(tokens[i], tokens[i + 1]);
				memset(tokens[i + 1], 0, sizeof(tokens[0]));
			}
		}

     		j = start + 1;           
        	while(!strcmp(tokens[j], ")")){ // 현재 토큰 뒤에 ')'가 나온 횟수 측정
                	rcount ++;
                	if(j==TOKEN_CNT)
                        	break;
                	j++;
        	}
	
		j = start - 1;
		while(!strcmp(tokens[j], "(")){ // 현재 토큰 앞에 '(' 나온 횟수 측정
        	        lcount ++;
                	if(j == 0)
                        	break;
               		j--;
		}
		if( (j!=0 && is_character(tokens[j][strlen(tokens[j])-1]) ) || j==0) // '('의 왼쪽 끝의 왼쪽에 char형 변수가 있거나 아무것도 없는 경우
			lcount = rcount; // 괄호의 짝이 같다고 판단

		if(lcount != rcount) // 괄호의 짝이 다를경우
			return false;

		if( (start - lcount) >0 && !strcmp(tokens[start - lcount - 1], "sizeof")){ // 토큰을 감싸고 있는 왼쪽괄호 전에 sizeof가 있을 경우
			return true; 
		}
		
		else if((!strcmp(tokens[start], "unsigned") || !strcmp(tokens[start], "struct")) && strcmp(tokens[start+1], ")")) { // unsigned나 struct의 바로 뒤에 닫는 괄호가 나온 경우
			strcat(tokens[start - lcount], tokens[start]); // 불필요하게 씌어진 괄호들을 제거
			strcat(tokens[start - lcount], tokens[start + 1]);
			strcpy(tokens[start - lcount + 1], tokens[start + rcount]);
		 
			for(int i = start - lcount + 1; i < TOKEN_CNT - lcount -rcount; i++) { // 제거한 이후의 토큰들을 정렬
				strcpy(tokens[i], tokens[i + lcount + rcount]);
				memset(tokens[i + lcount + rcount], 0, sizeof(tokens[0])); // 그 이후의 토큰 초기화
			}


		}
 		else{ // 그 외의 경우
			if(tokens[start + 2][0] == '('){ // // 현재 토큰으로부터 2번째 뒤의 토큰에 여는 괄호가 있을 경우
				j = start + 2;
				while(!strcmp(tokens[j], "(")){ // 그 뒤의 여는괄호가 닫힐 때까지 
					sub_lcount++;
					j++;
				} 	
				if(!strcmp(tokens[j + 1],")")){ // 여는 괄호 바로 다음 토큰이 닫는괄호 일경우
					j = j + 1;
					while(!strcmp(tokens[j], ")")){
						sub_rcount++;
						j++;
					}
				}
				else 
					return false;

				if(sub_lcount != sub_rcount) // 내부 괄호의 개수가 다른 경우
					return false;
				
				strcpy(tokens[start + 2], tokens[start + 2 + sub_lcount]); // start + 2 이후에 들어간 여ㅑ는 괄호를 제거	
				for(int i = start + 3; i<TOKEN_CNT; i++)
					memset(tokens[i], 0, sizeof(tokens[0])); // 그 이후의 토큰은 의미 없으므로 초기화

			}
			strcat(tokens[start - lcount], tokens[start]); // 괄호 제거
			strcat(tokens[start - lcount], tokens[start + 1]);
			strcat(tokens[start - lcount], tokens[start + rcount + 1]);
		 
			for(int i = start - lcount + 1; i < TOKEN_CNT - lcount -rcount -1; i++) { // 토큰 재정리
				strcpy(tokens[i], tokens[i + lcount + rcount +1]);
				memset(tokens[i + lcount + rcount + 1], 0, sizeof(tokens[0])); // 이후 토큰 초기화

			}
		}
	}
	return true;
}

void clear_tokens(char tokens[TOKEN_CNT][MINLEN]) // 토큰 배열 초기화
{
	int i;

	for(i = 0; i < TOKEN_CNT; i++)
		memset(tokens[i], 0, sizeof(tokens[i]));
}

char *rtrim(char *_str) // 문자열 오른쪽 공백 제거
{
	char tmp[BUFLEN];
	char *end;

	strcpy(tmp, _str);
	end = tmp + strlen(tmp) - 1;
	while(end != _str && isspace(*end))
		--end;

	*(end + 1) = '\0';
	_str = tmp;
	return _str;
}

char *ltrim(char *_str) // 문자열 왼쪽 공백 제거
{
	char *start = _str;

	while(*start != '\0' && isspace(*start))
		++start;
	_str = start;
	return _str;
}

char* remove_extraspace(char *str) // 잔여 공백 제거
{
	int i;
	char *str2 = (char*)malloc(sizeof(char) * BUFLEN);
	char *start, *end;
	char temp[BUFLEN] = "";
	int position;

	if(strstr(str,"include<")!=NULL){ // 답안 내용에 include<가 있는지 확인
		start = str; // 시작위치
		end = strpbrk(str, "<"); // <부터 위치 저장
		position = end - start;
	
		strncat(temp, str, position); // #include
		strncat(temp, " ", 1); // #include + ' '
		strncat(temp, str + position, strlen(str) - position + 1); // #include + ' ' + <filename>

		str = temp;		
	}
	
	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] ==' ') // 문자가 공백일 경우
		{
			if(i == 0 && str[0] ==' ') // 문자열 전의 공백일 경우
				while(str[i + 1] == ' ') // 인덱스 증가
					i++;	
			else{
				if(i > 0 && str[i - 1] != ' ') // 문자열 내부의 공백일 경우
					str2[strlen(str2)] = str[i];
				while(str[i + 1] == ' ') // 연속적 공백일 경우
					i++;
			} 
		}
		else // 공백이 없을 경우
			str2[strlen(str2)] = str[i];
	}

	return str2; // 불필요한 공백이 제거된 문자열 리턴
}



void remove_space(char *str) // 문자열 공백 제거 
{
	char* i = str;
	char* j = str;
	
	while(*j != 0)
	{
		*i = *j++;
		if(*i != ' ')
			i++;
	}
	*i = 0;
}

int check_brackets(char *str) // 괄호 짝 확인 함수, 맞음:1, 틀림:0
{
	char *start = str;
	int lcount = 0, rcount = 0;
	
	while(1){
		if((start = strpbrk(start, "()")) != NULL){
			if(*(start) == '(')
				lcount++;
			else
				rcount++;

			start += 1; 		
		}
		else
			break;
	}

	if(lcount != rcount)
		return 0;
	else 
		return 1;
}

int get_token_cnt(char tokens[TOKEN_CNT][MINLEN]) // 토큰의 개수 반환
{
	int i;
	
	for(i = 0; i < TOKEN_CNT; i++)
		if(!strcmp(tokens[i], ""))
			break;

	return i;
}
