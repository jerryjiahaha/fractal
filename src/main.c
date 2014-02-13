#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>
#include<string.h>
#include"fractal_lsystem.h"

char ** fractal_rule_build(char fractal_symbol[],...){
	va_list ap;
	int i;
	va_start(ap,fractal_symbol);
        

	char **fractal_rule=NULL;
	fractal_rule=(char **)calloc(strlen(fractal_symbol),sizeof(char *));
	if(fractal_rule==NULL){exit(-1);}
	for(i=0;i<strlen(fractal_symbol);i++){
                fractal_rule[i]=va_arg(ap,char *);
	}
	va_end(ap);
	return fractal_rule;
}

int main(int argc,char *argv[]){
	extern int fractal_time;
//	extern long double fractal_angle;
        fractal *fractal_stack=NULL;
	fractal_lsystem_conf *lsystem=NULL;
	if(argc==1){
		lsystem=fractal_lsystem_conf_get("lsystem.conf");
	}
	else{
	        lsystem=fractal_lsystem_conf_get(argv[1]);
	}
//	char **fractal_rule=NULL;
//	char fractal_begin[]="FXF+Y++FXF+Y++FXF+Y++FXF+Y++";   //initial state
	printf("debug2\n");
	fractal_stack=fractal_init(lsystem->begin);
//	char fractal_symbol[]="XFY";  //符号表,用于迭代
//	fractal_rule=fractal_rule_build(fractal_symbol,"+F[X]YF+X","FXF","Y+[X]+F");
	fractal_time=6;
//	fractal_angle=M_PI/6;
	int i=0;
	fractal_output(fractal_stack);
	fractal_draw(fractal_stack,lsystem->angle*M_PI,i);
	for(i=0;i<fractal_time;i++){
	        fractal_transform(fractal_stack,lsystem->symbol,lsystem->rule);
	        fractal_output(fractal_stack);
	        fractal_draw(fractal_stack,lsystem->angle,i+1);
	}
	fractal_destroy(fractal_stack);
//	if(fractal_rule != NULL){
//		free(fractal_rule),fractal_rule=NULL;
//	}
        if(lsystem !=NULL){
		fractal_lsystem_free(lsystem);
		lsystem=NULL;
	}
	return 0;
}


