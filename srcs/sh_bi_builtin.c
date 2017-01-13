#include "shell.h"
#include "libft.h"
#include <stdio.h>

int					add_env(char *name, char *value)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- ADD ENV ------", 2);
	t_duo				*env;

	env = savior(NULL, FALSE);
	if (name == NULL)
		return (-1);
	else
	{
		duo_pushback(&env, name, value);
		savior(env, TRUE);
	}
	return (0);
}

int					change_env(char *name, char *value)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- CHANGE ENV ------", 2);
	t_duo				*env;

	env = savior(NULL, FALSE);
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			return (1);
		}
		env = env->next;
	}
	add_env(name, value);
	return (0);
}

char				*get_env(char *name)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- GET ENV ------", 2);
	t_duo				*env;

	env = savior(NULL, FALSE);
	while (env)
	{
		if (ft_strcmp(name, env->name) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

int					is_builtin(char **cmd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- IS BUILTIN ------", 2);

	static const char	*bi[] = {"cd", "setenv", "unsetenv", "env", "exit"};
	int					i;

	i = 0;
	while (i < 5)
	{
		if (ft_strcmp(cmd[0], bi[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int					handle_builtin(char **cmd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- HANDLE BUILTIN ------", 2);

	int					i;
	t_duo				*env;
	static const char	*bi[] = {"cd", "setenv", "unsetenv", "env", "exit"};
	static int			(*tbl_bi[])(char **cmd, t_duo **env) = {&bi_cd,
							&bi_setenv, &bi_unsetenv, &bi_env, &bi_exit};

	env = savior(NULL, FALSE);
	i = 0;
	while (i < 5 && ft_strcmp(cmd[0], bi[i]) != 0)
		i++;
	if (i < 5 && ft_strcmp(cmd[0], bi[i]) == 0)
	{
		if (tbl_bi[i](cmd, &env) == ERROR)
			return (ERROR);
		else
			return (TRUE);
	}
	return (FALSE);
}
