#include "shell.h"
#include "libft.h"
#include <term.h>

int			fct_del(t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- DEL ------\n");

	int		tmp;

	if ((stline->pos_line >= 0 && stline->quote != 0 && stline->curs_x > 2)
	 || (stline->pos_line >= 0 && stline->quote == 0))
	{
		tmp = stline->pos_line;
		fct_right(stline, history);
		if (stline->pos_line != tmp)
			fct_backspace(stline, history);
	}
}

int			fct_home(t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- HOME ------\n");
	while ((stline->pos_line > 0 && stline->quote != 0 && stline->curs_x > 2)
	 || (stline->pos_line > 0 && stline->quote == 0))
		fct_left(stline, history);
}

int			fct_end(t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- END ------\n");
	while ((stline->pos_line) < (int)ft_strlen(stline->line))
		fct_right(stline, history);
}
