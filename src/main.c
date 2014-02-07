#include<math.h>
#include<stdlib.h>
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
	extern long double fractal_angle;
        fractal *fractal_stack=NULL;
	char **fractal_rule=NULL;
	char fractal_begin[]="X";   //initial state
	fractal_stack=fractal_init(fractal_begin);
	char fractal_symbol[]="XF";  //符号表,用于迭代
	fractal_rule=fractal_rule_build(fractal_symbol,"F-[[X]+X]+F[+FX]-X","FF");
	fractal_time=6;
	fractal_angle=M_PI*25/180;
	int i=0;
	fractal_output(fractal_stack);
	fractal_draw(fractal_stack,i);
	for(i=0;i<fractal_time;i++){
	        fractal_transform(fractal_stack,fractal_symbol,fractal_rule);
	        fractal_output(fractal_stack);
	        fractal_draw(fractal_stack,i+1);
	}
	fractal_destroy(fractal_stack);
	if(fractal_rule != NULL){
		free(fractal_rule),fractal_rule=NULL;
	}
	return 0;
}


