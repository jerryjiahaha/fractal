#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"fractal_lsystem.h"

int fractal_time=4;
//long double fractal_angle=M_PI/4;

fractal* fractal_node(char state){    //build a fractal node
	fractal *ftmp=NULL;
	ftmp=(fractal *)calloc(1,sizeof(fractal));
	if(ftmp==NULL){exit(-1);}
	ftmp->state=state;
	ftmp->next=NULL;
	return ftmp;
}

fractal * fractal_init(char *fractal_begin){     //init stack for L-system
	printf("debug3\n");
	if(fractal_begin==NULL){return NULL;}
	int flength=strlen(fractal_begin);
	int i=flength-1;
	fractal *fbase=fractal_node('$');
	fractal *fractal_stack=NULL;
	fractal_stack=fractal_node(fractal_begin[i]);
	i--;
	fractal_stack->next=fbase;
	fractal *ftmp=NULL;
	while(i>=0){
		ftmp=fractal_node(fractal_begin[i]);
		ftmp->next=fractal_stack;
		fractal_stack=ftmp;
                i--;
	}

	return fractal_stack;
}

int fractal_destroy(fractal *fractal_stack){  //destroy stack
	if(fractal_stack==NULL){return 0;}
	fractal *fpre=NULL;
	while(fractal_stack->next != NULL){
		fpre=fractal_stack;
		fractal_stack=fractal_stack->next;
		free(fpre);
	}
	free(fractal_stack);
	fractal_stack=fpre=NULL;
	return 0;
}


int fractal_transform(fractal *fractal_stack,char *fractal_symbol,char **fractal_rule){   //produce L-system fractal
	int i,j;
	int state=0;
	int symlen=strlen(fractal_symbol);
        int rulen;
	fractal *fpre=NULL;
	fractal	*ftmp=NULL;
	fractal	*frecord=NULL;
	while(fractal_stack != NULL){
		frecord=fractal_stack->next;    //逐个出栈,匹配规则
		state=0;
		for(i=0;i<symlen && state==0;i++){
			if(fractal_stack->state==fractal_symbol[i]){    //与规则表中可替换字符进行比较
				state=1;
				rulen=strlen(fractal_rule[i]);
				fpre=fractal_stack->next;
				for(j=rulen-1;j>0;j--){                 //按匹配规则进行迭代,链表插入操作
					ftmp=fractal_node(fractal_rule[i][j]);
					ftmp->next=fpre;
					fpre=ftmp;
				}
				fractal_stack->state=fractal_rule[i][0];
				fractal_stack->next=fpre;
			}
		}
		fractal_stack=frecord;
	}
	fpre=ftmp=frecord=NULL;
	return 0;
}


int fractal_output(fractal* fractal_stack){   //遍历
	while(fractal_stack != NULL){
		printf("%c",fractal_stack->state);
		fractal_stack=fractal_stack->next;
	}
	printf("\n");
	return 0;
}

typedef struct fractal_pos{         //posititon or angle of fractal points
	long double value;
	struct fractal_pos* next;
}fractal_pos;

fractal_pos * fractal_pos_node(long double value){   //build node
	fractal_pos * fnode=NULL;
	fnode=(fractal_pos*)calloc(1,sizeof(fractal_pos));
	if(fnode==NULL){exit(-1);}
	fnode->next=NULL;
	fnode->value=value;
        return fnode;
}

fractal_pos * fractal_draw_push(fractal_pos *i){    //push
	fractal_pos *fnew;
	fnew=fractal_pos_node(i->value);
	fnew->next=i;
	return fnew;
}

fractal_pos * fractal_draw_pop(fractal_pos *i){     //pop
	if(i==NULL || i->next==NULL){
		fprintf(stderr,"stack  error\n num of ']' may not bigger than '[''s \n");
		exit(-3);
	}
	fractal_pos *fnew=NULL;
	fnew=i->next;
        free(i);
	return fnew;
}

int fractal_draw(fractal *fractal_stack,long double fractal_angle,int ftime){   //draw fractal
	fractal_pos *x,*y,*the;
	x=fractal_pos_node(0);
	y=fractal_pos_node(0);
	the=fractal_pos_node(0);
	long double cir=2*M_PI;
	FILE *fout=NULL;
	char *filename;
	filename=(char*)calloc(30,sizeof(char));
	sprintf(filename,"frac_%d.dat",ftime);
	fout=fopen(filename,"w");
	if(fout==NULL){exit(-2);}
	long double c,s;
	int i,n=10;
	fprintf(fout,"%Lf\t%Lf\n",x->value,y->value);
        while(fractal_stack != NULL){
                switch(fractal_stack->state){
			case 'F' :{      //draw forward
			                 c=cos(the->value)/n,s=sin(the->value)/n;
                                         for(i=1;i<=n;i++){
		               	                 fprintf(fout,"%Lf\t%Lf\n",x->value+c*i,y->value+s*i);
			                 }
                                         x->value+=c*n,y->value+=s*n;
		        		 break;
		        }
                        case 'G' :{      //just move forward
	                   		 x->value+=cos(the->value);
			                 y->value+=sin(the->value);
			                 fprintf(fout,"%Lf\t%Lf\n",x->value,y->value);
					 break;
			}
		        case '+' :{      //turn left $angle
			                 the->value+=fractal_angle;
			                 if(the->value>cir){the->value-=cir;}
			                 else if(the->value<0){the->value+=cir;}
					 break;
		        }
		        case '-' :{      //turn right $angle
			                 the->value-=fractal_angle;
			                 if(the->value>cir){the->value-=cir;}
			                 else if(the->value<0){the->value+=cir;}
					 break;
		        }
		        case '|' :{      //turn back
					 the->value+=M_PI;
			                 if(the->value>cir){the->value-=cir;}
			                 else if(the->value<0){the->value+=cir;}
					 break;
		        }
                        case '[' :{      //push,save current state
                                         the=fractal_draw_push(the);
                                         x=fractal_draw_push(x);
                                         y=fractal_draw_push(y);
                                         break;
		        }
                        case ']' :{      //pop,restore state
                                         the=fractal_draw_pop(the);
                                         x=fractal_draw_pop(x);
                                         y=fractal_draw_pop(y);
                                         break;
                        }
		}
		fractal_stack=fractal_stack->next;
	}
	if(the != NULL){
		free(the),free(x),free(y);
		x=y=the=NULL;
	}
        fclose(fout);
	fout=NULL;
	free(filename);
	filename=NULL;
	return 0;
}


