#define YES 1
#define NO  0

/* 这些都仅仅是函数声明啊，为什么不加extern */

char *next_cms();
char** splitline(char *);	// 返回值是一个指针数组
void freelist(char **);

void *emalloc(size_t);
void *erealloc(void *,size_t);

int  execute(char **);
void fatal(char *,char *,int );

int process();




