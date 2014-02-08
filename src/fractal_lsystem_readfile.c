#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"fractal_lsystem.h"
#include"file.h"



int main(int argc,char *argv[]){

	file_read_stack * frac_stack=NULL;
	fractal_lsystem_conf * lsystem=NULL;
	lsystem=(fractal_lsystem_conf *)calloc(1,sizeof(fractal_lsystem_conf));
	if(lsystem==NULL){exit(-1);}
	
	if(argc==1){
		frac_stack=file_read_build_stack("lsystem.conf");
	}
	else{
		frac_stack=file_read_build_stack(argv[1]);
	}
       
	file_read_stack_output(frac_stack);

	read_stack_destroy(frac_stack),frac_stack=NULL;
	free(lsystem),lsystem=NULL;
	return 0;
}
