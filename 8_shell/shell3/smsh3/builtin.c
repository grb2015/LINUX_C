#include<string.h>
#include<ctype.h>
#include"smsh.h"
#include"varlib.h"

int assing(char *);
int okname(char *);

int builtin_command(char **args,int *resultp)
{

