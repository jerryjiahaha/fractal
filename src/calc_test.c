#include"str_process.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

#define syntaxerror "\033[1;31m syntax error: \033[0m"

int str_numlen(char *a){     //计算数字符串长度
	if(a==NULL){return 0;}
	int  count=0;
	char tmp=a[0];
	while( ('0'<=tmp && tmp<='9') || tmp=='.' ){
		count+=1;
		tmp=a[count];
	}
	return count;
}

int str_intlen(char *a){     //计算小数点之间整数串长度
	if(a==NULL){return 0;}
	int count=0;
	char tmp=a[0];
	while(( '0'<=tmp && tmp<='9' )){
		count+=1;
		tmp=a[count];
	}
	return count;
}

/*目标：从输入流的某个位置起始，返回读入的一个数;
 * 对于这个数字符串，先输出其长度，然后计算小数点的位置，如果有多个小数点则返回错误或者忽略之后小数点
 * 最后可以根据小数点位置分成两部分计算*/
long double str_to_longdouble(char *input,int *pos){  //输入字符流,转换的起始位置
//	if(input==NULL || pos > strlen(input) || strlen(input+pos) < length ){
//		fprintf(stderr,"input error\n");
//	}
        if(input==NULL || *pos > strlen(input)){
		fprintf(stderr,"no input!\n");
		exit(10);
	}
//	static int dotflag=0;      //小数点标记
	static const int base=10;  //十进制
	long double value1=0;
        long double value2=0;
	int i=0;
	char *start=NULL;
	start=input+*pos;
        int numlen=str_numlen(start);   //计算数字符串长度
	if(numlen==0){
		fprintf(stderr,"no valid input! invalid %c (%s) from %s\n",input[*pos],input+*pos,input);
		exit(11);
	}
	int intlen1=str_intlen(start);  //计算小数点之前部分长度
	int intlen2=str_intlen(start+intlen1+1); //计算小数点之后部分长度
	if(start[intlen1]!='.'){intlen2=0;}
	if(intlen1+intlen2 < numlen-1){      //与总长度比较判断是否有非法小数点,有多余小数点则报错并跳过
		fprintf(stderr,"%s invalid input %s\n",syntaxerror,start);   
	}
	for(i=0;i<intlen1;i++){          //整数部分
		value1=value1*base+start[i]-'0';
	}
	for(i=intlen1+intlen2;i>intlen1;i--){   //小数部分
		value2=value2/base+start[i]-'0';
	}
	*pos=*pos+numlen;               //读取位置后移
	return value1+value2/base;      
}

/*一下是逆波兰表达式处理的栈基本操作*/
str_calc_stack * str_calc_stack_init(){
	str_calc_stack *nstack=NULL;
	nstack=(str_calc_stack *)calloc(1,sizeof(str_calc_stack));   //初始
	if(nstack==NULL){fprintf(stderr,"alloc error\n"); exit(-1);}
	nstack->next=NULL;
	nstack->operator=0,nstack->num=0;
	return nstack;
}

str_calc_stack * str_calc_stack_node(char operator,long double num){   //节点
	str_calc_stack *nstack=NULL;
	nstack=(str_calc_stack *)calloc(1,sizeof(str_calc_stack));
	if(nstack==NULL){fprintf(stderr,"alloc error\n"); exit(-1);}
	nstack->next=NULL;
	nstack->operator=operator,nstack->num=num;
	return nstack;
}

str_calc_stack * str_calc_stack_push(str_calc_stack *nstack,char operator,long double num){  //压栈
	str_calc_stack *new=NULL;
	new=str_calc_stack_node(operator,num);
	new->next=nstack;
	return new;
}

str_calc_stack * str_calc_stack_pop(str_calc_stack *nstack){   //弹出栈
	if(nstack==NULL){return NULL;}
	str_calc_stack *new=NULL;
	new=nstack->next;
	free(nstack);
	return new;
}

int str_calc_stack_destroy(str_calc_stack *nstack){    //销毁
	while(nstack!=NULL){
		nstack=str_calc_stack_pop(nstack);
	}
	return 0;
}

str_calc_stack * str_calc_stack_reverse(str_calc_stack *nstack){  //反转
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

int str_calc_stack_print(str_calc_stack *stack){  //遍历
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

str_calc_stack * str_calc_syntax(char *input,int *pos){   //逆波兰的构造
	if(input==NULL){fprintf(stderr,"non input!\n"); return 0;}
	int pairflag=0;
	char ntmp=input[*pos];
	str_calc_stack *result=NULL;
	str_calc_stack *tmp=NULL;
	tmp=str_calc_stack_init();
	tmp->operator='$';
//	result=str_calc_stack_init();

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
					 *pos+=1;
					 break;
				 }
			case 'p':{
					 *pos+=1;
					 if(input[*pos]=='i'){
						 result=str_calc_stack_push(result,0,M_PI);
						 *pos+=1;
					 }
					 else{
						 fprintf(stderr,"%sunknown symbol(or already at the end) '%c' at '%s' in '%s'\ndo you mean pi?)\n",syntaxerror,input[*pos],input+*pos,input);
						 if('a'<=input[*pos] && input[*pos]<='Z'){
							 *pos+=1;
						 }
						 else{
							 str_calc_stack_destroy(result);
							 str_calc_stack_destroy(tmp);
							 exit(13);
						 }
					 }
					 break;
				 }
			case 'e':{
					 result=str_calc_stack_push(result,0,M_E);
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
//	str_calc_stack_print(result);
//        result=str_calc_stack_push(result,'$',0);
        tmp=str_calc_stack_pop(tmp);

	result=str_calc_stack_reverse(result);

	return result;
}


/*一下是对计算式的栈的操作*/
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

int str_calc_num_destroy(str_calc_num *num){
	while(num!=NULL){
		num=str_calc_num_pop(num);
	}
	return 0;
}

/*根据逆波兰表达式进行计算*/
long double str_calc_revpolsum(str_calc_stack *calc){
	int len=0;
	long double a,b;
	char caltmp=0;
	long double result=0;
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
	str_calc_stack_destroy(calc);
	if(sum->next!=NULL){
		fprintf(stderr,"too much num\n"); 
	        str_calc_num_destroy(sum);
		exit(19);}
	result=sum->value;
	str_calc_num_destroy(sum);
	return result;
}



long double str_calc(char *input){
	long double value=0;
	str_calc_stack * calinput=NULL;
	int i=0;
	calinput=str_calc_syntax(input,&i);
//	str_calc_stack_print(calinput);
	value=str_calc_revpolsum(calinput);

//	printf("\033[1;34mresult is \033[1;32m %Lf\033[0m\n",r);
	return value;
}



int main(int argc,char *argv[]){
	if(argc==1){return 0;}
	else{
		printf("\033[1;34mresult is \033[1;32m %Lf\033[0m\n",str_calc(argv[1]));
	}
	return 0;
}

