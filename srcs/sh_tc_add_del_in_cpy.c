#include <term.h>
#include "shell.h"
#include "libft.h"

int					str_addleft(char *tbl, char c)
{
	if (DEBUG_COPY_PASTE == 1)
		ft_putendl_fd("------- str_addleft ------", 2);

	int					i;

	i = 0;
	if (tbl == NULL)
		return (FALSE);
	while (tbl[i])
		i++;
	i--;
	while (i >= 0)
	{
		tbl[i + 1] = tbl[i];
		i--;
	}
	tbl[0] = c;
	return (TRUE);
}

int				str_delleft(char *tbl)
{
	if (DEBUG_COPY_PASTE == 1)
		ft_putendl_fd("------- str_delleft ------", 2);

	int					i;

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

int					del_in_copy(char **str, int *pos, t_line *stline, int dir)
{
	if (DEBUG_COPY_PASTE == 1)
		ft_putendl_fd("------- DEL_IN_COPY ------", 2);

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

int					add_in_copy(char **str, int *pos, t_line *stline, int dir)
{
	if (DEBUG_COPY_PASTE == 1)
		ft_putendl_fd("------- ADD_IN_COPY ------", 2);

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
