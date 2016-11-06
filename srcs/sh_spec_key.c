#include "shell.h"
#include "libft.h"
#include <term.h>

static void			del(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- DEL ------\n");
	int		tmp;

	tmp = stline->pos_line;
	move(RIGHT, stline);
	if (stline->pos_line != tmp)
		backspace(stline);
}

static void			home(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- HOME ------\n");
	while (stline->pos_line > 0)
		move(LEFT, stline);
}

static void			end(t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- END ------\n");
	while ((stline->pos_line) < (int)ft_strlen(stline->line))
		move(RIGHT, stline);
}

int					spec_key(int key, t_line *stline)
{
	if (DEBUG_TERMCAPS == 1)
		printf("------- SPEC KEY ------\n");
	if (key == DEL)
		del(stline);
	else if (key == HOME)
		home(stline);
	else if (key == END)
		end(stline);
	else
		return (-1);
	return (0);
}
