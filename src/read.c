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


typedef int fracread_comment_state;
/*aim:去掉注释,分解语句,每次调用后文件指针处于注释行回车后或正文前,则连续两次调用该函数文件指针位置不会变*/
int fractal_readcomment(FILE* fp,int *i,int *k,fracread_comment_state *cm_state){ 
	char ftmp=0;
	ftmp=fgetc(fp),(*i)+=1;	  //i count words num
	*cm_state=0;
	while(!feof(fp)){
		if(ftmp=='#' || ftmp==';'){  // '#'';'是注释符号
			*cm_state=1;
			ftmp=fgetc(fp),(*i)+=1;
			while(!feof(fp) && ftmp !='\n'){
				ftmp=fgetc(fp),(*i)+=1;
			}
			if(!feof(fp)){
			        ftmp=fgetc(fp),(*i)+=1;
			}
		}
		else{
			fseek(fp,-1L,SEEK_CUR);
			(*i)-=1,(*k)+=1;
		        return 0;
		}
	}
	return 1;
}


int fractal_lsystem_read(char *filename,fractal_lsystem * lsystem){

	FILE *fp=NULL;
	char ftmp=0;
	char *ctmp=NULL;
	int i=0,k=0;
	fracread_comment_state cm_state;
	fp=fopen(filename,"r");
	if(fp==NULL){
		fprintf(stderr,"conf File not found\nuse default");
		return 1;
	}
        while( fractal_readcomment(fp,&i,&k,&cm_state) == 0 ){
		i=0,k=0;
	        while( fractal_readcomment(fp,&i,&k,&cm_state)==0  && cm_state==0 && ( ftmp=fgetc(fp) ) != '\n' && ftmp != '\t' && ftmp != ' '){                             i+=1;
		}
		printf("k=%d\t",k);
                if(i!=0){
			i+=1;
			ctmp=(char*)realloc(ctmp,k*sizeof(char));
                	if(ctmp==NULL){exit(-1);}
		
                        fseek(fp,-i,SEEK_CUR);
                        fscanf(fp,"%s",ctmp);
                        printf("ctmp:%s\n",ctmp);
		}
	}
        
	fclose(fp),fp=NULL;
	free(ctmp),ctmp=NULL;
	return 0;
}

int main(int argc,char *argv[]){
	fractal_lsystem * lsystem=NULL;
	lsystem=(fractal_lsystem *)calloc(1,sizeof(fractal_lsystem));
	if(lsystem==NULL){return -1;}
	if(argc==1){
		fractal_lsystem_read("lsystem.conf",lsystem);
	}
	else{
		fractal_lsystem_read(argv[1],lsystem);
	}
	free(lsystem),lsystem=NULL;
	return 0;
}
