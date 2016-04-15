#include "shell.h"
#include "libft.h"
#include <term.h>

int		move(int key, t_line *stline)
{
	if (key == LEFT)
	{
		if (stline->curs_x > 3)
		{
			// go left
			tputs(tgetstr("le", NULL), 1, my_outc);
			stline->curs_x -= 1;
		}
	}
	else if (key == RIGHT)
	{
		if ((stline->curs_x - 3) < (int)ft_strlen(stline->line))
		{
			// go right
			tputs(tgetstr("nd", NULL), 1, my_outc);
			stline->curs_x += 1;
		}
	}
	return (0);
}
