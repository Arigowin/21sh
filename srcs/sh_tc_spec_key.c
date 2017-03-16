#include <term.h>
#include "shell.h"
#include "libft.h"

int					fct_del(char **str, int *pos, t_line *stline,
					t_history **history)
{
	int					tmp;

	if (stline->copy.start != -1)
		return (FALSE);
	if ((stline->curs_y == 0 && stline->curs_x >= 2)
	|| (stline->curs_y > 0 && stline->curs_x >= 0))
	{
		tmp = (*pos);
		fct_right(str, pos, stline, history);
		if ((*pos) != tmp)
			fct_backspace(str, pos, stline, history);
	}
	return (TRUE);
}

int					fct_home(char **str, int *pos, t_line *stline,
					t_history **history)
{
	while (*pos > 0 && ((stline->curs_y == 0 && stline->curs_x > 2)
	|| stline->curs_y > 0))
	{
		fct_left(str, pos, stline, history);
	}
	return (TRUE);
}

int					fct_end(char **str, int *pos, t_line *stline,
					t_history **history)
{
	while ((stline->copy.start == -1 && ((*pos)) < ft_strlen(*str))
	|| (stline->copy.start != -1 && ((*pos)) < ft_strlen(*str) - 1))
	{
		fct_right(str, pos, stline, history);
	}
	return (TRUE);
}
