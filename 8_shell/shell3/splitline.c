/******************************************
 * 	brief:	get next command
 * 	param:	
 * 		[in]  char*prompt: 输入的提示符，这里为"> "
 * 		[in]  FILE *fp   : fd文件，存放命令字符
 */
char * next_cmd(char*prompt,FILE *fp)
{
	char *buf;
	int bufsapce = 0;
	int pos = 0;
	int c;
	
	printf("%s",prompt);
	// 我们这里的fp是stdin,从输入中获取,如果不为文件结束符
	while( (c=getc(fp)) != EOF)
	{
		// 如果buf满了
		if(pos+1 >= bufspace)
		{
			if(bufspace == 0)
				buf = emalloc(BUFSIZ);
			else	// 重新分配buf的长度，增加BUFSIZ个字节
				buf = erealloc(buf,bufspace+BUFSIZ);
			bufspace += BUFSIZ;
		}
		// 如果输入的是换行符,代表执行当前行
		if(c == '\n')
			break;
		buf[pos++] = c;	 //将输入的字符存在buf中
	}
	// 如果fd中什么内容也没有,即第一个字符为EOF,这种情况返回NULL
	if(c == EOF && pos == 0)
		return NULL;
	buf[pos] = '\0';
	return buf;
}
	
		
		
			
	

