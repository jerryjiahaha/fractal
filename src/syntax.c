#include"str_process.h"
#include<stdlib.h>
#include<stdio.h>

typedef struct str_calc_stack{  //记录逆波兰表达式的栈
	char operator;
	long double  num;
	struct str_calc_stack *next;
}str_calc_stack;

str_calc_stack * str_calc_stack_init(){
	str_calc_stack *nstack=NULL;
	nstack=(str_calc_stack *)calloc(1,sizeof(str_calc_stack));
	if(nstack==NULL){fprintf(stderr,"alloc error\n"); exit(-1);}
	nstack->next=NULL;
	nstack->operator=0,nstack->num=0;
	return nstack;
}

str_calc_stack * str_calc_stack_node(char operator,long double num){
	str_calc_stack *nstack=NULL;
	nstack=(str_calc_stack *)calloc(1,sizeof(str_calc_stack));
	if(nstack==NULL){fprintf(stderr,"alloc error\n"); exit(-1);}
	nstack->next=NULL;
	nstack->operator=operator,nstack->num=num;
	return nstack;
}

str_calc_stack * str_calc_stack_push(str_calc_stack *nstack,char operator,long double num){
	str_calc_stack *new=NULL;
	new=str_calc_stack_node(operator,num);
	new->next=nstack;
	return new;
}

str_calc_stack * str_calc_stack_pop(str_calc_stack *nstack){
	if(nstack==NULL){return NULL;}
	str_calc_stack *new=NULL;
	new=nstack->next;
	free(nstack);
	return new;
}

int str_calc_stack_destroy(str_calc_stack *nstack){
	while(nstack!=NULL){
		nstack=str_calc_stack_pop(nstack);
	}
	return 0;
}

str_calc_stack * str_calc_stack_reverse(str_calc_stack *nstack){
        if(nstack==NULL){return NULL;}
	str_calc_stack *mstack=NULL;
	str_calc_stack *tmp=NULL;
        mstack=nstack;
	nstack=nstack->next;
	mstack->next=NULL;
	while(nstack!=NULL){
		tmp=nstack->next;
		nstack->next=mstack;
		mstack=nstack;
		nstack=tmp;
	}
	return mstack;
}

int str_calc_stack_print(str_calc_stack *stack){
        printf("\n");
	while(stack!=NULL){
		if(stack->operator==0){
			printf("\033[1;32m%Lf\033[0m ",stack->num);
		}
		else{
			printf("\033[1;33m%c\033[0m ",stack->operator);
		}
		stack=stack->next;
	}
        printf("\n");
	return 0;
}

str_calc_stack * syntax(char *input,int *pos){
	if(input==NULL){fprintf(stderr,"non input!\n"); return 0;}
	int pairflag=0;
	char ntmp=input[*pos];
	str_calc_stack *result=NULL;
	str_calc_stack *tmp=NULL;
	tmp=str_calc_stack_init();
	tmp->operator='$';
//	result=str_calc_stack_init();
//	int length=strlen(input);
	while(ntmp!='\0'){
		switch(ntmp){
			case '(':{
					 *pos+=1;
					 pairflag+=1;
					 tmp=str_calc_stack_push(tmp,'(',0);
					 break;
				 }
			case ')':{
					 pairflag-=1;
					 if(pairflag<0){fprintf(stderr,"sysntax error: invalid(too many) ')' in %s",input+*pos); exit(17);}
					 while(tmp->operator != '('){
						 result=str_calc_stack_push(result,tmp->operator,tmp->num);
						 tmp=str_calc_stack_pop(tmp);
					 }
					 tmp=str_calc_stack_pop(tmp);
					 *pos+=1;
					 break;
				 }
			case '+':
			case '-':{
				//	 ntmp=input[*pos];
					 while(tmp->operator!='$'){
						 if(tmp->operator=='('){break;}
						 else{
							 result=str_calc_stack_push(result,tmp->operator,tmp->num);
							 tmp=str_calc_stack_pop(tmp);
						 }
					 }
					 tmp=str_calc_stack_push(tmp,ntmp,0);
					 *pos+=1;
					 break;
				 }
			case '*':
			case '/':{
					 while(tmp->operator!='$' && tmp->operator!='+' && tmp->operator!='-'){
						 if(tmp->operator=='('){break;}
						 else{
							 result=str_calc_stack_push(result,tmp->operator,tmp->num);
							 tmp=str_calc_stack_pop(tmp);
						 }
					 }
					 tmp=str_calc_stack_push(tmp,ntmp,0);
	printf("next\n");
					 *pos+=1;
					 break;
				 }
			default:{
					result=str_calc_stack_push(result,0,str_to_longdouble(input,pos));
				}
		}
		ntmp=input[*pos];
	}
	if(pairflag!=0){fprintf(stderr,"syntax error: '(' is more than ')'"); exit(16);}
	while(tmp->operator!='$'){
		result=str_calc_stack_push(result,tmp->operator,tmp->num);
		tmp=str_calc_stack_pop(tmp);
	}
	str_calc_stack_print(result);
//        result=str_calc_stack_push(result,'$',0);
        tmp=str_calc_stack_pop(tmp);

	result=str_calc_stack_reverse(result);

	return result;
}

