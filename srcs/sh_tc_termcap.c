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
	if ((term_name = get_env("TERM")) == NULL)
		term_name = "xterm";
	tgetent(NULL, term_name);
	ft_strdel(&term_name);
	//if (tgetent(NULL, term_name) == ERROR) ->quand je fais ca, ca hurle de partout pour les termcap
	//	return (sh_error(TRUE, 0, NULL, NULL));
	return (TRUE);
}

int					init_term(int full_init)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- INIT TERM ------", 2);

	struct termios		term;
	char				*term_env;

	if ((term_env = get_env("TERM")) == NULL)
		return (sh_error(TRUE, 0, NULL, NULL));
	if (full_init == TRUE)
	{
		if (start_init_term() == ERROR)
			return (ERROR);
	}
	tcgetattr(0, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &term);// point to uninitialize byte selon valgrind
	ft_strdel(&term_env);
	return (TRUE);
}

int					reset_term(void)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- RESET TERM ------", 2);

	struct termios		term;

	tputs(tgetstr("ve", NULL), 1, my_outc);
	tcgetattr(0, &term);
//	if (tcgetattr(0, &term) == ERROR)
//		return (sh_error(TRUE, 0, NULL, NULL));
	term.c_lflag |= (ICANON | ECHO);
	tcsetattr(0, TCSANOW, &term);// point to uninitialize byte selon valgrind

	return (TRUE);
}
