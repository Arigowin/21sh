#include "libft.h"

int					ft_strncount(char *str, char c)
{
	int					count;
	int					i;

	count = 0;
	i = 0;
	if (!str)
		return (-1);

	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}
