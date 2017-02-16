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
		ft_strdel(&tmp);
		tmp = ft_strsub(path, ft_strlen(home), ft_strlen(path));
		ft_strdel(&path);
		path = ft_properjoin("~", tmp);
	}
	ft_strdel(&home);
	ft_strdel(&tmp);
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
	ft_strdel(&name);
	ft_strdel(&path);
	return (TRUE);
}

int				fill_path(char ***env)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- FILL PATH ------", 2);

	char				*tmp;

	tmp = NULL;
	if (((*env) = (char **)malloc(sizeof(char *) * 4)) == NULL)
		return (sh_error(TRUE, 6, NULL, NULL));
	if ((tmp = getcwd(tmp, MAX_PATH)) == NULL)
		return (sh_error(TRUE, 6, NULL, NULL));
	if (((*env)[0] =
	ft_strdup("PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin")) == NULL)
		return (sh_error(TRUE, 6, NULL, NULL));
	if (((*env)[1] = ft_properjoin("PWD=", tmp)) == NULL)
	{
		ft_strdel(&tmp);
		return (sh_error(TRUE, 6, NULL, NULL));
	}
	if (((*env)[2] = ft_strdup("TERM=xterm")) == NULL)
		return (sh_error(TRUE, 6, NULL, NULL));
	(*env)[3] = NULL;
	ft_strdel(&tmp);
	return (TRUE);
}
