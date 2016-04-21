#include "shell.h"
#include "libft.h"

int				event(int key, t_line *stline)
{
	if (key == RETURN)
	{
		ft_putchar(key);
		return (1);
	}
	else if (key == BACKSPACE)
			backspace(stline);
	else if (key == END || key == HOME || key == DEL)
		spec_key(key, stline);
	else if (key == LEFT || key == RIGHT || key == OP_RIGHT || key == OP_LEFT)
		move(key, stline);
	else if (key != TAB)
		insert(stline, key, ++(stline->curs_x) - 4);
	return (0);
}
