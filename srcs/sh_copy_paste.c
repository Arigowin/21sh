#include "shell.h"
#include "libft.h"
#include <term.h>

static int			addtbl_left(char *tbl, char c)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- ADDTBL_LEFT ------\n");

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

static int			deltbl_left(char *tbl)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- DELTBL_LEFT ------\n");

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
	if (DEBUG_COPY_PASTE == 1)
		printf("------- DEL_IN_COPY ------\n");

	if (dir != 1 && dir != 2)
		return (FALSE);
	if (dir == 1) // right
	{
		tputs(tgetstr("ue", NULL), 1, my_outc);
		ft_putchar(stline->line[stline->pos_line]);
		stline->cpy_pos--;
		deltbl_left(stline->copy);
		tputs(tgetstr("us", NULL), 1, my_outc);
		tputs(tgetstr("le", NULL), 1, my_outc);
	}
	else // (dir == 2) left
	{
		tputs(tgetstr("ue", NULL), 1, my_outc);
		ft_putchar(stline->line[stline->pos_line]);
		stline->cpy_pos--;
		stline->copy[stline->cpy_pos] = 0;
		tputs(tgetstr("us", NULL), 1, my_outc);
		tputs(tgetstr("le", NULL), 1, my_outc);
	}
	return (TRUE);
}

int				add_in_copy(t_line *stline, int dir)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- ADD_IN_COPY ------\n");

	if (dir != 1 && dir != 2)
		return (FALSE);
	if (dir == 1) // right
	{
		ft_putchar((stline->line)[stline->pos_line]);
		tputs(tgetstr("le", NULL), 1, my_outc);
		stline->copy[stline->cpy_pos] = (stline->line)[stline->pos_line];
		stline->cpy_pos++;
	}
	else // (dir == 2) left
	{
		ft_putchar((stline->line)[stline->pos_line]);
		tputs(tgetstr("le", NULL), 1, my_outc);
		addtbl_left(stline->copy, (stline->line)[stline->pos_line]);
		stline->cpy_pos++;
	}
	return (TRUE);
}

static int		hide_underline(t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- HIDE_HIGHLIGHT ------\n");

	char		*tmp;
	int			i;
	int			curs_pos;

	curs_pos = stline->pos_line;
	tmp = ft_strdup(stline->line);
	fct_end(stline, history);
	while ((stline->pos_line) > 0)
		fct_backspace(stline, history);
	i = 0;
	while (tmp[i])
	{
		fct_insert(stline, tmp[i]);
		i++;
	}
	while (stline->pos_line != curs_pos)
	{
		if (stline->pos_line > curs_pos)
			fct_left(stline, history);
		else if (stline->pos_line < curs_pos)
			fct_right(stline, history);
	}
	return (TRUE);
}

int				fct_underline(t_line *stline, t_history **history)
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
		if ((stline->pos_line) < (int)ft_strlen(stline->line))
		{
			// start highlight
			tputs(tgetstr("us", NULL), 1, my_outc);
			ft_putchar((stline->line)[stline->pos_line]);
			stline->copy[0] = (stline->line)[stline->pos_line];
			stline->pos_line++;
			stline->curs_x++;
			fct_left(stline, history);
			stline->cpy_pos = 1;
			stline->cpy_start = stline->pos_line;
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
		hide_underline(stline, history);
		return (TRUE);
	}
	return (FALSE);
}

int		fct_copy(t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- COPY ------\n");

	if (stline->cpy_start == -1)
		return (FALSE);
	tputs(tgetstr("ue", NULL), 1, my_outc);
	stline->cpy_start = -1;
	hide_underline(stline, history);
	return (TRUE);
}

int		fct_paste(t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- PASTE ------\n");

	int		i;

	i = 0;
	(void)history;
	if (!stline->copy || stline->cpy_start != -1)
		return (FALSE);
	while (stline->copy[i])
	{
		fct_insert(stline, stline->copy[i]);
		i++;
	}
	tputs(tgetstr("ue", NULL), 1, my_outc);
	return (TRUE);
}

int		fct_cut(t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- PASTE ------\n");

	int		curs_start;
	int		curs_end;

	curs_start = stline->cpy_start;
	curs_end = stline->pos_line;

	if (stline->cpy_start == -1)
		return (FALSE);
	tputs(tgetstr("ue", NULL), 1, my_outc);
	stline->cpy_start = -1;

	if (curs_end > curs_start)
	{
		fct_del(stline, history);
		curs_end--;
		while (curs_end >= curs_start)
		{
			fct_backspace(stline, history);
			curs_end--;
		}
	}
	else
	{
		while (curs_end <= curs_start)
		{
			fct_del(stline, history);
			curs_start--;
		}
	}
	return (TRUE);
}
