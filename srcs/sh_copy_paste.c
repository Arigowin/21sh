#include <term.h>
#include "shell.h"
#include "libft.h"

int					fct_copy(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- COPY ------\n");

	if (stline->copy.start == -1)
		return (FALSE);
	tputs(tgetstr("me", NULL), 1, my_outc);
	stline->copy.start = -1;
	hide_highlight(str, pos, stline, history);
	return (TRUE);
}

int					fct_paste(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- PASTE ------\n");

	int					i;

	i = 0;
	(void)history;
	if (!stline->copy.cpy || stline->copy.start != -1)
		return (FALSE);
	while (stline->copy.cpy[i])
	{
		fct_insert(stline->copy.cpy[i], str, pos, stline);
		i++;
	}
	tputs(tgetstr("me", NULL), 1, my_outc);
	return (TRUE);
}

int					fct_cut(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- PASTE ------\n");

	int					curs_start;
	int					curs_end;

	curs_start = stline->copy.start;
	curs_end = (*pos);
	if (stline->copy.start == -1)
		return (FALSE);
	tputs(tgetstr("me", NULL), 1, my_outc);
	stline->copy.start = -1;
	if (curs_end > curs_start)
	{
		fct_del(str, pos, stline, history);
		while (--curs_end >= curs_start)
			fct_backspace(str, pos, stline, history);
	}
	else
	{
		while (curs_end <= curs_start)
		{
			fct_del(str, pos, stline, history);
			curs_start--;
		}
	}
	tputs(tgetstr("ve", NULL), 1, my_outc);
	return (TRUE);
}
