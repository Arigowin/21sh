#include "libft.h"

int				event(char buf[3])
{
	if (buf[0] == 10)
	{
		ft_putchar(buf[0]);
		return (0);
	}
	else if (buf[1] == 0)
	{
		if (buf[0] == 127)
			backspace(&line, &x);
		else if (buf[0] != 9)
			insert(&line, buf[0], ++x - 4);
	}
	else if (buf[0] == 27)
		move(buf[0] + buf[1] + buf[2], &x, line);
	ft_bzero(buf, 3);
	return (0);
}

//t_bool			readline(char ***t)
//{
//	char	buf[3];
//	int		ret;
//	char	line[BUFF_S];
//	int x;
//
//	(void)t;
//	ft_bzero(line, BUFF_S);
//	x  = 3;
//	while ((ret = read(0, buf, 3)) > 0)
//	{
//		buf[ret] = '\0';
//		if (buf[0] == 10)
//		{
//			ft_putchar(buf[0]);
//			break ;
//		}
//		else if (buf[1] == 0)
//		{
//			if (buf[0] == 127)
//				backspace(&line, &x);
//			else if (buf[0] != 9)
//				insert(&line, buf[0], ++x - 4);
//		}
//		else if (buf[0] == 27)
//			move(buf[0] + buf[1] + buf[2], &x, line);
//		ft_bzero(buf, 3);
//	}
//	if (ret == -1)
//		return (FALSE);
//	if ((*t = lexer(line, SEP)) == NULL)
//		return (FALSE);
//	return (TRUE);
//}