typedef struct str_calc_num{
	long double value;
	struct str_calc_num * next;
}str_calc_num;

str_calc_num * str_calc_num_node(long double value){
        str_calc_num * sum=NULL;
	sum=(str_calc_num*)calloc(1,sizeof(str_calc_num));
	if(sum==NULL){fprintf(stderr,"alloc err\n");  exit(-1);}
	sum->next=NULL;
	sum->value=value;
	return sum;
}

str_calc_num * str_calc_num_push(str_calc_num *sum,long double value){
	str_calc_num * new=NULL;
	new=str_calc_num_node(value);
	new->next=sum;
	return new;
}

str_calc_num * str_calc_num_pop(str_calc_num *sum){
	str_calc_num * new=NULL;
        if(sum==NULL){return NULL;}
	new=sum->next;
	free(sum);
	return new;
}

long double str_calc_revpolsum(str_calc_stack *calc){
	int len=0;
	int a,b;
	char caltmp=0;
	str_calc_num * sum=NULL;
	if(calc==NULL || calc->operator != 0){fprintf(stderr,"syntax error input or NULL stack\n"); return 0;}
	sum=str_calc_num_node(calc->num);
	len+=1;
	calc=str_calc_stack_pop(calc);
	while(calc!=NULL){
		caltmp=calc->operator;
		if(caltmp==0){
			sum=str_calc_num_push(sum,calc->num);
			len+=1;
		}
		else{
	                if(len<2){fprintf(stderr,"syntax error ,too much operator"); exit(17);}
			a=sum->value;
			sum=str_calc_num_pop(sum);
			b=sum->value;
			sum=str_calc_num_pop(sum);
			switch(caltmp){
				case '+':{
						 sum=str_calc_num_push(sum,a+b);
						 break;
					 }
                                case '-':{
						 sum=str_calc_num_push(sum,b-a);
						 break;
					 }
			        case '*':{
						 sum=str_calc_num_push(sum,a*b);
						 break;
					 }
                                case '/':{
						 sum=str_calc_num_push(sum,b/a);
						 break;
					 }
				default:{fprintf(stderr,"syntax error,undefined symbol '%c'\n",caltmp); exit(18);}
				len-=1;
			}
		}
		calc=str_calc_stack_pop(calc);
	}
	if(sum->next!=NULL){
		fprintf(stderr,"too much num\n"); 
		str_calc_stack_destroy(calc);
		exit(19);}
	return sum->value;
}

int main(int argc,char *argv[]){
	if(argc==0){return 0;}
	str_calc_stack * calinput=NULL;
	int i=0;
	calinput=syntax(argv[1],&i);
	str_calc_stack_print(calinput);
	long double r=str_calc_revpolsum(calinput);
	printf("\033[1;34mresult is \033[1;32m %Lf\033[0m\n",r);
	return 0;
}

