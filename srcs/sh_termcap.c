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

	if ((term_name = getenv("TERM")) == NULL)
		term_name = "xterm";
	if (tgetent(NULL, term_name) == -1)
		return (-1);
	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
		return (-1);
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
