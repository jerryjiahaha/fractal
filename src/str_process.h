#ifndef _STR_CALC_H_
#define _STR_CALC_H_


typedef struct str_calc_stack{  //记录逆波兰表达式的栈
	char operator;
	long double  num;
	struct str_calc_stack *next;
}str_calc_stack;

typedef struct str_calc_num{
	long double value;
	struct str_calc_num * next;
}str_calc_num;

long double str_to_longdouble(char *input,int *pos);
long double str_calc(char *input,int *pos,int *pairflag);

int str_len(char *str);
int str_cp(char *from,char **to);

#endif
