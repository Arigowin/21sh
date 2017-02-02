#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int					bi_echo(char **arg, t_duo **env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI ECHO ------", 2);

	(void)env;
	int i;

	i = 1;
	if (check_opt(arg, &i) == ERROR)
		return (ERROR);
	if (tbl_len(arg) > 1)
	{
		while (arg && arg[i])
		{
			ft_putstr_fd(arg[i], 1);
			if (arg[i + 1])
				ft_putchar(' ');
			i++;
		}
	}
	ft_putchar('\n');
	return (TRUE);
}
