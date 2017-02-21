#include <term.h>
#include "shell.h"
#include "libft.h"


static int			enlarge_line(char **str, int *pos)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- ENLARGE LINE ------", 2);

	char				*tmp;
	int					i;

	if (!((*pos) % BUFF_SIZE == 0 && (*pos) + 1 > BUFF_SIZE))
		return (FALSE);
	if ((tmp = ft_strdup(*str)) == NULL)
		return (sh_error(TRUE, 6, NULL, NULL));
	i = ft_strlen(*str) + BUFF_SIZE;
	ft_strdel(str);
	if ((*str = ft_strnew(i)) == NULL)
		return (sh_error(TRUE, 6, NULL, NULL));
	i = 0;
	while (tmp[i])
	{
		(*str)[i] = tmp[i];
		i++;
	}
	free(tmp);
	return (TRUE);
}

static int			screen_up(int *pos, char **str, t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- SCREEN UP ------", 2);

	char				*line;
	int					nb_line;
	int					len;
	int					rel_p;
	int					i;

	i = 0;
	line = ft_strrchr(*str, '\n');
	len = (line != NULL ? ft_strlen(line + 1) : ft_strlen(*str));
	rel_p = (line != NULL ? *pos - (ft_strlen(*str) - ft_strlen(line)) : *pos);
	nb_line = (len + PRT_LEN) / stline->win.ws_col;
	if (nb_line && rel_p != len && (len + PRT_LEN) % stline->win.ws_col == 0)
	{
		i = nb_line - stline->curs_y;
		while (i-- > 0)
			tputs(tgetstr("do", NULL), 1, my_outc);
		i = nb_line - stline->curs_y;
		while (i-- > 0)
			tputs(tgetstr("up", NULL), 1, my_outc);
		i = stline->curs_x;
		while (i-- > 0)
			tputs(tgetstr("nd", NULL), 1, my_outc);
	}
	return (TRUE);
}

static int			insert_char(char c, char *end_line, char **str, int *pos)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- INSERT CHAR ------", 2);

	int					i;

	i = 0;
	// ce == shift + d in vim
	tputs(tgetstr("ce", NULL), 1, my_outc);
	(*str)[*pos] = c;
	i = (*pos) + 1;
	while (end_line[i - (*pos) - 1] != 0)
	{
		(*str)[i] = end_line[i - (*pos) - 1];
		i++;
	}
	(*str)[i] = '\0';
	// save the current cursor position
	tputs(tgetstr("sc", NULL), 1, my_outc);
	ft_putstr(end_line);
	// restore the last saved cursor position.
	tputs(tgetstr("rc", NULL), 1, my_outc);
	return(TRUE);
}

static int			muli_line_insert(t_line *stline, char c)
{
	if (stline->mini_prt == FALSE && c == '\n')
	{
		stline->curs_x = 0;
		stline->curs_y++;
	}
	if (stline->curs_x >= stline->win.ws_col)
	{
		stline->curs_x = 0;
		stline->curs_y++;
		tputs(tgetstr("cr", NULL), 1, my_outc);
		tputs(tgetstr("do", NULL), 1, my_outc);
	}
	return (TRUE);
}

int					fct_insert(char **str, int *pos, char c, t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		ft_putendl_fd("------- INSERT ------", 2);

	char				*end_line;

	end_line = NULL;
	if (stline->copy.start != -1)
		return (FALSE);
	enlarge_line(str, pos);
	if ((*str)[*pos] == '\0')
		(*str)[*pos] = c;
	else if (!(end_line = ft_strsub(*str, *pos, ft_strlen(*str))))
		return (sh_error(TRUE, 6, NULL, NULL));
	/* FREE : stline*/
	screen_up(pos, str, stline);
	ft_putchar(c);
	if (end_line != NULL)
		insert_char(c, end_line, str, pos);
	stline->curs_x++;
	(*pos)++;
	muli_line_insert(stline, c);
	ft_strdel(&end_line);
	return (TRUE);
}
