#include "shell.h"
#include "libft.h"

int				event(char buf[], t_line *stline)
{
//	printf("%d, %d, %d\n", buf[0], buf[1], buf[2]);
	if (buf[0] == RETURN)
	{
		ft_putchar(buf[0]);
		return (1);
	}
	else if (buf[1] == 0)
	{
		if (buf[0] == BACKSPACE)
			backspace(stline);
		else if (buf[0] == DEL)
			del(stline);
		else if (buf[0] != TAB)
			insert(stline, buf[0], ++(stline->curs_x) - 4);
	}
	else if (buf[0] == 27 || buf[0] == 59)
		move(buf[0] + buf[1] + buf[2], stline);
	ft_bzero(buf, 3);
	return (0);
}
