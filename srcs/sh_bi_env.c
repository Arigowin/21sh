#include "shell.h"
#include "libft.h"

static int			print_env(char **arg, t_duo **env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- PRINT MODIFIED ENV ------", 2);

	t_duo				*cpy;
	int					i;

	cpy = *env;
	i = 0;
	if (arg[1] && arg[2])
		return (ERROR);
	while (arg[1] && arg[1][i])
	{
		if (arg[1][i] == '=')
			break ;
		i++;
	}
	if (arg[1] && arg[1][i] == '\0')
		return (ERROR);
	while (cpy)
	{
		ft_putstr(cpy->name);
		ft_putchar('=');
		ft_putendl(cpy->value);
		cpy = cpy->next;
	}
	ft_putendl(arg[1]);
	return (TRUE);
}

int					bi_env(char **arg, t_duo **env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI ENV ------", 2);

	t_duo				*cpy;

	cpy = *env;
	if (arg[1])
	{
		if (print_env(arg, env) == -1)
		{
			ft_putstr("21sh :env :");
			ft_putstr(arg[1]);
			ft_putendl(": No such file or directory");
			return (-1);
		}
	}
	else
	{
		while (cpy)
		{
			ft_putstr(cpy->name);
			ft_putchar('=');
			ft_putendl(cpy->value);
			cpy = cpy->next;
		}
	}
	return (TRUE);
}
