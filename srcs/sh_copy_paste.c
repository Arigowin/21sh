#include "shell.h"
#include "libft.h"
#include <term.h>

int				addtbl_left(char *tbl, char c)
{
	int		i;

	i = 0;
	if (tbl == NULL)
		return (FALSE);
	while (tbl[i])
		i++;
	while (i >= 0)
	{
		tbl[i + 1] = tbl[i];
		i--;
	}
	tbl[0] = c;
	return (TRUE);
}

int				deltbl_left(char *tbl)
{
	int		i;

	i = 0;
	if (tbl == NULL)
		return (FALSE);
	while (tbl[i])
	{
		tbl[i] = tbl[i + 1];
		i++;
	}
	return (TRUE);
}

int				del_in_copy(t_line *stline, int dir)
{
	if (dir != 1 && dir != 2)
		return (FALSE);
	if (dir == 1) // right
	{
		tputs(tgetstr("ue", NULL), 1, my_outc);

		ft_putchar(stline->line[stline->curs_x - 3]);
		stline->cpy_pos--;
		deltbl_left(stline->copy);

		tputs(tgetstr("us", NULL), 1, my_outc);
		tputs(tgetstr("le", NULL), 1, my_outc);
	}
	else // (dir == 2) left
	{
		tputs(tgetstr("ue", NULL), 1, my_outc);

		ft_putchar(stline->line[stline->curs_x - 3]);
		stline->cpy_pos--;
		stline->copy[stline->cpy_pos] = 0;

		tputs(tgetstr("us", NULL), 1, my_outc);
		tputs(tgetstr("le", NULL), 1, my_outc);
	}
	return (TRUE);
}

int				add_in_copy(t_line *stline, int dir)
{
	if (dir != 1 && dir != 2)
		return (FALSE);
	if (dir == 1) // right
	{
		ft_putchar((stline->line)[stline->curs_x - 3]);
		tputs(tgetstr("le", NULL), 1, my_outc);
		stline->copy[stline->cpy_pos] =
			(stline->line)[stline->curs_x - 3];
		stline->cpy_pos++;
	}
	else // (dir == 2) left
	{
		ft_putchar((stline->line)[stline->curs_x - 3]);
		tputs(tgetstr("le", NULL), 1, my_outc);
		addtbl_left(stline->copy, (stline->line)[stline->curs_x - 3]);
		stline->cpy_pos++;
	}
	return (TRUE);
}

static int		hide_highlight(t_line *stline)
{
	char		*tmp;
	int			i;
	int			curs_pos;

	curs_pos = stline->curs_x;
	tmp = ft_strdup(stline->line);
	while ((stline->curs_x - 3) < (int)ft_strlen(stline->line))
		move(RIGHT, stline);
	while ((stline->curs_x) > 3)
		backspace(stline);
	i = 0;
	while (tmp[i])
	{
		insert(stline, tmp[i], ++(stline->curs_x) - 4);
		i++;
	}
	while (stline->curs_x != curs_pos)
	{
		if (stline->curs_x > curs_pos)
			move(LEFT, stline);
		else if (stline->curs_x < curs_pos)
			move(RIGHT, stline);
	}
	return (TRUE);
}

static int		highlight(t_line *stline)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- HIGHLIGHT ------\n");
	if (stline->cpy_start == -1)
	{
		if (stline->copy != NULL)
		{
			free(stline->copy);
			stline->copy = NULL;
		}
		if (stline->copy == NULL)
		{
			stline->copy = ft_strnew(ft_strlen(stline->line));
			stline->cpy_pos = 0;
		}
		if ((stline->curs_x - 3) < (int)ft_strlen(stline->line))
		{
			// start highlight
			tputs(tgetstr("us", NULL), 1, my_outc);

			ft_putchar((stline->line)[stline->curs_x - 3]);
			stline->copy[0] = (stline->line)[stline->curs_x - 3];
			stline->curs_x++;
			move(LEFT, stline);
			stline->cpy_pos = 1;
			stline->cpy_start = stline->curs_x;

			return (TRUE);
		}
	}
	else
	{
		tputs(tgetstr("ue", NULL), 1, my_outc);
		free(stline->copy);
		stline->copy = NULL;
		stline->cpy_pos = 0;
		stline->cpy_start = -1;
		hide_highlight(stline);
		return (TRUE);
	}
	return (FALSE);
}

static int		copy(t_line *stline)
{
	tputs(tgetstr("ue", NULL), 1, my_outc);
	stline->cpy_start = -1;
	hide_highlight(stline);
	return (TRUE);
}

static int		paste(t_line *stline)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- PASTE ------\n");
	int		i;

	i = 0;
	while (stline->copy[i])
	{
		insert(stline, stline->copy[i], ++(stline->curs_x) - 4);
		i++;
	}
	tputs(tgetstr("ue", NULL), 1, my_outc);
	return (TRUE);
}

int				copy_paste(int key, t_line *stline)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- COPY_PASTE ------\n");


	if (key == HIGHLIGHT)
		highlight(stline);
	else if (key == PASTE && stline->copy && stline->cpy_start == -1)
	{
		paste(stline);
	}
	else if (key == COPY && stline->cpy_start != -1)
	{
		copy(stline);
	}
	else if (key == CUTE && stline->cpy_start != -1)
	{
		printf("CUTE\n");
	}
	return (TRUE);
}
