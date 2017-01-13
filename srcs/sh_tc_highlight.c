#include <term.h>
#include "shell.h"
#include "libft.h"

int					hide_highlight(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		ft_putendl_fd("------- HIDE_HIGHLIGHT ------", 2);

	char				*tmp;
	int					i;
	int					curs_pos;

	curs_pos = (*pos);
	tmp = ft_strdup(*str);
	fct_end(str, pos, stline, history);
	while (((*pos)) > 0)
		fct_backspace(str, pos, stline, history);
	i = 0;
	while (tmp[i])
	{
		fct_insert(str, pos, tmp[i], stline);
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

static int			init_highlight(char **str, int *pos, t_line *stline)
{
	if (DEBUG_COPY_PASTE == 1)
		ft_putendl_fd("------- INIT HIGHLIGHT ------", 2);

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

static int			highlight(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		ft_putendl_fd("------- HIGHLIGHT ------", 2);

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

int					fct_highlight(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_COPY_PASTE == 1)
		ft_putendl_fd("------- FCT HIGHLIGHT ------", 2);

	int					len;

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
