#include "shell.h"
#include "libft.h"
#include <term.h>

int			backspace(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- BACKSPACE ------\n");
	char	*tmp;
	int		i;

	tmp = ft_strsub(stline->line, stline->curs_x - 3, ft_strlen(stline->line));
	(stline->line)[stline->curs_x - 4] = 0;
	i = 0;
	while (tmp && tmp[i])
	{
		(stline->line)[(stline->curs_x - 4) + i] = tmp[i];
		i++;
	}
	(stline->line)[(stline->curs_x - 4) + i] = 0;
	if (stline->curs_x > 3)
	{
		(stline->curs_x)--;
		// go left
		tputs(tgetstr("le", NULL), 1, my_outc);
		// delete one character position at the cursor.
		tputs(tgetstr("dc", NULL), 1, my_outc);
	}
	return (0);
}

static int	insert2(t_line *stline, char c, int pos, char **tmp)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- INSERT2 ------\n");
	if ((stline->line)[pos] == 0)
		(stline->line)[pos] = c;
	else
	{
		if (!(*tmp = ft_strsub(stline->line, pos, ft_strlen(stline->line))))
			return (-1);
	}
	ft_putchar(c);
	return (0);
}

int			insert(t_line *stline, char c, int pos)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- INSERT ------\n");
	char	*tmp;
	int		i;

	tmp = NULL;
	if (insert2(stline, c, pos, &tmp) == -1)
		return (-1);
	if (tmp != NULL)
	{
		// ce == shift + d in vim
		tputs(tgetstr("ce", NULL), 1, my_outc);
		// save the current cursor position
		tputs(tgetstr("sc", NULL), 1, my_outc);
		(stline->line)[pos] = c;
		i = pos + 1;
		while (tmp[i - pos - 1] != 0)
		{
			(stline->line)[i] = tmp[i - pos - 1];
			i++;
		}
		(stline->line)[i] = 0;
		ft_putstr(tmp);
		// restore the last saved cursor position.
		tputs(tgetstr("rc", NULL), 1, my_outc);
	}
	return (0);
}
