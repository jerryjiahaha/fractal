#include"str_process.h"
#include<stdio.h>
#include<stdlib.h>

int main(){
	char *name=NULL;
	char *kaka=NULL;
	char k[]="fuckyouall";
	str_cp(k,&kaka);
	str_cp(kaka,&name);
        printf("name:%s\n%d\n",name,str_len(k));
        free(kaka);
	free(name);


	char **rule=NULL;
	rule=(char**)realloc(rule,3*sizeof(char *));
	rule=(char**)realloc(rule,4*sizeof(char *));
	printf("\tnow| %s| %d\n",*(rule+3),str_len(*(rule+3)));
//	printf("\tnow:%s,%d\n",*(rule+1));
	return 0;
}



