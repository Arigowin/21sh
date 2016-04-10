#include "shell.h"
#include "libft.h"
#include <term.h>

t_bool			backspace(char (*line)[], int *x)
{
	// probleme trop a droite
	char	*tmp;
	int		i;

	if ((tmp = ft_strsub(*line, *x - 3, ft_strlen(*line))) == NULL)
		return (FALSE);
	(*line)[*x - 4] = 0;
	i = 0;
	while (tmp[i])
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
	return (TRUE);
}

t_bool		insert(char (*line)[], char c, int pos)
{
	char	*tmp;
	int		i;

	tmp = NULL;
	if ((*line)[pos] == 0)
		(*line)[pos] = c;
	else
		if ((tmp = ft_strsub(*line, pos, ft_strlen(*line))) == NULL)
			return (FALSE);
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
	return (TRUE);
}
