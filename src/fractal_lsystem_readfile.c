#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"fractal_lsystem.h"
#include"file.h"
#include"str_process.h"

int fractal_lsystem_conf_show(fractal_lsystem_conf *lsystem);

char **je_realloc(char **origin,int size){
        if(size<=0){return NULL;}
	char **tmp=NULL;
	int i=0;
	if(size==1){
		tmp=(char **)calloc(1,sizeof(char *));
		if(tmp[0]!=NULL){
			printf("\r\tWTF!\n");
		}
		tmp[0]=NULL;
	}
	else{
		tmp=(char **)calloc(size,sizeof(char *));
		if(tmp==NULL){exit(-1);}
		if(tmp[size-1]!=NULL){
			printf("\r\tWTF 2!\n");
		}
		tmp[size-1]=NULL;
		for(i=0;i<size-1;i++){
			tmp[i]=origin[i];
		}
		free(origin);
	}
	return tmp;
}



int fractal_lsystem_rule_get(file_read_word *cur,int *frule,fractal_lsystem_conf **lsystem,file_read_stack *fstack){
	int i=0;
	int j=str_len((*lsystem)->symbol);
	if(j==0){
		(*lsystem)->symbol=(char *)calloc(1,sizeof(char));
		*(((*lsystem)->symbol)+0)='\0';
	        j=str_len((*lsystem)->symbol);
	}
	if(str_len(cur->word)!=2){
		fprintf(stderr,"syntax error: symbol must be only one character,but %s has %d,skip this\n",cur->word,str_len(cur->word)-1);
		(*frule)-=1;
		return 1;
	}
	else{
		while(i<j-1){
			if((*lsystem)->symbol[i] == cur->word[0]){
				fprintf(stderr,"error: rule for symbol %c has already defined , use the first\n",(*lsystem)->symbol[i]);
				(*frule)-=1;
				return 1;
			}
			i+=1;
		}
//		fractal_lsystem_conf_show(*lsystem);
	        
		(*lsystem)->symbol=(char *)realloc((*lsystem)->symbol,(*frule+1)*sizeof(char));
		if((*lsystem)->symbol==NULL){
			fprintf(stderr,"alloc err!\n");
			exit(-1);
		}
                (*lsystem)->symbol[*frule-1]=cur->word[0];
		(*lsystem)->symbol[*frule]='\0';
		(*lsystem)->rule=je_realloc((*lsystem)->rule,*frule);
		if((*lsystem)->rule==NULL){
			fprintf(stderr,"alloc err!\n");
			exit(-1);
		}
		str_cp((cur->next)->word,&(*lsystem)->rule[(*frule)-1]);
	}
	return 0;
}






