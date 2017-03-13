#include "shell.h"
#include "libft.h"

int					valid_env_name(char *str, char *bi)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- IS VALID ------", 2);

	int					i;

	i = -1;
	if (str[++i] != '_' && ft_isalpha(str[i]) == FALSE)
		return (sh_error(FALSE, 15, str, bi));
	while (str[i])
	{
		if (str[i] != '_' && str[i] != '.' && ft_isalnum(str[i]) == FALSE)
			return (sh_error(FALSE, 15, str, bi));
		i++;
	}
	return (TRUE);
}

int					bi_setenv(char **arg, t_duo **env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI SETENV ------", 2);

	int					i;

	i = 0;
	while (arg[i])
	{
		if (i == 1 && valid_env_name(arg[i], "setenv") == FALSE)
			return (FALSE);
		i++;
	}
	if (i < 2)
	{
		return (bi_env(arg, env));
	}
	if (i == 2 || i == 3)
	{
		change_env(arg[1], arg[2]);
		return (TRUE);
	}
	if (i > 3)
		return (sh_error(FALSE, 10, NULL, NULL));
	return (TRUE);
}
