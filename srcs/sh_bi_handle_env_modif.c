#include "shell.h"
#include "libft.h"

static int			add_env(char *name, char *value) //static ac change env
{
	if (DEBUG == 1)
		ft_putendl_fd("------- ADD ENV ------", 2);
	t_duo				*env;

	env = savior(NULL, FALSE);
	if (name == NULL)
		return (ERROR);
/* MSG ret: ERROR exit: TRUE msg: "value not set" */
	else
	{
		duo_pushback(&env, name, value);
		savior(env, TRUE);
	}
	return (TRUE);
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
			ft_strdel(&(env->value));
			env->value = ft_strdup(value);
			return (TRUE);
		}
		env = env->next;
	}
	add_env(name, value);
	return (TRUE);
}

char				*get_env(char *name)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- GET ENV ------", 2);
	t_duo				*env;
	char				*tmp;

	tmp = NULL;
	env = savior(NULL, FALSE);
	while (env)
	{
		if (ft_strcmp(name, env->name) == 0)
		{
			if (env->value != NULL && ((tmp = ft_strdup(env->value)) == NULL)) // MALLOC
				return (NULL);
				/* MSG ret: NULL exit: TRUE msg: malloc error */
			return (tmp);
		}
		env = env->next;
	}
	return (NULL);
}
