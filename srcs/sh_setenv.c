#include "shell.h"
#include "libft.h"

static int			is_valid(char *str)
{
	if (DEBUG_BUILTIN == 1)
		printf("------- IS VALID ------\n");

	int					i;

	i = -1;
	if (str[++i] != '_' &&  ft_isalpha(str[i]) == FALSE)
	{
		ft_putstr_fd("21sh: setenv: '", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (ERROR);
	}
	while (str[i])
	{
		if (str[i] != '_' && str[i] != '.' && ft_isalnum(str[i]) == FALSE)
		{
			ft_putstr_fd("21sh: setenv: '", 2);
			ft_putstr_fd(str, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			return (ERROR);
		}
		i++;
	}
	return (0);
}

int					bi_setenv(char **arg, t_duo **env)
{
	if (DEBUG_BUILTIN == 1)
		printf("------- BI SETENV ------\n");

	int					i;

	i = 0;
	while (arg[i])
	{
		if (i == 1 && is_valid(arg[i]) != 0)
			return (ERROR);
		i++;
	}
	if (i < 2)
	{
		bi_env(arg, env);
		return (0);
	}
	if (i == 2 || i == 3)
	{
		change_env(arg[1], arg[2]);
		return (0);
	}
	if (i > 3)
	{
		ft_putendl_fd("21sh: setenv: too many arguments.", 2);
		return (ERROR);
	}
	return (0);
}
