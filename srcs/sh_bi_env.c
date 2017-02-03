#include "shell.h"
#include "libft.h"

static int			print_env(t_duo *env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- PRINT ENV ------", 2);

	while (env)
	{
		ft_putstr(env->name);
		ft_putchar('=');
		ft_putendl(env->value);
		env = env->next;
	}
	return (TRUE);
}

static int			modif_env(char **arg, t_duo *env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- MODIF ENV ------", 2);

	int					i;
	int					j;
	char				*key;
	char				*value;

	i = 0;
	j = 0;
	while (arg[i])
	{
		if (strchr(arg[i], '=') != NULL)
		{
			while (arg[i][j] != '=')
				j++;
			key = ft_strsub(arg[i], 0, j);
			j++;
			value = ft_strsub(arg[i], j, ft_strlen(arg[i]) - j);
			duo_pushback(&env, key, value);
		}
		else
		{
			if (access(arg[i], F_OK) == ERROR)
			{
				ft_putstr("21sh :env :");
				ft_putstr(arg[1]);
				ft_putendl(": No such file or directory");
				return (ERROR);
			}
			if (ft_strstr(arg[i], "./") != NULL)
			{
				if (access(arg[i], X_OK) == ERROR)
				{
					ft_putstr("21sh :env :");
					ft_putstr(arg[1]);
					ft_putendl(": Permission denied");
					return (ERROR);
				}
			}
		}
		i++;
	}
	print_env(env);
	return (TRUE);
	//	t_duo				*cpy;
	//	int					i;
	//
	//	cpy = *env;
	//	i = 0;
	//	if (arg[1] && arg[2])
	//		return (ERROR);
	//	while (arg[1] && arg[1][i])
	//	{
	//		if (arg[1][i] == '=')
	//			break ;
	//		i++;
	//	}
	//	if (arg[1] && arg[1][i] == '\0')
	//		return (ERROR);
	//	while (cpy)
	//	{
	//		ft_putstr(cpy->name);
	//		ft_putchar('=');
	//		ft_putendl(cpy->value);
	//		cpy = cpy->next;
	//	}
	//	ft_putendl(arg[1]);
	//	return (TRUE);
}

int					bi_env(char **arg, t_duo **env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI ENV ------", 2);

	if (tbl_len(arg) > 1)
	{
		if (modif_env(arg, *env) == ERROR)
		{
			return (ERROR);
		}
	}
	else
		print_env(*env);
	return (TRUE);
}
