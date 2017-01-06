#include "shell.h"
#include "libft.h"
#include <term.h>

int					handle_delete_quote(char **str, int *pos, t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- HANDLE DELETE QUOTE ------\n");

	if ((*str)[*pos - 1] == stline->quote)
		stline->quote = 0;
	else if (stline->quote == 0 && ((*str)[*pos - 1] == QUOTE
	|| (*str)[*pos - 1] == DQUOTE))
		stline->quote = (*str)[*pos - 1];
	return (TRUE);
}

int					delete_char(char **str, int *pos, int i, t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- DELETE_CHAR ------\n");

	if (stline->curs_x < 0 && stline->curs_y > 0)
	{
		(stline->curs_x)--;
		(stline->curs_y)--;
		tputs(tgetstr("up", NULL), 1, my_outc);
		while (stline->curs_x++ < stline->win.ws_col)
			tputs(tgetstr("nd", NULL), 1, my_outc);
		tputs(tgetstr("dc", NULL), 1, my_outc);
		(*str)[--(*pos)] = '\0';
	}
	else
	{
		i = (*pos);
		fct_end(str, pos, stline, NULL);
		while (i <= (*pos))
		{
			tputs(tgetstr("dc", NULL), 1, my_outc);
			fct_left(str, pos, stline, NULL);
			tputs(tgetstr("dc", NULL), 1, my_outc);
			(*str)[(*pos)] = '\0';
		}
	}
	return (TRUE);
}

// attention on a modifier pas sur que ca fonctionne !!!!
int					fct_backspace(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- BACKSPACE ------\n");

	char				*eol;
	int					i;
	int					save_pos;

	save_pos = 0;
	if (stline->copy.start != -1)
		return (FALSE);
	handle_delete_quote(str, pos, stline);
	eol = ft_strsub(*str, (*pos), ft_strlen(*str));
	if (left_move_cdt(*pos, stline))
	{
		i = 0;
		save_pos = *pos;
		delete_char(str, pos, i, stline);
		while (eol && eol[i]) // !!!!
			fct_insert(eol[i++], str, pos, stline);
		while ((*pos) >= save_pos)
			fct_left(str, pos, stline, history);
		if (stline->curs_x == stline->win.ws_col)
			tputs(tgetstr("nd", NULL), 1, my_outc);
	}
	if (eol)
		free(eol);
	return (TRUE);
}

static int			enlarge_line(char **str, int *pos)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- ENLARGE LINE ------\n");

	char				*tmp;
	int					i;

	if (!((*pos) % BUFF_SIZE == 0 && (*pos) + 1 > BUFF_SIZE))
		return (FALSE);
	tmp = ft_strdup(*str);
	i = ft_strlen(*str) + BUFF_SIZE;
	ft_strdel(str);
	*str = ft_strnew(i);
	i = 0;
	while (tmp[i])
	{
		(*str)[i] = tmp[i];
		i++;
	}
	free(tmp);
	return (TRUE);
}

int					insert_char(char c, char *end_line, char **str, int *pos)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- INSERT CHAR ------\n");
	int					i;

	i = 0;
	// ce == shift + d in vim
	tputs(tgetstr("ce", NULL), 1, my_outc);
	// save the current cursor position
	tputs(tgetstr("sc", NULL), 1, my_outc);
	(*str)[(*pos)] = c;
	i = (*pos) + 1;
	while (end_line[i - (*pos) - 1] != 0)
	{
		(*str)[i] = end_line[i - (*pos) - 1];
		i++;
	}
	(*str)[i] = '\0';
	ft_putstr(end_line);
	// restore the last saved cursor position.
	tputs(tgetstr("rc", NULL), 1, my_outc);
	return(TRUE);
}

int					fct_insert(char c, char **str, int *pos, t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- INSERT ------\n");

	char				*end_line;

	end_line = NULL;
	if (stline->copy.start != -1)
		return (FALSE);
	enlarge_line(str, pos);
	if ((*str)[(*pos)] == '\0')
		(*str)[(*pos)] = c;
	else if (!(end_line = ft_strsub(*str, (*pos), ft_strlen(*str))))
		return (ERROR);
	ft_putchar(c);
	if (end_line != NULL)
		insert_char(c, end_line, str, pos);
	stline->curs_x++;
	(*pos)++;
	if (stline->curs_x >= stline->win.ws_col)
	{
		stline->curs_x = 0;
		stline->curs_y++;
		tputs(tgetstr("cr", NULL), 1, my_outc);
		tputs(tgetstr("do", NULL), 1, my_outc);
	}
	ft_strdel(&end_line);
	return (0);
}
