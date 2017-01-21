#include <stdio.h>
#include "shell.h"
#include "libft.h"

static char			*get_path(void)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- GET PATH ------", 2);

	char				*path;
	char				*tmp;
	char				*home;

	if ((home = get_env("HOME")) == NULL)
		/* RET: null EXIT: false MSG: "HOME not set" */
		return (NULL);
	if ((path = get_env("PWD")) == NULL)
		/* RET: null EXIT: false MSG: "PWD not set" */
		return (NULL);
	if ((tmp = ft_strsub(path, 0, ft_strlen(home))) == NULL)
		/* RET: null EXIT: false MSG: "malloc fail" */
		return (NULL);
	if (home && ft_strcmp(home, tmp) == 0)
	{
		free(tmp);
		tmp = ft_strsub(path, ft_strlen(home), ft_strlen(path));
		free(path);
		path = ft_properjoin("~", tmp);
	}
	if (home)
		free(home);
	if (tmp)
		free(tmp);
	return (path);
}

int					display_prompt(void)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- DISPLAY PROMPT ------", 2);

	char				*name;
	char				*path;

	path = get_path(); //free ok
	name = get_env("LOGNAME"); //free ok
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
	return (TRUE);
}

int				fill_path(char ***env)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- FILL PATH ------", 2);

	char				*tmp;

	tmp = NULL;
	if (((*env) = (char **)malloc(sizeof(char *) * 3)) == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
		return (ERROR);
	if ((tmp = getcwd(tmp, MAX_PATH)) == NULL)
		/* RET: error EXIT: false MSG: "Cannot get current directory" */
		return (ERROR);
	if (((*env)[0] = ft_strdup("PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin")) == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
		return (ERROR);
	(*env)[1] = ft_properjoin("PWD=", tmp);
	(*env)[2] = NULL;
	if (tmp)
		free(tmp);
	return (TRUE);
}
