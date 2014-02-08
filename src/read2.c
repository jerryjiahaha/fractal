#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"fractal_lsystem.h"

typedef struct fractal_lsystem{
	char *name;
        long double angle;
        char *begin;
	char **rule;
}fractal_lsystem;

int file_byte_count(FILE *fp){
	int i=0;
	fgetc(fp);
	while(!feof(fp)){
		i+=1;
		fgetc(fp);
	}
	fseek(fp,0L,0);
	return i;
}


/*aim:去掉注释,分解语句,每次调用后文件指针处于注释行回车后或正文前,则连续两次调用该函数文件指针位置不会变*/
int file_read_comment(FILE *fp,int *cstat,int *wcount){
	*wcount=0;
	char ctmp=0;
	if(feof(fp)){return 2;}
	ctmp=fgetc(fp),(*wcount)+=1;
	*cstat=0;
	while(!feof(fp)){
		if(ctmp=='#' || ctmp==';'){                 // '#',';' 为注释 
			*cstat=1;                           //遇到注释,标记
			ctmp=fgetc(fp),*(wcount)+=1;  
			while(ctmp != '\n' && ctmp != '\r' && !feof(fp)){   //读入整行将其跳过
				ctmp=fgetc(fp),(*wcount)+=1;
			}
			if(!feof(fp)){
				ctmp=fgetc(fp),(*wcount)+=1;             //继续判断下一行
			}
		}
		else{                                       //遇到常规字符,退回
			fseek(fp,-1L,SEEK_CUR),*(wcount)-=1;
			return 0;
		}
	}
	return 1;
}

typedef struct file_read_word{
	char *word;
	int  pos;
	struct file_read_word *next;
	struct file_read_word *previous;
}file_read_word;

int fractal_read_stack(char *filename){
	FILE *fp=NULL;
	char *ftmp=NULL;
	if( ( fp=fopen(filename,"r") ) == NULL ){
		fprintf(stderr,"file not found!\n");
		exit(2);
	}
        char ctmp=0;          //store 1 byte from file
	int cstat=0,wlen=0;   //cstat注释标记,wlen有效字符长度
	int j=0;              
	int wcount=0;         //连续处理字符数
        while( file_read_comment(fp,&cstat,&wcount) == 0 ){
		wlen=0;
		while( file_read_comment(fp,&cstat,&wcount)==0 && cstat==0  && (ctmp=fgetc(fp))!=' ' && ctmp!='\t' && ctmp!='\n' && ctmp!='\r'){
		//	printf("%c",ctmp);
			wlen+=1;
		}
		if(wlen!=0){                        
			ftmp=(char *)realloc(ftmp,(wlen+1)*sizeof(char));
			if(ftmp==NULL){exit(-1);}
			if(cstat==0){				
                                fseek(fp,-wlen-1,SEEK_CUR);
			}
			else{
		//		printf("\twc=%d\t",wcount);
				fseek(fp,-wlen-wcount,SEEK_CUR);
			}
			for(j=0;j<wlen;j++){
				ftmp[j]=fgetc(fp);
			}
			ftmp[wlen]='\0';
		        printf(" %d\t%s\n",wlen,ftmp);
			fseek(fp,wcount,SEEK_CUR);
		}
	}
	free(ftmp),ftmp=NULL;
	fclose(fp),fp=NULL;
	return 0;
}


int main(int argc,char *argv[]){
	
	fractal_lsystem * lsystem=NULL;
	lsystem=(fractal_lsystem *)calloc(1,sizeof(fractal_lsystem));
	if(lsystem==NULL){exit(-1);}
	
	if(argc==1){
		fractal_read_stack("lsystem.conf");
	}
	else{
		fractal_read_stack(argv[1]);
	}
        
	free(lsystem),lsystem=NULL;
	return 0;
}
