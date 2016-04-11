#include "minishell.h"
#include "libft.h"

int				event(char buf[], char **line, int *x)
{
//	printf("%d, %d, %d\n", buf[0], buf[1], buf[2]);
	if (buf[0] == 10)
	{
		ft_putchar(buf[0]);
		return (1);
	}
	else if (buf[1] == 0)
	{
		if (buf[0] == 127)
			backspace(line, x);
		else if (buf[0] == 126)
			del(line, x);
		else if (buf[0] != 9)
			insert(line, buf[0], ++(*x) - 4);
	}
	else if (buf[0] == 27)
		move(buf[0] + buf[1] + buf[2], x, *line);
	ft_bzero(buf, 3);
	return (0);
}
