#include <stdio.h>
#include "shell.h"
#include "libft.h"

static char		*get_path(void)
{
	if (DEBUG == 1)
		printf("------- GET PATH ------\n");
	char			*path;
	char			*tmp;
	char			*home;

	home = get_env("HOME");
	path = get_env("PWD");
	tmp = ft_strsub(path, 0, ft_strlen(home));
	if (home && ft_strcmp(home, tmp) == 0)
	{
		free(tmp);
		tmp = ft_strsub(path, ft_strlen(home), ft_strlen(path));
		free(path);
		path = ft_properjoin("~", tmp);
	}
	free(home);
	free(tmp);
	return (path);
}

int				display_prompt(void)
{
	if (DEBUG == 1)
		printf("------- DISPLAY PROMPT ------\n");
	char			*name;
	char			*path;

	path = get_path();
	name = get_env("LOGNAME");
	if (name)
	{
		ft_putstr("\033[34;1m");
		ft_putstr(name);
		ft_putstr("\033[0m:");
	}
	if (path)
	{
		ft_putstr("\033[32;1m");
		ft_putstr(path);
		ft_putstr("\033[0m");
	}
	if (path || name)
		ft_putchar('\n');
	ft_putstr("\033[36m> \033[0m");
	free(name);
	free(path);
	return (0);
}

int				fill_path(char ***env)
{
	if (DEBUG == 1)
		printf("------- FILL PATH ------\n");
	char			*tmp;

	tmp = NULL;
	if (((*env) = (char **)malloc(sizeof(char *) * 3)) == NULL)
		return (-1);
	if ((tmp = getcwd(tmp, MAX_PATH)) == NULL)
		return (-1);
	(*env)[0] = ft_strdup("PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
	(*env)[1] = ft_properjoin("PWD=", tmp);
	(*env)[2] = NULL;
	free(tmp);
	return (0);
}
