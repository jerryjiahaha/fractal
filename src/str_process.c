#include"str_process.h"
#include<stdlib.h>
#include<stdio.h>


int str_len(char *str){
	int  i=0;
	char c=0;
	if(str==NULL){
		return 0;
	}
	else{
		while( (c=str[i]) != '\0' ){
			i+=1;
		}
	}
	return i+1;
}


int str_cp(char *from,char **to){
	printf("from:\t%s\tsize:%d\t\twill:to:\t%s\n",from,str_len(from),*to);
	int l=0,i=0;
        if(from==NULL){
		fprintf(stderr,"string to paste is NULL\n");
		*to=NULL;
		return 1;
	}
	else{ 
         	if( *to != NULL ){
			printf("to_not_NULL:%s,size:%d\n",*to,str_len(*to));
     //    		free(*to),(*to)=NULL;
         	}
		l=str_len(from);
		*to=(char *)calloc(l,sizeof(char));
		if((*to)==NULL){
			fprintf(stderr,"alloc mem error\n");
			exit(-1);
		}
		for(i=0;i<l;i++){
			(*to)[i]=from[i];
		}
	}
	return 0;
}
