#ifndef _STR_CALC_H_
#define _STR_CALC_H_

long double str_to_longdouble(char *input,int *pos);
long double str_calc(char *input,int *pos,int *pairflag);

int str_len(char *str);
int str_cp(char *from,char **to);

#endif
