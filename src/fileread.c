#include"file.h"
#include<stdio.h>
#include<stdlib.h>

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
int file_read_comment(FILE *fp,int *cstat,int *wcount){  //cstat,注释标志位;wcount,处理字符计数
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


int read_stack_destroy(file_read_stack *fstack){ //销毁
	file_read_word *tmp=NULL;
	while(fstack->ftop != fstack->fbase){
		tmp=(fstack->ftop)->next;
		if( (fstack->ftop)->word != NULL ){
			free((fstack->ftop)->word),(fstack->ftop)->word=NULL;
		}
		free(fstack->ftop);
		fstack->ftop=tmp;
	}
	free(fstack->fbase);
        free(fstack);
	tmp=NULL;
	return 0;
}

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

int strcp(char *from,char **to){;
	int l=0,i=0;
        if(from==NULL){
		*to=NULL;
		return 1;
	}
	else{ 
		l=str_len(from);
		*to=(char *)calloc(l,sizeof(char));
		for(i=0;i<l;i++){
			(*to)[i]=from[i];
		}
	}
	return 0;
}


file_read_word *read_word_node(char *word,int pos){ //read_word_node 记录一个字符串的节点
	file_read_word * fword=NULL;
	fword=(file_read_word*)calloc(1,sizeof(file_read_word));
	if(fword==NULL){exit(-1);}
//	fword->word=(char *)calloc(sizeof(word),sizeof(char));
        if(word!=NULL){		
	        strcp(word,&(fword->word));
	}
//	fword->word=word,
	fword->pos=pos;
	fword->next=fword->previous=NULL;
	return fword;
}


/*从基地址插入节点*/
//read_stack_pushr(frac_stack,read_word_node(ftmp,wlen))
int read_stack_pushr(file_read_stack *fstack,file_read_word *fnode){
	if( fstack->fbase != fstack->ftop ){
		fnode->next=fstack->fbase;                  //fnode->fbase
		fnode->previous=(fstack->fbase)->previous;  //*<-fnode = *<-fbase
		(fstack->fbase)->previous=fnode;            //fnode<-fbase
		(fnode->previous)->next=fnode;              //*->fnode
	}
	else{    //首节点
		fnode->next=fstack->fbase;
		(fstack->fbase)->previous=fnode;
		fstack->ftop=fnode;
	} 
	return 0;
}

file_read_stack *read_stack_init(int len){
	file_read_stack * fstack=NULL;
	fstack=(file_read_stack*)calloc(1,sizeof(file_read_stack));
	if(fstack==NULL){
		printf("alloc err!\n");
		exit(-1);
	}
	fstack->length=len;
	fstack->fbase=read_word_node(NULL,0);
	fstack->ftop=fstack->fbase;
        int i=0;
	for(i=1;i<=len;i++){                
        	read_stack_pushr(fstack,read_word_node(NULL,i));
	}
	return fstack;
}

int file_read_stack_output(file_read_stack *fstack){
	printf("\n");
        file_read_word *cur=NULL;
	cur=fstack->ftop;
	while(cur != fstack->fbase && cur!=NULL){
		printf(" %d\t%s\n",cur->pos,cur->word);
		cur=cur->next;
	}
	return 0;
}



file_read_stack * file_read_build_stack(char *filename){
        file_read_stack *frac_stack=NULL;
        frac_stack=read_stack_init(0);        
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
			wlen+=1;
		}
		if(wlen!=0){                        
			ftmp=(char *)realloc(ftmp,(wlen+1)*sizeof(char));
			if(ftmp==NULL){exit(-1);}
			if(cstat==0){				
                                fseek(fp,-wlen-1,SEEK_CUR);
			}
			else{
				fseek(fp,-wlen-wcount,SEEK_CUR);
			}
			for(j=0;j<wlen;j++){
				ftmp[j]=fgetc(fp);
			}
			ftmp[wlen]='\0';
	//	        printf("wl:%d\tftmp:%s\t",wlen,ftmp);
			read_stack_pushr(frac_stack,read_word_node(ftmp,wlen));
			fseek(fp,wcount,SEEK_CUR);
		}
	}
	free(ftmp);
	ftmp=NULL;
	fclose(fp),fp=NULL;
	return frac_stack;
}


