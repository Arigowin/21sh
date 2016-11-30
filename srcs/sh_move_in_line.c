#include "shell.h"
#include "libft.h"
#include <term.h>

int			fct_left(t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE LEFT ------\n");

	(void)history;
	if ((stline->pos_line > 0 && stline->quote != 0 && stline->curs_x > 2)
			|| (stline->pos_line > 0 && stline->quote == 0))
	{
		if (stline->copy.start != -1 && stline->pos_line > stline->copy.start)
			del_in_copy(stline, LEFT);
		(stline->curs_x)--;
		if (stline->curs_x < 0 && stline->curs_y > 0)
		{
			tputs(tgetstr("up", NULL), 1, my_outc);
			stline->curs_x = -1;
			(stline->curs_y)--;
			while (++(stline->curs_x) < stline->win.ws_col)
				tputs(tgetstr("nd", NULL), 1, my_outc);
			tputs(tgetstr("nd", NULL), 1, my_outc);
			(stline->curs_x)--;
		}
		else
			tputs(tgetstr("le", NULL), 1, my_outc);
		(stline->pos_line)--;
		if (stline->copy.start != -1 && stline->pos_line < stline->copy.start)
			add_in_copy(stline, LEFT);
	}
	return (TRUE);
}

int			fct_right(t_line *stline, t_history **history)
{
//	if (DEBUG_TERMCAPS == 1)
//		printf("------- MOVE RIGHT ------\n");

	(void)history;
	if ((stline->copy.start == -1 && (stline->pos_line)
		< (int)ft_strlen(stline->line)) || (stline->copy.start != -1
		&& (stline->pos_line) < (int)ft_strlen(stline->line) - 1))
	{
		if (stline->copy.start != -1 && stline->pos_line < stline->copy.start)
			del_in_copy(stline, RIGHT);
		if (stline->curs_x >= stline->win.ws_col - 1)
		{
			(stline->curs_x) = 0;
			(stline->curs_y)++;
			tputs(tgetstr("cr", NULL), 1, my_outc);
			tputs(tgetstr("do", NULL), 1, my_outc);
		}
		else
		{
			(stline->curs_x)++;
			tputs(tgetstr("nd", NULL), 1, my_outc);
		}
		(stline->pos_line)++;
		if (stline->copy.start != -1 && stline->pos_line > stline->copy.start)
			add_in_copy(stline, RIGHT);
	}
	return (TRUE);
}

int			fct_ctrl_left(t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE WORD LEFT ------\n");
	int		x;

	(void)history;
	if ((stline->pos_line > 0 && stline->quote != 0 && stline->curs_x > 2)
			|| (stline->pos_line > 0 && stline->quote == 0))
		fct_left(stline, history);
	x = stline->pos_line;
	while ((x > 0 && stline->quote != 0 && stline->curs_x > 2)
			|| (x > 0 && stline->quote == 0))
	{
		if ((stline->line[x - 1] == ' ' || stline->line[x - 1] == '\n')
				&& stline->line[x] != ' ')
			break ;
		fct_left(stline, history);
		x = stline->pos_line;
	}
	return (TRUE);
}

int			fct_ctrl_right(t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE WORD RIGHT ------\n");
	int		x;

	(void)history;
	if ((stline->pos_line) < (int)ft_strlen(stline->line))
		fct_right(stline, history);

	x = stline->pos_line;
	while ((x) < (int)ft_strlen(stline->line))
	{
		if (stline->line[x - 1] == ' ' && stline->line[x] != ' ')
			break ;
		fct_right(stline, history);
		x = stline->pos_line;
	}
	return (TRUE);
}
