#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "shell.h"
#include "libft.h"

static int			change_dir(char *path)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- CHANGE DIR ------", 2);

	struct stat			stat_buf;

	if (chdir(path) == -1)
	{
		ft_putstr("21sh: cd: ");
		ft_putstr(path);
		if (stat(path, &stat_buf) == 0 && !S_ISDIR(stat_buf.st_mode))
			ft_putendl_fd(": not a directory", 2);
		else if ((access(path, F_OK)) == ERROR)
			ft_putendl_fd(": no such file or directory", 2);
		else
			ft_putendl_fd(": permission denied", 2);
		return (FALSE);
	}
	return (TRUE);
}

static int			cd_home()
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- CD HOME ------", 2);

	char 				*path;
	int					ret;

	if ((path = get_env("HOME")) == NULL)
	{
		ft_putendl_fd("21sh: cd: no HOME variable set.", 2);
		return (FALSE);
	}
	ret = change_dir(path);
	ft_strdel(&path);
	return (ret);
}

static int			handle_cd_arg(int *i, int *ret, char **arg)
{
	char				*tmp;

	tmp = get_env("OLDPWD");
	if (check_opt(arg, i) == ERROR)
	{
		ft_strdel(&tmp);
		return (FALSE);
	}
	if (!arg[*i])
		*ret = cd_home();
	else if (arg[*i] && arg[*i][0] == '-' && !arg[*i][1])
	{
		if (tmp)
			*ret = change_dir(tmp);
		else
			ft_putendl_fd("21sh: cd: no OLDPWD variable set.", 2);
	}
	else
		*ret = change_dir(arg[*i]);
	ft_strdel(&tmp);
	return (TRUE);
}

int					bi_cd(char **arg, t_duo **env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI CD ------", 2);

	char				*tmp;
	char				*path;
	int					i;
	int					ret;

	(void)env;
	i = 1;
	ret = 0;
	tmp = NULL;
	path = NULL;
	if (handle_cd_arg(&i, &ret, arg) == FALSE)
		return (FALSE);
	if (ret == TRUE)
	{
		tmp = get_env("PWD");
		change_env("OLDPWD", tmp);
		path = getcwd(path, MAX_PATH);
		change_env("PWD", path);
	}
	ft_strdel(&tmp);
	ft_strdel(&path);
	return (FALSE);
}
