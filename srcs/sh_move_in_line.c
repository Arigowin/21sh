#include "shell.h"
#include "libft.h"
#include <term.h>

static void		move_left(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE LEFT ------\n");


	if (stline->pos_line > 0)
	{
		if (stline->cpy_start != -1 && stline->pos_line > stline->cpy_start)
			del_in_copy(stline, 2);

		(stline->curs_x)--;
		if (stline->curs_x < 0 && stline->curs_y > 0)
		{
			tputs(tgetstr("up", NULL), 1, my_outc);
			stline->curs_x = 0;
			(stline->curs_y)--;

			while (stline->curs_x < stline->win.ws_col)
			{
				tputs(tgetstr("nd", NULL), 1, my_outc);
				(stline->curs_x)++;
			}
			tputs(tgetstr("nd", NULL), 1, my_outc);
			(stline->curs_x)--;
		}
		else
			tputs(tgetstr("le", NULL), 1, my_outc);

		stline->pos_line -= 1;
		if (stline->cpy_start != -1 && stline->pos_line < stline->cpy_start)
			add_in_copy(stline, 2);
	}
}

static void		move_right(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE RIGHT ------\n");


	if ((stline->pos_line) < (int)ft_strlen(stline->line))
	{
		if (stline->cpy_start != -1 && stline->pos_line < stline->cpy_start)
			del_in_copy(stline, 1);

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
		stline->pos_line += 1;
		if (stline->cpy_start != -1 && stline->pos_line > stline->cpy_start)
			add_in_copy(stline, 1);
	}
}

static void		move_word_left(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE WORD LEFT ------\n");
	int		x;

	if ((stline->pos_line) > 0)
		move_left(stline);

	x = stline->pos_line;
	while (x > 0)
	{
		if (stline->line[x - 1] == ' ' && stline->line[x] != ' ')
			break ;
		move_left(stline);
		x = stline->pos_line;
	}
}

static void		move_word_right(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE WORD RIGHT ------\n");
	int		x;

	if ((stline->pos_line) < (int)ft_strlen(stline->line))
		move_right(stline);

	x = stline->pos_line;
	while ((x) < (int)ft_strlen(stline->line))
	{
		if (stline->line[x + 1] == ' ' && stline->line[x] != ' ')
			break ;
		move_right(stline);
		x = stline->pos_line;
	}
}

int				move(int key, t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE ------\n");
	if (key == LEFT)
		move_left(stline);
	else if (key == RIGHT)
		move_right(stline);
	else if (key == OP_RIGHT)
		move_word_right(stline);
	else if (key == OP_LEFT)
		move_word_left(stline);
	else
		return (-1);
	return (0);
}
