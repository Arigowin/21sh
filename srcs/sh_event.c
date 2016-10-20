#include "shell.h"
#include "libft.h"

int				event(int key, t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		printf("------- EVENT ------\n");
	if (key == RETURN)
	{
		if (stline->copy != NULL && stline->cpy_start != -1)
		{
			copy_paste(HIGHLIGHT, stline);
		}
		if (stline->line && stline->line[0])
			add_history(history, stline->line);
		ft_putchar(key);
		return (1);
	}
	else if (key == BACKSPACE)
		backspace(stline);
	else if (key == END || key == HOME || key == DEL)
		spec_key(key, stline);
	else if (key == LEFT || key == RIGHT || key == OP_RIGHT || key == OP_LEFT)
		move(key, stline);
	else if (key == UP || key == DOWN)
		nav_history(key, history, stline);
	else if (key == HIGHLIGHT || key == PASTE || key == COPY || key == CUTE)
		copy_paste(key, stline);
	else if (key != TAB)
		insert(stline, key, ++(stline->curs_x) - 4);
	return (0);
}
