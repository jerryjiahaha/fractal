#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include"str_process.h"



long double  str_calc(char *input,int *pos,int *pairflag){   //输入流,位置,括号匹配标示
	if(input==NULL){return 0;}
	long double value=0;
	int newpairflag=0;
	if(input[*pos]!='+' && input[*pos]!='-' && input[*pos]!='*' &&  input[*pos]!='/' && input[*pos]!='.' && input[*pos]!='(' && input[*pos]!=')' &&( input[*pos]<'0' || input[*pos]>'9') && input[*pos]!='\0' ){
		fprintf(stderr,"syntax error: unknown symbol %c (%s) in %s\n",input[*pos],input+*pos,input);
		*pos+=1;
		return str_calc(input,pos,&newpairflag);
	}

	switch (input[*pos]){
		case '\0':{return value;}
		case '*':{
				 *pos=*pos+1;
				 switch(input[*pos]){
					 case '\0':{fprintf(stderr,"no num after '*'\n"); exit(12);}
					 case '(':{newpairflag+=1; return value*str_calc(input,pos,&newpairflag);}
					 default:return value*str_to_longdouble(input,pos);
				 }
					 break;
			 }
		case '/':{
				 *pos=*pos+1;
				 switch(input[*pos]){
					 case '\0':{fprintf(stderr,"no num after '/'\n"); exit(13);}
					 case '(':{newpairflag+=1; return value/str_calc(input,pos,&newpairflag);}
					 default:return value/str_to_longdouble(input,pos);
				 }
				 break;
			 }
		case '+':{
				 *pos=*pos+1;
				 switch(input[*pos]){
					 case '\0':{fprintf(stderr,"no num after '+'\n"); exit(14);}
					 case '(':{newpairflag+=1; return value+str_calc(input,pos,&newpairflag);}
					 default:return value+str_calc(input,pos,&newpairflag);
				 }
				 break;
			 }
		case '-':{
				 *pos=*pos+1;
				 switch(input[*pos]){
					 case '\0':{fprintf(stderr,"no num after '-'\n"); exit(14);}
					 case '(':{newpairflag+=1; return value-str_calc(input,pos,&newpairflag);}
					 default:return value-str_calc(input,pos,&newpairflag);
				 }
				 break;
			 }
		case '(':{
				 newpairflag+=1;
//				 *pairflag+=1;
				 *pos+=1;
				 return str_calc(input,pos,&newpairflag);
			 }
		case ')':{

				 *pairflag-=1;
				 if(*pairflag<0){
					 fprintf(stderr,"error input ')' %s\n",input+*pos);
					 exit(12);
				 }
				 break;
			 }
		default:{        return str_to_longdouble(input,pos);
			}
	}
        
	return value;
}

	

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
	if(intlen1+intlen2 < numlen){      //与总长度比较判断是否有非法小数点,有多余小数点则报错并跳过
		fprintf(stderr,"sysntax error: invalid input %s\n",start);   
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



/*
int main(int argc,char *argv[]){
	if(argc<=1){return 0;}
	int pos=argv[1][0]-'0';
	printf("output:\t%Lf\nthen:\t%s",str_to_longdouble(argv[2],&pos),argv[2]+pos);
	return 0;
}
*/
