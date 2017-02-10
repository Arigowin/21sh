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

	home = get_env("HOME");
	path = get_env("PWD");
	tmp = ft_strsub(path, 0, ft_strlen(home));
	if (home && ft_strcmp(home, tmp) == 0)
	{
		if (tmp)
			free(tmp);
		tmp = ft_strsub(path, ft_strlen(home), ft_strlen(path));
		if (path)
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
		ft_putstr_color("\033[34;1m", name);
		ft_putstr(":");
	}
	if (path)
		ft_putstr_color("\033[32;1m", path);
	if (path || name)
		ft_putchar('\n');
	ft_putstr_color("\033[36m", "> ");
	if (name)
		free(name);
	if (path)
		free(path);
	return (TRUE);
}

int				fill_path(char ***env)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- FILL PATH ------", 2);

	char				*tmp;

	tmp = NULL;
	if (((*env) = (char **)malloc(sizeof(char *) * 4)) == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
		return (ERROR);
	if ((tmp = getcwd(tmp, MAX_PATH)) == NULL)
		/* RET: error EXIT: false MSG: "Cannot get current directory" */
		return (ERROR);
	if (((*env)[0] = ft_strdup("PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin")) == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
		return (ERROR);
	(*env)[1] = ft_properjoin("PWD=", tmp);
	(*env)[2] = ft_strdup("TERM=xterm");
	(*env)[3] = NULL;
	if (tmp)
		free(tmp);
	return (TRUE);
}
