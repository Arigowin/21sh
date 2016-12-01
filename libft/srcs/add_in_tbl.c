#include "libft.h"

int 			add_in_tbl(char **tbl, char c)
{
	int				i;

	i = 0;
	if (!(tbl && *tbl))
		return (-1);
	i = ft_strlen(*tbl);
	(*tbl)[i] = c;
	return (0);
}
