#include "shell.h"
#include "libft.h"
#include <term.h>

static void		move_left(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE LEFT ------\n");


	if (stline->curs_x > 3)
	{
		if (stline->cpy_start != -1 && stline->curs_x > stline->cpy_start)
			del_in_copy(stline, 2);
		tputs(tgetstr("le", NULL), 1, my_outc);
		stline->curs_x -= 1;
		if (stline->cpy_start != -1 && stline->curs_x < stline->cpy_start)
			add_in_copy(stline, 2);
	}
}

static void		move_right(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE RIGHT ------\n");
	if ((stline->curs_x - 3) < (int)ft_strlen(stline->line))
	{
		if (stline->cpy_start != -1 && stline->curs_x < stline->cpy_start)
			del_in_copy(stline, 1);
		tputs(tgetstr("nd", NULL), 1, my_outc);
		stline->curs_x += 1;
		if (stline->cpy_start != -1 && stline->curs_x > stline->cpy_start)
			add_in_copy(stline, 1);
	}
}

static void		move_word_left(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE WORD LEFT ------\n");
	int		x;

	if ((stline->curs_x) > 3)
	{
		tputs(tgetstr("le", NULL), 1, my_outc);
		stline->curs_x--;
	}
	x = stline->curs_x - 3;
	while (x > 0)
	{
		if (stline->line[x - 1] == ' ' && stline->line[x] != ' ')
			break ;
		tputs(tgetstr("le", NULL), 1, my_outc);
		x--;
	}
	stline->curs_x = x + 3;
}

static void		move_word_right(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- MOVE WORD RIGHT ------\n");
	int		x;

	if ((stline->curs_x - 3) < (int)ft_strlen(stline->line))
	{
		tputs(tgetstr("nd", NULL), 1, my_outc);
		stline->curs_x++;
	}
	x = stline->curs_x - 3;
	while ((x) < (int)ft_strlen(stline->line))
	{
		if (stline->line[x + 1] == ' ' && stline->line[x] != ' ')
			break ;
		tputs(tgetstr("nd", NULL), 1, my_outc);
		x++;
	}
	stline->curs_x = x + 3;
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
