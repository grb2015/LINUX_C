#include<stdio.h>
char c;
int main(){
    int repeat=10;
    FILE * fp2 = fopen("file3", "w");//打开输出文件
    if (fp2==NULL) {//若打开文件失败则退出
        puts("不能打开文件！");
        return 0;
    }
  //  while((c=getchar()) != EOF)//从键盘读取一个字符
    while(repeat-->0)
    {
	for(c='0';c<='9';c++)
	{
    		fputc(c,fp2);//向输出文件写入一个字符
    		fputc('\n',fp2);//向输出文件写入一个字符
	}
    }
    fclose(fp2);//关闭输出文件，相当于保存
    return 0;
}
