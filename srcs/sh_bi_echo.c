#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int					bi_echo(char **arg, t_duo **env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI ECHO ------", 2);

	(void)env;
	int i;

	i = 0;
	/*
	while (arg && arg[i])
	{
		ft_putendl_fd(arg[i], 1);
		i++;
	}
	*/
	if (!arg[i])
		ft_putendl_fd("", 1);
	return (TRUE);
}
