#include "shell.h"
#include <term.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int			init_term(void)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- INIT TERM ------\n");
	struct termios	term;
	char			*term_name;
	int status = 0;

	if ((term_name = getenv("TERM")) == NULL)
		term_name = "xterm";
	printf("OK1 %s\n", term_name);
//	if (tgetent(NULL, term_name) == -1)
//		return (-1);
	status = tgetent(NULL, term_name);
	printf("OK2 %d\n", status);
	if (tcgetattr(0, &term) == -1)
		return (-1);
	printf("OK3\n");
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
		return (-1);
	printf("OK4\n");
	tputs(tgetstr("le", NULL), 1, my_outc);
	printf("OK5\n");
	return (0);
}

int			reset_term(void)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- RESET TERM ------\n");
	struct termios	term;

	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag |= (ICANON | ECHO);
	if (tcsetattr(0, TCSANOW, &term) == -1)
		return (-1);
	return (1);
}
