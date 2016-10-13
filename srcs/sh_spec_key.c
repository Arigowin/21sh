#include "shell.h"
#include "libft.h"
#include <term.h>

static void			del(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- DEL ------\n");
	int		tmp;

	tmp = stline->curs_x;
	move(RIGHT, stline);
	if (stline->curs_x != tmp)
		backspace(stline);
}

static void			home(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- HOME ------\n");
	while (stline->curs_x > 3)
	{
		tputs(tgetstr("le", NULL), 1, my_outc);
		stline->curs_x--;
	}
}

static void			end(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- END ------\n");
	while ((stline->curs_x - 3) < (int)ft_strlen(stline->line))
	{
		tputs(tgetstr("nd", NULL), 1, my_outc);
		stline->curs_x++;
	}
}

int					spec_key(int key, t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- SPEC KEY ------\n");
	if (key == DEL)
		del(stline);
	else if (key == HOME)
		home(stline);
	else if (key == END)
		end(stline);
	else
		return (-1);
	return (0);
}
