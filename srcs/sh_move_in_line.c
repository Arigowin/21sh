#include "shell.h"
#include "libft.h"
#include <term.h>

int		move(int key, int *x, char *line)
{
	if (key == LEFT)
	{
		if (*x > 3)
		{
			// go left
			tputs(tgetstr("le", NULL), 1, my_outc);
			*x -= 1;
		}
	}
	else if (key == RIGHT)
	{
		if ((*x - 3) < (int)ft_strlen(line))
		{
			// go right
			tputs(tgetstr("nd", NULL), 1, my_outc);
			*x += 1;
		}
	}
	return (0);
}
