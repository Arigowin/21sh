#include "shell.h"
#include "libft.h"
#include <term.h>

int			fct_ctrl_up(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------------------ MOVE CTRL UP -------------------\n");

	(void)history;
	(void)str;
	if (stline->curs_y < 1)
		return (TRUE);
	stline->curs_y--;
	(*pos) -= (stline->win.ws_col);
	if ((*pos) < 0)
	{
		(*pos) = 0;
		if (stline->curs_x == 0)
			tputs(tgetstr("nd", NULL), 1, my_outc);
		tputs(tgetstr("nd", NULL), 1, my_outc);
		stline->curs_x = 2;
	}
	tputs(tgetstr("up", NULL), 1, my_outc);
	return (TRUE);
}

int			fct_ctrl_down(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------------------ MOVE CTRL DOWN -------------------\n");

	int		nb_ligne;
	int		i;

	(void)history;
	nb_ligne = (ft_strlen(*str) + PRT_LEN) / stline->win.ws_col;
	if (nb_ligne <= stline->curs_y)
		return (TRUE);
	stline->curs_y++;
	if ((int)ft_strlen(*str) >= ((*pos) + stline->win.ws_col))
	{
		(*pos) += stline->win.ws_col;
		i = stline->curs_x;
	}
	else
	{
		(*pos) = ft_strlen(*str);
		i = ((*pos) - (stline->win.ws_col * nb_ligne)) + PRT_LEN;
		stline->curs_x = i;
	}
	tputs(tgetstr("do", NULL), 1, my_outc);
	while (i-- > 0)
		tputs(tgetstr("nd", NULL), 1, my_outc);
	return (TRUE);
}
