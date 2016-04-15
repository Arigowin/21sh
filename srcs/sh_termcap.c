#include "minishell.h"
#include <term.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int			init_term()
{
	struct termios	term;
	char			*nameterm;

	if ((nameterm = getenv("TERM")) == NULL)
		nameterm = "xterm-256color";
	if (tgetent(NULL, nameterm) == -1)
		return (-1);
	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(1, TCSADRAIN, &term) == -1)
		return (-1);
	return (0);
}

int			reset_term()
{
	struct termios term;
	
	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag |= (ICANON | ECHO);
	if (tcsetattr(1, TCSANOW, &term) == -1)
		return (-1);
	return (1);
}
