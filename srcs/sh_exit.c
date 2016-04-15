#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int				bi_exit(char **arg, t_duo **env)
{
	int			i;

	i = 0;
	if (arg && arg[1] && arg[2])
	{
		ft_putendl("21sh: exit : too many arguments");
		return (-1);
	}
	while (arg && arg[1] && arg[1][i])
	{
		if (ft_isdigit(arg[1][i]) == 0)
		{
			ft_putstr("21sh :exit : ");
			ft_putstr(arg[1]);
			ft_putendl(": numeric argument required");
			return (-1);
		}
		i++;
	}
	i = (arg && arg[1] ? ft_atoi(arg[1]) : 0);
	ft_putendl("exit");
	free_tab(&arg);
	duo_del(env);
	exit(i);
	return (0);
}
