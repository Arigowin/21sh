#include <term.h>
#include "shell.h"
#include "libft.h"

int					left_move_cdt(int pos, t_line *stline)
{
	return (pos > 0
		&& ((stline->quote != 0 && stline->curs_y == 0 && stline->curs_x > 2)
			|| (stline->quote != 0 && stline->curs_y > 0)
			|| (stline->quote == 0 && stline->curs_y == 0 && stline->curs_x > 2)
			|| (stline->quote == 0 && stline->curs_y > 0)));
}

static int			multi_left(char **str, int *pos, t_line *stline)
{
	int					nb;
	char				*tmp;
	char				*chr;

	if ((*str)[*pos - 1] == '\n')
	{
		if ((tmp = ft_strsub(*str, 0, ft_strlen(*str) - ft_strlen(&((*str)[*pos - 1])))) == NULL)
			return (ERROR);
		chr = ft_strrchr(tmp, '\n');
		nb = (chr ? ft_strlen(chr + 1) : ft_strlen(tmp));
		if (stline->curs_y == 0)
			nb += 2;
		while (++(stline->curs_x) < nb - 1)
			tputs(tgetstr("nd", NULL), 1, my_outc);
		stline->curs_x = nb + 1;
		ft_strdel(&tmp);
	}
	else
	{
		while (++(stline->curs_x) < stline->win.ws_col)
			tputs(tgetstr("nd", NULL), 1, my_outc);
	}
	return (TRUE);
}

int					fct_left(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- MOVE LEFT ------", 2);

	(void)history;
	if (*str && *pos > 0 && left_move_cdt(*pos, stline))
	{
			if (stline->copy.start != -1 && (*pos) > stline->copy.start)
				del_in_copy(str, pos, stline, LEFT);
			(stline->curs_x)--;
			if (stline->curs_x < 0 && stline->curs_y > 0)
			{
				tputs(tgetstr("up", NULL), 1, my_outc);
				stline->curs_x = -1;
				(stline->curs_y)--;
				multi_left(str, pos, stline);
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
		ft_putendl_fd("------- MOVE RIGHT ------", 2);

	(void)history;
	if ((stline->copy.start == -1 && ((*pos))
		< (int)ft_strlen(*str)) || (stline->copy.start != -1
		&& ((*pos)) < (int)ft_strlen(*str) - 1))
	{
		if (stline->copy.start != -1 && (*pos) < stline->copy.start)
			del_in_copy(str, pos, stline, RIGHT);
		if (stline->curs_x >= stline->win.ws_col - 1 || (*str)[*pos] == '\n')
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
		ft_putendl_fd("------- MOVE WORD LEFT ------", 2);

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
		ft_putendl_fd("------- MOVE WORD RIGHT ------", 2);

	int					x;

	(void)history;
	if (((*pos)) < (int)ft_strlen(*str))
		fct_right(str, pos, stline, history);

	x = (*pos);
	while ((x) < (int)ft_strlen(*str))
	{
		if (((*str)[x - 1] == ' ' || (*str)[x - 1] == '\n')&& (*str)[x] != ' ')
			break ;
		fct_right(str, pos, stline, history);
		x = (*pos);
	}
	return (TRUE);
}
