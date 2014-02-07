#include<stdio.h>
#include<string.h>
#include"fractal_lsystem.h"

typedef struct fractal_lsystem{
	char *name;
        long double angle;
        char *begin;
	char **rule;
}fractal_lsystem;

int fractal_readcomment(FILE* fp){
	char ftmp=0;
	ftmp=fgetc(fp);
	while(!feof(fp)){
		if(ftmp=='#' || ftmp==';'){  // '#'';'是注释符号
			ftmp=fgetc(fp);
			while(!feof(fp) && ftmp !='\n'){
				ftmp=fgetc(fp);
			}
			ftmp=fgetc(fp);
		}
		else{
			fseek(fp,-1L,SEEK_CUR);
		        return 0;
		}
	}
	return 1;
}


int fractal_lsystem_read(char *filename,fractal_lsystem * lsystem){

	FILE *fp=NULL;
	char ftmp=0;
	int i=0;
	fp=fopen(filename,"r");
	if(fp==NULL){
		fprintf(stderr,"conf File not found\nuse default");
		return 1;
	}
        while( fractal_readcomment(fp) == 0 ){
		ftmp=fgetc(fp);
		if(!feof(fp) && ftmp != '\n'){
			i+=1;


            

	return 0;
}

int main(int argc,char *argv[]){
	fractal_lsystem * lsystem=NULL;
	if(argc==1){
		fractal_lsystem_read("lsystem.conf",lsystem);
	}
	else{
		fractal_lsystem_read(argv[1],lsystem);
	}
	return 0;
}
