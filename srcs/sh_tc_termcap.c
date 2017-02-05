#include <term.h>
#include <termios.h>
#include "shell.h"
#include "libft.h"

int					my_outc(int c)
{
	ft_putchar(c);
	return (0);
}

int					init_term(void)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- INIT TERM ------", 2);

	struct termios		term;
	char				*term_name;

	if ((term_name = getenv("TERM")) == NULL)
		term_name = "xterm";
	if (tgetent(NULL, term_name) == ERROR)
		/* RET: error EXIT: true MSG: "Could not access the termcap data base." */
		return (ERROR);
	if (tcgetattr(1, &term) == ERROR)
		/* RET: error EXIT: true MSG: "Could not access the termcap data base." */
		return (ERROR);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(1, TCSADRAIN, &term) == ERROR)
		/* RET: error EXIT: true MSG: "Could not access the termcap data base." */
		return (ERROR);
	return (TRUE);
}

int					reset_term(void)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- RESET TERM ------", 2);

	struct termios		term;

	tputs(tgetstr("ve", NULL), 1, my_outc);
	if (tcgetattr(1, &term) == ERROR)
		/* RET: error EXIT: true MSG: "Could not access the termcap data base." */
		return (ERROR);
	term.c_lflag |= (ICANON | ECHO);
	if (tcsetattr(1, TCSANOW, &term) == ERROR)
		/* RET: error EXIT: true MSG: "Could not access the termcap data base." */
		return (ERROR);
	return (TRUE);
}
