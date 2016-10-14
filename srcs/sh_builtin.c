#include "shell.h"
#include "libft.h"
#include <stdio.h>

int				add_env(char *name, char *value)
{
	if (DEBUG == 1)
		printf("------- ADD ENV ------\n");
	t_duo				**env;

	env = savior(NULL, FALSE);
	if (name == NULL)
		return (-1);
	else
	{
		duo_pushback(env, name, value);
		savior(env, TRUE);
	}
	free(env);
	return (0);
}

int				change_env(t_duo **env, char *name, char *value)
{
	if (DEBUG == 1)
		printf("------- CHANGE ENV ------\n");
	t_duo				**env;

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
	free(env);
	return (0);
}

char			*get_env(char *name)
{
	if (DEBUG == 1)
		printf("------- GET ENV ------\n");
	t_duo				**env;

	env = savior(NULL, FALSE);
	while (*env)
	{
		if (ft_strcmp(name, (*env)->name) == 0)
			return (ft_strdup((*env)->value));
		*env = (*env)->next;
	}
	free(env);
	return (NULL);
}

int				handle_builtin(char **cmd)
{
	if (DEBUG == 1)
		printf("------- HANDLE BUILTIN ------\n");
	static const char	*bi[] = {"cd", "setenv", "unsetenv", "env", "exit"};
	static int			(*fct_tbl[])(char **cmd, t_duo **env) = {&bi_cd,
		&bi_setenv, &bi_unsetenv, &bi_env, &bi_exit};
	int					i;
	t_duo				**env;

	env = savior(NULL, FALSE);
	i = 0;
	while (i < 5 && ft_strcmp(cmd[0], bi[i]) != 0)
		i++;
	if (i < 5 && ft_strcmp(cmd[0], bi[i]) == 0)
	{
		if (fct_tbl[i](cmd, env) == -1)
			return (-1);
		else
			return (1);
	}
	free(env);
	return (0);
}
