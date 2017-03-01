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
		if (stat(path, &stat_buf) == 0 && !S_ISDIR(stat_buf.st_mode))
			return (sh_error(FALSE, 16, path, NULL));
		else if ((access(path, F_OK)) == ERROR)
			return (sh_error(FALSE, 17, path, NULL));
		else
			return (sh_error(FALSE, 18, path, NULL));
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
		return (sh_error(FALSE, 13, NULL, NULL));
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
			return (sh_error(TRUE, 11, NULL, NULL));
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
