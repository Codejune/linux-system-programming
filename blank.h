#ifndef BLANK_H_
#define BLANK_H_

#ifndef true
	#define true 1
#endif
#ifndef false
	#define false 0
#endif
#ifndef BUFLEN
	#define BUFLEN 1024
#endif

#define OPERATOR_CNT 24
#define DATATYPE_SIZE 35
#define MINLEN 64
#define TOKEN_CNT 50

typedef struct node{
	int parentheses;
	char *name;
	struct node *parent;
	struct node *child_head;
	struct node *prev;
	struct node *next;
}node;

typedef struct operator_precedence{
	char *operator;
	int precedence;
}operator_precedence;

void compare_tree(node *root1,  node *root2, int *result);
node *make_tree(node *root, char (*tokens)[MINLEN], int *idx, int parentheses);
node *change_sibling(node *parent);
node *create_node(char *name, int parentheses);
int get_precedence(char *op);
int is_operator(char *op);
void print(node *cur);
node *get_operator(node *cur);
node *get_root(node *cur);
node *get_high_precedence_node(node *cur, node *new);
node *get_most_high_precedence_node(node *cur, node *new);
node *insert_node(node *old, node *new);
node *get_last_child(node *cur);
void free_node(node *cur);
int get_sibling_cnt(node *cur);

int make_tokens(char *str, char tokens[TOKEN_CNT][MINLEN]);
int is_typeStatement(char *str);
int find_typeSpecifier(char tokens[TOKEN_CNT][MINLEN]);
int find_typeSpecifier2(char tokens[TOKEN_CNT][MINLEN]);
int is_character(char c);
int all_star(char *str);
int all_character(char *str);
int reset_tokens(int start, char tokens[TOKEN_CNT][MINLEN]);
void clear_tokens(char tokens[TOKEN_CNT][MINLEN]);
int get_token_cnt(char tokens[TOKEN_CNT][MINLEN]);
char *rtrim(char *_str);
char *ltrim(char *_str);
void remove_space(char *str);
int check_brackets(char *str);
char* remove_extraspace(char *str);

#endif
