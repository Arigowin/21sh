#include "shell.h"
#include "libft.h"
#include <term.h>

int					left_move_cdt(int pos, t_line *stline)
{
	return (pos > 0
		&& ((stline->quote != 0 && stline->curs_y == 0 && stline->curs_x > 2)
			|| (stline->quote != 0 && stline->curs_y > 0)
			|| (stline->quote == 0 && stline->curs_y == 0 && stline->curs_x > 2)
			|| (stline->quote == 0 && stline->curs_y > 0)));
}

int					fct_left(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE LEFT ------\n");

	(void)history;
	if (left_move_cdt(*pos, stline))
	{
		if (stline->copy.start != -1 && (*pos) > stline->copy.start)
			del_in_copy(str, pos, stline, LEFT);
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
		((*pos))--;
		if (stline->copy.start != -1 && (*pos) < stline->copy.start)
			add_in_copy(str, pos, stline, LEFT);
	}
	return (TRUE);
}

int					fct_right(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE RIGHT ------\n");

	(void)history;
	if ((stline->copy.start == -1 && ((*pos))
		< (int)ft_strlen(*str)) || (stline->copy.start != -1
		&& ((*pos)) < (int)ft_strlen(*str) - 1))
	{
		if (stline->copy.start != -1 && (*pos) < stline->copy.start)
			del_in_copy(str, pos, stline, RIGHT);
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
		((*pos))++;
		if (stline->copy.start != -1 && (*pos) > stline->copy.start)
			add_in_copy(str, pos, stline, RIGHT);
	}
	return (TRUE);
}

int					fct_ctrl_left(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE WORD LEFT ------\n");

	int					x;

	(void)history;
	if (left_move_cdt(*pos, stline))
		fct_left(str, pos, stline, history);
	x = (*pos);
	while (left_move_cdt(x, stline))
	{
		if (((*str)[x - 1] == ' ' || (*str)[x - 1] == '\n') && (*str)[x] != ' ')
			break ;
		fct_left(str, pos, stline, history);
		x = (*pos);
	}
	return (TRUE);
}

int					fct_ctrl_right(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE WORD RIGHT ------\n");

	int					x;

	(void)history;
	if (((*pos)) < (int)ft_strlen(*str))
		fct_right(str, pos, stline, history);

	x = (*pos);
	while ((x) < (int)ft_strlen(*str))
	{
		if ((*str)[x - 1] == ' ' && (*str)[x] != ' ')
			break ;
		fct_right(str, pos, stline, history);
		x = (*pos);
	}
	return (TRUE);
}
