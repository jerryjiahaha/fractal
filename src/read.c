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

int fractal_readcomment(FILE* fp,int *i){
	char ftmp=0;
	ftmp=fgetc(fp),(*i)+=1;	
	while(!feof(fp)){
		if(ftmp=='#' || ftmp==';'){  // '#'';'是注释符号
			ftmp=fgetc(fp),(*i)+=1;
			while(!feof(fp) && ftmp !='\n'){
				ftmp=fgetc(fp),(*i)+=1;
			}
			if(!feof){
			        ftmp=fgetc(fp),(*i)+=1;
			}
		}
		else{
			fseek(fp,-1L,SEEK_CUR);
			(*i)-=1;
		        return 0;
		}
	}
	return 1;
}


int fractal_lsystem_read(char *filename,fractal_lsystem * lsystem){

	FILE *fp=NULL;
	char ftmp=0;
	char *ctmp=NULL;
	int i=0;
	fp=fopen(filename,"r");
	if(fp==NULL){
		fprintf(stderr,"conf File not found\nuse default");
		return 1;
	}
        while( fractal_readcomment(fp,&i) == 0 ){
		i=0;
	        while( fractal_readcomment(fp,&i)==0  &&  ( ftmp=fgetc(fp)	) != '\n' && ftmp != '\t' && ftmp != ' '){                        
	                i+=1;
		}
                if(i!=0){
			i+=1;
			ctmp=(char*)realloc(ctmp,i*sizeof(char));
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
