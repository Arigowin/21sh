#include "shell.h"
#include "libft.h"
#include <term.h>

int					fct_del(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- DEL ------\n");

	int					tmp;

	if (stline->copy.start != -1)
		return (FALSE);
	if ((stline->curs_x >= 2 && stline->curs_y == 0)
	|| (stline->curs_x >= 0 && stline->curs_y > 0))
	{
		tmp = (*pos);
		fct_right(str, pos, stline, history);
		if ((*pos) != tmp)
			fct_backspace(str, pos, stline, history);
	}
	return (TRUE);
}

int					fct_home(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- HOME ------\n");

	while (left_move_cdt(*pos, stline))
	{
		fct_left(str, pos, stline, history);
	}
	return (TRUE);
}

int					fct_end(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- END ------\n");

	while ((stline->copy.start == -1 && ((*pos))
				< (int)ft_strlen(*str)) || (stline->copy.start != -1
				&& ((*pos)) < (int)ft_strlen(*str) - 1))
	{
		fct_right(str, pos, stline, history);
	}
	return (TRUE);
}
