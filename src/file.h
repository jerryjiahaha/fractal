#ifndef _FILE_H_
#define _FILE_H_

#include<stdio.h>

typedef struct file_read_word{   //内容节点
	char *word;
	int  pos;
	struct file_read_word *next;
	struct file_read_word *previous;
}file_read_word;

typedef struct file_read_stack{  //存储读进语句的栈,其实是双向链表
	file_read_word *ftop;
	file_read_word *fbase;
	int length;
}file_read_stack;

file_read_word *read_word_node(char *word,int pos);
int read_stack_destroy(file_read_stack *fstack);
int read_stack_pushr(file_read_stack *fstack,file_read_word *fnode);
file_read_stack *read_stack_init(int len);
int file_read_stack_output(file_read_stack *fstack);
file_read_stack * file_read_build_stack(char *filename);

/*aim:去掉注释,分解语句,每次调用后文件指针处于注释行回车后或正文前,则连续两次调用该函数文件指针位置不会变*/
int file_read_comment(FILE *fp,int *cstat,int *wcount); //cstat,注释标志位;wcount,处理字符计数
int file_byte_count(FILE *fp);

#endif
