enum states
{
	NEUTRAL,	// 在if的控制下
	WANT_THEN,	// 在then 的控制下
	THEN_BLOCK	// fi
};
enum results
{
	SUCCESS,
	FAIL
};

int is_control_command(char *s)
{
	return (strcmp(s,"if") == 0 || strcmp(s,"then") == 0 || strcmp(s,"fi") == 0); 
}

int do_control_command(char **args)
{
	char 	*cmd = args[0];
	int rv = -1;
	
	// 根据args[0]来的
	if(strcmp(cmd,"if") == 0)
	{
		if(if_state != NEUTRAL)
			rv = syn_err("if unexpected");
		else
		{
			last_stat = process(args+1);
			if_result =
		
		
	


