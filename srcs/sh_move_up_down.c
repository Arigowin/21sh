#include "shell.h"
#include "libft.h"
#include <term.h>

int			fct_ctrl_up(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------------------ MOVE CTRL UP -------------------\n");

	char			*tmp;
	int				rel_pos;

	(void)history;
	(void)str;
	tmp = ft_strrchr(*str, '\n');
	if (stline->curs_y < 1)
		return (TRUE);
	stline->curs_y--;
	(*pos) -= (stline->win.ws_col);
	rel_pos = (tmp != NULL ? *pos - (ft_strlen(*str) - ft_strlen(tmp)) : *pos);
	if (rel_pos < 0)
	{
		(*pos) = (tmp != NULL ? (ft_strlen(*str) - ft_strlen(tmp)) + 1 : 0);
		if (stline->curs_x == 0)
			tputs(tgetstr("nd", NULL), 1, my_outc);
		tputs(tgetstr("nd", NULL), 1, my_outc);
		stline->curs_x = 2;
	}
	tputs(tgetstr("up", NULL), 1, my_outc);
	return (TRUE);
}

static int			down_term(int i, t_line *stline)
{
	tputs(tgetstr("do", NULL), 1, my_outc);
	stline->curs_y++;
	while (i-- > 0)
		tputs(tgetstr("nd", NULL), 1, my_outc);
	return (TRUE);
}

int					fct_ctrl_down(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------------------ MOVE CTRL DOWN -------------------\n");

	int			nb_ligne;
	int			last_line;
	int			rel_pos;
	char		*tmp;

	(void)history;
	tmp = ft_strrchr(*str, '\n');
	last_line = (tmp != NULL ? ft_strlen(tmp + 1) : ft_strlen(*str));
	rel_pos = (tmp != NULL ? *pos - (ft_strlen(*str) - ft_strlen(tmp)) : *pos);
	nb_ligne = (last_line + PRT_LEN) / stline->win.ws_col;
	if (stline->curs_y >= nb_ligne )
		return (TRUE);
	if (last_line >= (rel_pos + stline->win.ws_col))
	{
		(*pos) += stline->win.ws_col;
		rel_pos = stline->curs_x;
	}
	else
	{
		(*pos) = ft_strlen(*str);
		rel_pos = (tmp ? *pos - (ft_strlen(*str) - ft_strlen(tmp + 1)) : *pos);
		rel_pos = (rel_pos - (stline->win.ws_col * nb_ligne)) + PRT_LEN;
		stline->curs_x = rel_pos;
	}
	return (down_term(rel_pos, stline));
}
