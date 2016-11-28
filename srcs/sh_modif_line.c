#include "shell.h"
#include "libft.h"
#include <term.h>

int			fct_backspace(t_line *stline, t_history **history)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- BACKSPACE ------\n");
	char	*end_line;
	int		i;
	int		save_pos;

	save_pos = 0;
	if ((stline->line)[stline->pos_line - 1] == stline->quote)
		stline->quote = 0;
	else if (stline->quote == 0 && ((stline->line)[stline->pos_line - 1] == QUOTE ||
				(stline->line)[stline->pos_line - 1] == DQUOTE))
		stline->quote = (stline->line)[stline->pos_line - 1];

	end_line = ft_strsub(stline->line, stline->pos_line, ft_strlen(stline->line));
	if ((stline->pos_line > 0 && stline->quote != 0 && stline->curs_x > 2)
	 || (stline->pos_line > 0 && stline->quote == 0))
	{

		if (stline->curs_x < 0 && stline->curs_y > 0)
		{
			(stline->curs_x)--;
			(stline->curs_y)--;
			tputs(tgetstr("up", NULL), 1, my_outc);
			while (stline->curs_x < stline->win.ws_col)
			{
				tputs(tgetstr("nd", NULL), 1, my_outc);
				(stline->curs_x)++;
			}
			tputs(tgetstr("dc", NULL), 1, my_outc);
			stline->pos_line--;
			(stline->line)[stline->pos_line] = 0;
		}
		else
		{
			save_pos = stline->pos_line;
			i = stline->pos_line;
			fct_end(stline, history);
			while (i <= stline->pos_line)
			{
				tputs(tgetstr("dc", NULL), 1, my_outc);
				fct_left(stline, history);
				tputs(tgetstr("dc", NULL), 1, my_outc);
				(stline->line)[stline->pos_line] = 0;
			}
		}
		i = 0;
		while (end_line && end_line[i])
		{
			fct_insert(stline, end_line[i]);
			i++;
		}
		while (stline->pos_line >= save_pos)
			fct_left(stline, history);
		if (stline->curs_x == stline->win.ws_col)
			tputs(tgetstr("nd", NULL), 1, my_outc);
	}
	if (end_line)
		free(end_line);
	return (0);
}

static void			enlarge_line(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- TEST ------\n");
	char				*tmp;
	int					i;

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
}

int					fct_insert(t_line *stline, char c)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- INSERT ------\n");
	char				*tmp;
	int					i;

	tmp = NULL;
	if (stline->pos_line % BUFF_SIZE == 0 && stline->pos_line + 1 > BUFF_SIZE)
		enlarge_line(stline);
	if ((stline->line)[stline->pos_line] == '\0')
		(stline->line)[stline->pos_line] = c;
	else
	{
		if (!(tmp = ft_strsub(stline->line, stline->pos_line, ft_strlen(stline->line))))
			return (-1);
	}
	ft_putchar(c);
	if (tmp != NULL)
	{
		// ce == shift + d in vim
		tputs(tgetstr("ce", NULL), 1, my_outc);
		// save the current cursor position
		tputs(tgetstr("sc", NULL), 1, my_outc);
		(stline->line)[stline->pos_line] = c;
		i = stline->pos_line + 1;
		while (tmp[i - stline->pos_line - 1] != 0)
		{
			(stline->line)[i] = tmp[i - stline->pos_line - 1];
			i++;
		}
		(stline->line)[i] = 0;
		ft_putstr(tmp);
		// restore the last saved cursor position.
		tputs(tgetstr("rc", NULL), 1, my_outc);
	}
	stline->curs_x++;
	stline->pos_line++;
	if (stline->curs_x >= stline->win.ws_col)
	{
		stline->curs_x = 0;
		stline->curs_y++;
		tputs(tgetstr("cr", NULL), 1, my_outc);
		tputs(tgetstr("do", NULL), 1, my_outc);
	}
	if (tmp)
		free(tmp);
	return (0);
}
