/**********************************************
 *  * filename     :       play2.c
 *   * auther       ：      renbin.guo
 *    * brief        ：      set tty into char-by-char and no-echo mode
 *     *                      (对play1.c的改进，不显示出来输入)
 *      *
 *       * note         :       better: timeout if user walks away
 *        *
 *         * history      :       2017-04-11      renbin.guo created
 *          * 
 *           * ***********************************************/

#include	<stdio.h>
#include	<termios.h>

#define	QUESTION	"Do you want another transaction"

main()
{
	int	response;

	tty_mode(0);				/* save mode */
	set_cr_noecho_mode();			/* set -icanon, -echo	*/
	response = get_response(QUESTION);	/* get some answer	*/
	tty_mode(1);				/* restore tty state	*/
	return response;
}

int get_response(char *question)
/*
 *  * purpose: ask a question and wait for a y/n answer
 *   *  method: use getchar and ignore non y/n answers
 *    * returns: 0=>yes, 1=>no
 *     */
{
	printf("%s (y/n)?", question);
	while(1){
		switch( getchar() ){
			case 'y': 
			case 'Y': return 0;
			case 'n': 
			case 'N': 
			case EOF: return 1;
		}
	}
}
set_cr_noecho_mode()
/* 
 *  * purpose: put file descriptor 0 into chr-by-chr mode and noecho mode
 *   *  method: use bits in termios
 *    */
{
	struct	termios	ttystate;

	tcgetattr( 0, &ttystate);		/* read curr. setting	*/
	ttystate.c_lflag    	&= ~ICANON;	/* no buffering		*/
	ttystate.c_lflag    	&= ~ECHO;	/* no echo either	*/
	ttystate.c_cc[VMIN]  	=  1;		/* get 1 char at a time	*/
	tcsetattr( 0 , TCSANOW, &ttystate);	/* install settings	*/
}

/* how == 0 => save current mode,  how == 1 => restore mode */
/*    renbin.guo added:   bug:    you must set_mode(0)first before you reset set_mode(1)*/

tty_mode(int how)

{
	static struct termios original_mode;
	if ( how == 0 )
		tcgetattr(0, &original_mode);
	else
		return tcsetattr(0, TCSANOW, &original_mode); 
}

