#include "shell.h"
#include "libft.h"
#include <stdio.h>

static char		*save_start(char *str, int *i)
{
	if (DEBUG == 1)
		printf("----------- SAVE START ----------\n");
	char		*start;

	*i = 0;
	start = NULL;
	while (str[*i] && str[*i] != '$')
		(*i)++;
	if (str[*i - 1] && str[*i - 1] == '\\')
		return (NULL);
	if (*i != 0)
	{
		if ((start = ft_strsub(str, 0, *i)) == NULL)
			return (NULL);
	}
	return (start);
}


static char		*save_env(char *str, int i, int *j)
{
	if (DEBUG == 1)
		printf("----------- SAVE ENV ----------\n");
	char	*env;

	env = NULL;
	while (str[*j] && str[*j] != ' ' && str[*j] != '/')
		(*j)++;
	if ((env = ft_strsub(str, i + 1, *j - i)) == NULL)
		return (NULL);
	return (env);
}

static char		*save_end(char *str, int j)
{
	if (DEBUG == 1)
		printf("----------- SAVE END ----------\n");
	char	*end;

	end = NULL;
	if (str[j + 1])
	{
		if ((end = ft_strsub(str, j, ft_strlen(str) - j)) == NULL)
			return (NULL);
	}
	return (end);
}

static int		replace_dollar_bis(char **str, char *var[], char *env_content)
{
	if (DEBUG == 1)
		printf("--------------- REPLACE DOLLAR BIS ---------------\n");
	char		*tmp;

	tmp = NULL;
	if (var[0] != NULL)
	{
		if (env_content != NULL)
			tmp = ft_strjoin(var[0], env_content);
		else
			tmp = ft_strdup(var[0]);
	}
	else
		tmp = ft_strdup(env_content);
	if (var[2] != NULL)
	{
		if (tmp != NULL)
			*str = ft_strjoin(tmp, var[2]);
		else
			*str = ft_strdup(var[2]);
		if (*str == NULL)
			return (ERROR);
	}
	else
		*str = ft_strdup(tmp);
	ft_strdel(&tmp);
	return (TRUE);
}

int				replace_dollar(char **str)
{
	if (DEBUG == 1)
		printf("--------------- REPLACE DOLLAR ---------------\n");
	int		i;
	int		j;
	char	*var[3];
	char	*env_content;

	env_content = NULL;
	if (ft_strchr(*str, '$'))
	{
		var[0] = save_start(*str, &i);
		j = i + 1;
		var[1] = save_env(*str, i, &j);
		var[2] = save_end(*str, j);
		env_content = get_env(var[1]);
		ft_strdel(str);
		if (replace_dollar_bis(str, var, env_content) == ERROR)
			return (ERROR);
		ft_strdel(&(var[1]));
		ft_strdel(&(var[0]));
		ft_strdel(&(var[2]));
		ft_strdel(&env_content);
	}
	return (TRUE);
}
