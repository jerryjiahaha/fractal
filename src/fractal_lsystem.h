#ifndef _FRACTAL_LSYSTEM_H
#define _FRACTAL_LSYSTEM_H


typedef struct fractal {
	char   state;          //symbol for L-system
	long   double  value;  //extra info (optional)
	struct fractal *next;
}fractal;

typedef struct fractal_lsystem_conf{
	char *name;
        long double angle;
        char *begin;
	char *symbol;
	char **rule;
}fractal_lsystem_conf;


extern long double fractal_angle;
extern int fractal_time;
extern char fractal_symbol[];
extern char fractal_begin[];

extern char ** fractal_rule_build(char fractal_symbol[],...);
extern fractal* fractal_node(char state);
extern fractal * fractal_init(char fractal_begin[]);
extern int fractal_destroy(fractal *fractal_stack);
extern int fractal_transform(fractal *fractal_stack,char fractal_symbol[],char **fractal_rule);
extern int fractal_output(fractal* fractal_stack);
extern int fractal_draw(fractal *fractal_stack,int ftime);

#endif


//TO DO
/*
 dim   2/3
 name  $name
 angle $fractal_angle
 begin $char_begin
 paint $position  $value
 rule  $symbol (=) $transformation
*/
