#include <term.h>
#include <termios.h>
#include "shell.h"
#include "libft.h"

int					my_outc(int c)
{
	ft_putchar(c);
	return (0);
}

static int			start_init_term(void)
{
	char				*term_name;

	term_name = NULL;
	if ((term_name = getenv("TERM")) == NULL)
		term_name = "xterm";
	if (tgetent(NULL, term_name) == ERROR)
		/* RET: error EXIT: true MSG: "Could not access the termcap data base." */
		return (ERROR);
	return (TRUE);
}

int					init_term(int full_init)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- INIT TERM ------", 2);

	struct termios		term;

	if (full_init == TRUE)
	{
		if (start_init_term() == ERROR)
			return (ERROR);
	}
	if (tcgetattr(0, &term) == ERROR)
		/* RET: error EXIT: true MSG: "Could not access the termcap data base." */
		return (ERROR);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term) == ERROR)
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
	if (tcgetattr(0, &term) == ERROR)
		/* RET: error EXIT: true MSG: "Could not access the termcap data base." */
		return (ERROR);
	term.c_lflag |= (ICANON | ECHO);
	if (tcsetattr(0, TCSANOW, &term) == ERROR)
		/* RET: error EXIT: true MSG: "Could not access the termcap data base." */
		return (ERROR);
	return (TRUE);
}
