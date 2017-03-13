#include <term.h>
#include "shell.h"
#include "libft.h"

static int			delete_char(char **str, int *pos, int i, t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- DELETE_CHAR ------", 2);

	if (stline->curs_x < 0 && stline->curs_y > 0)
	{
		(stline->curs_x)--;
		(stline->curs_y)--;
		tputs(tgetstr("up", NULL), 1, my_outc);
		while (stline->curs_x++ < stline->win.ws_col)
			tputs(tgetstr("nd", NULL), 1, my_outc);
		tputs(tgetstr("dc", NULL), 1, my_outc);
		(*str)[--(*pos)] = '\0';
	}
	else
	{
		i = (*pos);
		fct_end(str, pos, stline, NULL);
		while (i <= (*pos))
		{
			tputs(tgetstr("dc", NULL), 1, my_outc);
			fct_left(str, pos, stline, NULL);
			tputs(tgetstr("dc", NULL), 1, my_outc);
			(*str)[(*pos)] = '\0';
		}
	}
	return (TRUE);
}

static int			delete_in_quote(char **str, int *pos, t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- HANDLE DELETE QUOTE ------", 2);

	if (*pos && *pos > 0 && (*str)[*pos - 1] == stline->quote)
		stline->quote = 0;
	else if (*pos && *pos > 0 && stline->quote == 0
	&& ((*str)[*pos - 1] == QUOTE || (*str)[*pos - 1] == DQUOTE))
		stline->quote = (*str)[*pos - 1];
	return (TRUE);
}

int					fct_backspace(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- BACKSPACE ------", 2);

	char				*eol;
	int					i;
	int					save_pos;

	save_pos = 0;
	if (stline->copy.start != -1)
		return (FALSE);
	delete_in_quote(str, pos, stline);
	eol = ft_strsub(*str, (*pos), ft_strlen(*str));
	if (*pos > 0	&& ((stline->curs_y == 0 && stline->curs_x > 2)
	|| stline->curs_y > 0))
	{
		i = 0;
		save_pos = *pos;
		delete_char(str, pos, i, stline);
		while (eol && eol[i])
			fct_insert(str, pos, eol[i++], stline);
		while ((*pos) >= save_pos)
			fct_left(str, pos, stline, history);
		if (stline->curs_x == stline->win.ws_col)
			tputs(tgetstr("nd", NULL), 1, my_outc);
	}
	ft_strdel(&eol);
	return (TRUE);
}
