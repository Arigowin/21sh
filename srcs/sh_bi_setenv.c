#include "shell.h"
#include "libft.h"

static int			is_valid(char *str)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- IS VALID ------", 2);

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
		if (i == 1 && !is_valid(arg[i]))
			return (ERROR);
		/* MSG ret: ERROR exit: FALSE msg: "" */
		// en cas d erreur on aura deja ecris l erreur dans is_valid
		i++;
	}
	// on affiche env si on a 1 seul arg et que arg[0] == env ou setenv
	if (i < 2) 
	{
		return (bi_env(arg, env));
//		return (TRUE);
	}
	// on change env si on a 2 ou 3 arg et que arg[1] existe dans env
	if (i == 2 || i == 3)
	{
		change_env(arg[1], arg[2]);
		return (TRUE);
	}
	// erreur si plus de 3 args
	if (i > 3)
	{
		ft_putendl_fd("21sh: setenv: too many arguments.", 2);
		return (ERROR);
		/* MSG ret: ERROR exit: FALSE msg: "" */
	}
	return (TRUE);
}
