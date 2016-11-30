#include "shell.h"
#include "libft.h"
#include <term.h>

int					handle_delete_quote(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- HANDLE DELETE QUOTE ------\n");
	int					pos;

	pos = stline->pos_line;
	if ((stline->line)[pos - 1] == stline->quote)
		stline->quote = 0;
	else if (stline->quote == 0 && ((stline->line)[pos - 1] == QUOTE
			|| (stline->line)[pos - 1] == DQUOTE))
		stline->quote = (stline->line)[pos - 1];
	return (TRUE);
}

int					delete_char(t_line *stline, int i, int *save_pos)
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
		(stline->line)[--stline->pos_line] = '\0';
	}
	else
	{
		(*save_pos) = stline->pos_line;
		i = stline->pos_line;
		fct_end(stline, NULL);
		while (i <= stline->pos_line)
		{
			tputs(tgetstr("dc", NULL), 1, my_outc);
			fct_left(stline, NULL);
			tputs(tgetstr("dc", NULL), 1, my_outc);
			(stline->line)[stline->pos_line] = '\0';
		}
	}
	return (TRUE);
}

int					fct_backspace(t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- BACKSPACE ------\n");
	char				*eol;
	int					i;
	int					save_pos;

	save_pos = 0;
	if (stline->copy.start != -1)
		return (FALSE);
	handle_delete_quote(stline);
	eol = ft_strsub(stline->line, stline->pos_line, ft_strlen(stline->line));
	if ((stline->pos_line > 0 && stline->quote != 0 && stline->curs_x > 2)
			|| (stline->pos_line > 0 && stline->quote == 0))
	{
		i = 0;
		delete_char(stline, i, &save_pos);
		while (eol && eol[i])
		{
			fct_insert(stline, eol[i]);
			i++;
		}
		while (stline->pos_line >= save_pos)
			fct_left(stline, history);
		if (stline->curs_x == stline->win.ws_col)
			tputs(tgetstr("nd", NULL), 1, my_outc);
	}
	if (eol)
		free(eol);
	return (TRUE);
}

static int			enlarge_line(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- ENLARGE LINE ------\n");
	char				*tmp;
	int					i;

	if (!(stline->pos_line % BUFF_SIZE == 0
				&& stline->pos_line + 1 > BUFF_SIZE))
		return (FALSE);
	tmp = ft_strdup(stline->line);
	i = ft_strlen(stline->line) + BUFF_SIZE;
	free(stline->line);
	stline->line = ft_strnew(i);
	i = 0;
	while (tmp[i])
	{
		(stline->line)[i] = tmp[i];
		i++;
	}
	free(tmp);
	return (TRUE);
}

int					insert_char(t_line *stline, char c, char *end_line)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- INSERT CHAR ------\n");
	int					i;

	i = 0;
	// ce == shift + d in vim
	tputs(tgetstr("ce", NULL), 1, my_outc);
	// save the current cursor position
	tputs(tgetstr("sc", NULL), 1, my_outc);
	(stline->line)[stline->pos_line] = c;
	i = stline->pos_line + 1;
	while (end_line[i - stline->pos_line - 1] != 0)
	{
		(stline->line)[i] = end_line[i - stline->pos_line - 1];
		i++;
	}
	(stline->line)[i] = '\0';
	ft_putstr(end_line);
	// restore the last saved cursor position.
	tputs(tgetstr("rc", NULL), 1, my_outc);
	return(TRUE);
}

int					fct_insert(t_line *stline, char c)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- INSERT ------\n");
	char				*end_line;

	end_line = NULL;
	if (stline->copy.start != -1)
		return (FALSE);
	enlarge_line(stline);
	if ((stline->line)[stline->pos_line] == '\0')
		(stline->line)[stline->pos_line] = c;
	else if (!(end_line = ft_strsub(stline->line, stline->pos_line,
					ft_strlen(stline->line))))
		return (-1);
	ft_putchar(c);
	if (end_line != NULL)
		insert_char(stline, c, end_line);
	stline->curs_x++;
	stline->pos_line++;
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
