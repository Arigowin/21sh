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
	tbl[i] = '\0';
	return (TRUE);
}

int				del_in_copy(t_line *stline, int dir)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- DEL_IN_COPY ------\n");

	if (dir != RIGHT && dir != LEFT)
		return (FALSE);
	tputs(tgetstr("me", NULL), 1, my_outc);
	ft_putchar_color(RESET_COLOR, stline->line[stline->pos_line]);
	tputs(tgetstr("le", NULL), 1, my_outc);
	stline->copy.pos--;
	if (dir == RIGHT)
		deltbl_left(stline->copy.cpy);
	else
		stline->copy.cpy[stline->copy.pos] = '\0';
	tputs(tgetstr("mr", NULL), 1, my_outc);
	return (TRUE);
}

int				add_in_copy(t_line *stline, int dir)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- ADD_IN_COPY ------\n");

	if (dir != RIGHT && dir != LEFT)
		return (FALSE);
	tputs(tgetstr("mr", NULL), 1, my_outc);
	ft_putchar_color(COLOR, stline->line[stline->pos_line]);
	tputs(tgetstr("le", NULL), 1, my_outc);
	if (dir == RIGHT)
	{
		stline->copy.cpy[stline->copy.pos] = (stline->line)[stline->pos_line];
		stline->copy.pos++;
	}
	else
	{
		addtbl_left(stline->copy.cpy, (stline->line)[stline->pos_line]);
		stline->copy.pos++;
	}
	tputs(tgetstr("me", NULL), 1, my_outc);
	return (TRUE);
}

static int		hide_highlight(t_line *stline, t_history **history)
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
	tputs(tgetstr("ve", NULL), 1, my_outc);
	return (TRUE);
}

static int		init_highlight(t_line *stline)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- INIT HIGHLIGHT ------\n");
	if (stline->copy.cpy != NULL)
	{
		stline->copy.bkup = ft_strdup(stline->copy.cpy);
		ft_strdel(&(stline->copy.cpy));
	}
	stline->copy.cpy = ft_strnew(ft_strlen(stline->line));
	tputs(tgetstr("vi", NULL), 1, my_outc);
	return (TRUE);
}

static int		highlight(t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- HIGHLIGHT ------\n");
	// start highlight
	tputs(tgetstr("mr", NULL), 1, my_outc);
	ft_putchar_color(COLOR, stline->line[stline->pos_line]);
	stline->copy.pos = 0;
	stline->copy.cpy[stline->copy.pos] = (stline->line)[stline->pos_line];
	stline->pos_line++;
	stline->curs_x++;
	fct_left(stline, history);
	stline->copy.pos++;
	stline->copy.start = stline->pos_line;
	tputs(tgetstr("me", NULL), 1, my_outc);
	return (TRUE);
}

int				fct_highlight(t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- FCT HIGHLIGHT ------\n");
	int			len;

	len = ft_strlen(stline->line);
	if (stline->copy.start == -1)
	{
		init_highlight(stline);
		if (stline->pos_line == len)
			fct_left(stline, history);
		highlight(stline, history);
	}
	else
	{
		tputs(tgetstr("me", NULL), 1, my_outc);
		ft_strdel(&(stline->copy.cpy));
		if (stline->copy.bkup != NULL)
			stline->copy.cpy = ft_strdup(stline->copy.bkup);
		stline->copy.start = -1;
		hide_highlight(stline, history);
		return (TRUE);
	}
	return (FALSE);
}

int		fct_copy(t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- COPY ------\n");

	if (stline->copy.start == -1)
		return (FALSE);
	tputs(tgetstr("me", NULL), 1, my_outc);
	stline->copy.start = -1;
	hide_highlight(stline, history);
	return (TRUE);
}

int		fct_paste(t_line *stline, t_history **history)
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
		fct_insert(stline, stline->copy.cpy[i]);
		i++;
	}
	tputs(tgetstr("me", NULL), 1, my_outc);
	return (TRUE);
}

int		fct_cut(t_line *stline, t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- PASTE ------\n");

	int		curs_start;
	int		curs_end;

	curs_start = stline->copy.start;
	curs_end = stline->pos_line;
	if (stline->copy.start == -1)
		return (FALSE);
	tputs(tgetstr("me", NULL), 1, my_outc);
	stline->copy.start = -1;
	if (curs_end > curs_start)
	{
		fct_del(stline, history);
		while (--curs_end >= curs_start)
			fct_backspace(stline, history);
	}
	else
	{
		while (curs_end <= curs_start)
		{
			fct_del(stline, history);
			curs_start--;
		}
	}
	tputs(tgetstr("ve", NULL), 1, my_outc);
	return (TRUE);
}
