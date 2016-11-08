#include "shell.h"
#include "libft.h"
#include <term.h>

static int	move_ctrl_up(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------------------ MOVE CTRL UP -------------------\n");
	if (stline->curs_y < 1)
		return (FALSE);
	stline->curs_y--;
	stline->pos_line -= (stline->win.ws_col);
	if (stline->pos_line < 0)
	{
		stline->pos_line = 0;

		if (stline->curs_x == 0)
			tputs(tgetstr("nd", NULL), 1, my_outc);
		tputs(tgetstr("nd", NULL), 1, my_outc);
		stline->curs_x = 2;
	}
	tputs(tgetstr("up", NULL), 1, my_outc);
	return (TRUE);
}

static int	move_ctrl_down(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------------------ MOVE CTRL DOWN -------------------\n");
	int		nb_ligne;
	int		i;

	nb_ligne = (ft_strlen(stline->line) + PRT_LEN) - 1;
	nb_ligne = nb_ligne / stline->win.ws_col;
	if (nb_ligne <= stline->curs_y)
		return (FALSE);
	stline->curs_y++;
	if ((int)ft_strlen(stline->line) >= (stline->pos_line + stline->win.ws_col))
	{
		stline->pos_line += stline->win.ws_col;
		i = stline->curs_x;
	}
	else
	{
		stline->pos_line = ft_strlen(stline->line);
		i = (stline->pos_line - (stline->win.ws_col * nb_ligne))+ 2;
		stline->curs_x = i;
	}
	tputs(tgetstr("do", NULL), 1, my_outc);
	while (i > 0)
	{
		tputs(tgetstr("nd", NULL), 1, my_outc);
		i--;
	}
	return (TRUE);
}

int			move_up_down(int key, t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------------------ MOVE UP DOWN -------------------\n");
	if (key == CTRL_UP)
		move_ctrl_up(stline);
	else if (key == CTRL_DOWN)
		move_ctrl_down(stline);
	return (TRUE);
}
