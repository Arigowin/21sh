#include "shell.h"
#include "libft.h"
#include <term.h>

int					str_addleft(char *tbl, char c)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- str_addleft ------\n");

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

static int			str_delleft(char *tbl)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- str_delleft ------\n");

	int		i;

	i = 0;
	if (tbl == NULL)
		return (FALSE);
	while (tbl[i])
	{
		tbl[i] = tbl[i + 1];
		i++;
	}
	tbl[i] = '\0';
	return (TRUE);
}

int				del_in_copy(char **str, int *pos, t_line *stline, int dir)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- DEL_IN_COPY ------\n");

	if (dir != RIGHT && dir != LEFT)
		return (FALSE);
	tputs(tgetstr("me", NULL), 1, my_outc);
	ft_putchar_color(RESET_COLOR, (*str)[*pos]);
	tputs(tgetstr("le", NULL), 1, my_outc);
	(stline->copy.pos)--;
	if (dir == RIGHT)
		str_delleft(stline->copy.cpy);
	else
		stline->copy.cpy[stline->copy.pos] = '\0';
	tputs(tgetstr("mr", NULL), 1, my_outc);
	return (TRUE);
}

int				add_in_copy(char **str, int *pos, t_line *stline, int dir)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- ADD_IN_COPY ------\n");

	if (dir != RIGHT && dir != LEFT)
		return (FALSE);
	tputs(tgetstr("mr", NULL), 1, my_outc);
	ft_putchar_color(COLOR, (*str)[(*pos)]);
	tputs(tgetstr("le", NULL), 1, my_outc);
	if (dir == RIGHT)
	{
		stline->copy.cpy[stline->copy.pos] = (*str)[(*pos)];
		(stline->copy.pos)++;
	}
	else
	{
		str_addleft(stline->copy.cpy, (*str)[(*pos)]);
		(stline->copy.pos)++;
	}
	tputs(tgetstr("me", NULL), 1, my_outc);
	return (TRUE);
}

static int		hide_highlight(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- HIDE_HIGHLIGHT ------\n");

	char		*tmp;
	int			i;
	int			curs_pos;

	curs_pos = (*pos);
	tmp = ft_strdup(*str);
	fct_end(str, pos, stline, history);
	while (((*pos)) > 0)
		fct_backspace(str, pos, stline, history);
	i = 0;
	while (tmp[i])
	{
		fct_insert(tmp[i], str, pos, stline);
		i++;
	}
	while ((*pos) != curs_pos)
	{
		if ((*pos) > curs_pos)
			fct_left(str, pos, stline, history);
		else if ((*pos) < curs_pos)
			fct_right(str, pos, stline, history);
	}
	tputs(tgetstr("ve", NULL), 1, my_outc);
	return (TRUE);
}

static int		init_highlight(char **str, int *pos, t_line *stline)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- INIT HIGHLIGHT ------\n");

	(void)pos;
	if (stline->copy.cpy != NULL)
	{
		stline->copy.bkup = ft_strdup(stline->copy.cpy);
		ft_strdel(&(stline->copy.cpy));
	}
	stline->copy.cpy = ft_strnew(ft_strlen(*str));
	tputs(tgetstr("vi", NULL), 1, my_outc);
	return (TRUE);
}

static int		highlight(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- HIGHLIGHT ------\n");

	// start highlight
	tputs(tgetstr("mr", NULL), 1, my_outc);
	ft_putchar_color(COLOR, (*str)[(*pos)]);
	stline->copy.pos = 0;
	stline->copy.cpy[stline->copy.pos] = (*str)[(*pos)];
	(*pos)++;
	stline->curs_x++;
	fct_left(str, pos, stline, history);
	stline->copy.pos++;
	stline->copy.start = (*pos);
	tputs(tgetstr("me", NULL), 1, my_outc);
	return (TRUE);
}

int				fct_highlight(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- FCT HIGHLIGHT ------\n");

	int			len;

	len = ft_strlen(*str);
	if (stline->copy.start == -1)
	{
		init_highlight(str, pos, stline);
		if ((*pos) == len)
			fct_left(str, pos, stline, history);
		highlight(str, pos, stline, history);
	}
	else
	{
		tputs(tgetstr("me", NULL), 1, my_outc);
		ft_strdel(&(stline->copy.cpy));
		if (stline->copy.bkup != NULL)
			stline->copy.cpy = ft_strdup(stline->copy.bkup);
		stline->copy.start = -1;
		hide_highlight(str, pos, stline, history);
		return (TRUE);
	}
	return (FALSE);
}

int		fct_copy(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- COPY ------\n");

	if (stline->copy.start == -1)
		return (FALSE);
	tputs(tgetstr("me", NULL), 1, my_outc);
	stline->copy.start = -1;
	hide_highlight(str, pos, stline, history);
	return (TRUE);
}

int		fct_paste(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- PASTE ------\n");

	int		i;

	i = 0;
	(void)history;
	if (!stline->copy.cpy || stline->copy.start != -1)
		return (FALSE);
	while (stline->copy.cpy[i])
	{
		fct_insert(stline->copy.cpy[i], str, pos, stline);
		i++;
	}
	tputs(tgetstr("me", NULL), 1, my_outc);
	return (TRUE);
}

int		fct_cut(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- PASTE ------\n");

	int		curs_start;
	int		curs_end;

	curs_start = stline->copy.start;
	curs_end = (*pos);
	if (stline->copy.start == -1)
		return (FALSE);
	tputs(tgetstr("me", NULL), 1, my_outc);
	stline->copy.start = -1;
	if (curs_end > curs_start)
	{
		fct_del(str, pos, stline, history);
		while (--curs_end >= curs_start)
			fct_backspace(str, pos, stline, history);
	}
	else
	{
		while (curs_end <= curs_start)
		{
			fct_del(str, pos, stline, history);
			curs_start--;
		}
	}
	tputs(tgetstr("ve", NULL), 1, my_outc);
	return (TRUE);
}
