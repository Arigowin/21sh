#include "shell.h"
#include "libft.h"

int				event(int key, t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		printf("------- EVENT ------\n");
	if (key == RETURN && stline->quote == 0)
	{
		if (stline->line && stline->line[0])
			add_history(history, stline->line);
		ft_putchar(key);
		return (1);
	}
	else if (key == RETURN && stline->quote != 0)
	{
//		ft_putchar(key);
		insert(stline, '\n', ++(stline->curs_x) - 4);
		if (stline->quote == QUOTE)
			ft_putstr("quote> ");
		if (stline->quote == DQUOTE)
			ft_putstr("dquote> ");
		return (2);
	}
	else if (key == BACKSPACE)
		backspace(stline);
	else if (key == END || key == HOME || key == DEL)
		spec_key(key, stline);
	else if (key == LEFT || key == RIGHT || key == OP_RIGHT || key == OP_LEFT)
		move(key, stline);
	else if (key == UP || key == DOWN)
		nav_history(key, history, stline);
	else if (key != TAB)
		insert(stline, key, ++(stline->curs_x) - 4);
	return (0);
}