fractal_lsystem_conf * fractal_lsystem_syntax(file_read_stack * fstack){
	fractal_lsystem_conf * lsystem=NULL;
	lsystem=(fractal_lsystem_conf *)calloc(1,sizeof(fractal_lsystem_conf));
	if(lsystem==NULL){exit(-1);}
	lsystem->name=lsystem->begin=lsystem->symbol=NULL;
	lsystem->rule=NULL;
	int fname=0,fang=0,fbegin=0,frule=0;
	file_read_word *cur=NULL;
	cur=fstack->ftop;
	while(cur != fstack->fbase){
		switch(cur->word[0]){
			case 'n':{
                                         if(strcmp(cur->word,"name") ==0){
						 cur=cur->next;
						 if( cur == fstack->fbase ){
							 fprintf(stderr,"syntax error: none after name\n");
							 exit(3);
						 }
						 fname+=1;
						 if(fname==1){
							 str_cp(cur->word,&(lsystem->name));
						 }
						 else if(fname==2){
							 fprintf(stderr,"more than one name !use the first!\n");
						 }
					 }
					 else{
						 fprintf(stderr,"syntax error: unrecognize symbol '%s' , maybe you mean 'name'?\n",cur->word);
						 exit(8);
					 }
					 break;
				 }
			case 'a':{
					 if(strcmp(cur->word,"angle") ==0){
						 cur=cur->next;
						 if(cur == fstack->fbase ){
							 fprintf(stderr,"syntax error: none after angle\n");
							 exit(4);
						 }
						 fang+=1;
						 if(fang==1){
							 lsystem->angle=str_calc(cur->word);
						 }
						 else if(fang==2){
							 fprintf(stderr,"more than one angle! use the first!\n");
						 }
					 }
					 else{
						 fprintf(stderr,"syntax error: unrecognize symbol '%s' , maybe you mean 'angle'?\n",cur->word);
						 exit(9);
					 }
					 break;
				 }
			case 'b':{
					 if(strcmp(cur->word,"begin") ==0){
						 cur=cur->next;
						 if(cur == fstack->fbase ){
							 fprintf(stderr,"sysntax error: none after begin\n");
							 exit(5);
						 }
						 fbegin+=1;
						 if(fbegin==1){
							 str_cp(cur->word,&lsystem->begin);
						 }
						 else if(fbegin==2){
							 fprintf(stderr,"more than one begin! use the first!\n");
						 }
					 }
					 else{
						 fprintf(stderr,"syntax error: unrecognize symbol '%s' , maybe you mean 'begin'?\n",cur->word);
						 exit(10);
					 }
					 break;
				 }
			case 'r':{
					 if(strcmp(cur->word,"rule") ==0){
						 cur=cur->next;
						 if(cur == fstack->fbase ){
							 fprintf(stderr,"syntax error: none after rule\n");
							 exit(6);
						 }
						 else if(cur->next == fstack->fbase ){
							 fprintf(stderr,"syntax error: not enouph char behind rule\n");
							 exit(6);
						 }
						 frule+=1;
						 fractal_lsystem_rule_get(cur,&frule,&lsystem,fstack);
						 cur=cur->next;
					 }
					 else{
						 fprintf(stderr,"syntax error: unrecognize symbol '%s' , maybe you mean 'rule'?\n",cur->word);
						 exit(11);
					 }
					 break;
				 }
			default:{
					fprintf(stderr,"unrecognize symbol %s\n",cur->word);
					exit(7);
					break;
				}
		}
		cur=cur->next;
	}
        return lsystem;
}

int fractal_lsystem_free(fractal_lsystem_conf *lsystem){
	if(lsystem->name!=NULL){
		free(lsystem->name);
	}
	if(lsystem->begin!=NULL){
		free(lsystem->begin);
	}
	int i=0,j=0;
	if(lsystem->symbol!=NULL){
	        j=str_len(lsystem->symbol)-1;
		for(i=0;i<j;i++){
			if(lsystem->rule!=NULL){
				free(lsystem->rule[i]);
			}
		}
                if(lsystem->rule!=NULL){
		        free(lsystem->rule);
		}
		free(lsystem->symbol);
	}
	free(lsystem);
	return 0;
}

int  fractal_lsystem_conf_show(fractal_lsystem_conf *lsystem){
	printf("\n");
	int i=0,j=0;
        if(lsystem!=NULL){
		if(lsystem->name!=NULL){
			printf("name\t%s\n",lsystem->name);
		}
		printf("angle\t%Lf*Pi\n",lsystem->angle);
		if(lsystem->begin!=NULL){
			printf("begin\t%s\n",lsystem->begin);
		}
		i=str_len(lsystem->symbol);
		if(i>1){
			i-=1;
			printf("symbol\t%s\n",lsystem->symbol);
                        for(j=0;j<i;j++){
				printf("rule\t%c = %s\n",lsystem->symbol[j],lsystem->rule[j]);
			}
		}
		return 0;
	}
	return 1;
}




fractal_lsystem_conf * fractal_lsystem_conf_get(char *filename){
	file_read_stack * frac_stack=NULL;
	fractal_lsystem_conf * lsystem=NULL;
       
	frac_stack=file_read_build_stack(filename);
	file_read_stack_output(frac_stack);
        lsystem=fractal_lsystem_syntax(frac_stack);
        fractal_lsystem_conf_show(lsystem);
	read_stack_destroy(frac_stack),frac_stack=NULL;
	
	return lsystem;
}
