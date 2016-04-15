#include "shell.h"
#include "libft.h"
#include <term.h>

int			backspace(char **line, int *x)
{
	char	*tmp;
	int		i;

	tmp = ft_strsub(*line, *x - 3, ft_strlen(*line));
	(*line)[*x - 4] = 0;
	i = 0;
	while (tmp && tmp[i])
	{
		(*line)[(*x - 4) + i] = tmp[i];
		i++;
	}
	(*line)[(*x - 4) + i] = 0;
	if (*x > 3)
	{
		(*x)--;
		// go left
		tputs(tgetstr("le", NULL), 1, my_outc);
		// delete one character position at the cursor.
		tputs(tgetstr("dc", NULL), 1, my_outc);
	}
	return (0);
}

int			del(char **line, int *x)
{
	int		tmp;

	tmp = *x;
	move(185, x, *line);
	if (*x != tmp)
		backspace(line, x);
	return (0);
}

int		insert(char **line, char c, int pos)
{
	char	*tmp;
	int		i;

	tmp = NULL;
	if ((*line)[pos] == 0)
		(*line)[pos] = c;
	else
	{
		if ((tmp = ft_strsub(*line, pos, ft_strlen(*line))) == NULL)
			return (-1);
	}
	ft_putchar(c);
	if (tmp != NULL)
	{
		// ce == shift + d in vim
		tputs(tgetstr("ce", NULL), 1, my_outc);
		// save the current cursor position
		tputs(tgetstr("sc", NULL), 1, my_outc);
		(*line)[pos] = c;
		i = pos + 1;
		while (tmp[i - pos - 1] != 0)
		{
			(*line)[i] = tmp[i - pos - 1];
			i++;
		}
		(*line)[i] = 0;
		ft_putstr(tmp);
		// restore the last saved cursor position.
		tputs(tgetstr("rc", NULL), 1, my_outc);
	}
	return (0);
}
