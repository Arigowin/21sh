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
		{
			ft_putendl(": not a directory");
		}
		else if ((access(path, F_OK)) == ERROR)
		{
			ft_putendl(": no such file or directory");
		}
		else
		{
			ft_putendl(": permission denied");
		}
		return (ERROR);
	}
	ft_strdel(&path);
	return (TRUE);
}

int					cd_home()
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- CD HOME ------", 2);

	char 				*path;
	int					ret;

	if ((path = get_env("HOME")) == NULL)
	{
		ft_putendl_fd("21sh: cd: no HOME variable set.", 2);
		return (ERROR);
	}
	ret = change_dir(path);
	return (ret);
}

int					bi_opt(char *arg, int *no_more, char *handled_opt)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI OPT ------", 2);

	int					i;

	i = 1;
	if (*no_more == TRUE)
		return (FALSE);
	if (arg && arg[0] && arg[0] == '-' && arg[1] && arg[1] == '-')
		*no_more = TRUE;
	else if (arg && arg[0] && arg[0] == '-' && arg[1])
	{
		while (arg[i])
		{
			if (ft_strchr(handled_opt, arg[i]) == NULL)
			{
				ft_putstr_fd(arg, 2);
				ft_putendl_fd(": invalid option.", 2);
				//	cd_error(1, arg); // invalid option
				return (ERROR);
			}
			i++;
		}
	}
	return (TRUE);
}

int					check_opt(char **arg, int *i)
{
	int					no_more;
	int					ret;

	ret = FALSE;
	no_more = FALSE;
	while (arg[*i] && arg[*i][0] && arg[*i][0] == '-' && arg[*i][1])
	{
		if ((ret = bi_opt(arg[*i], &no_more, "")) != TRUE)
			break ;
		(*i)++;
	}
	if (ret == ERROR)
		return (ERROR);
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

	i = 1;
	ret = 0;
	(void)env;
	path = NULL;
	tmp = get_env("OLDPWD");
	if (check_opt(arg, &i) == ERROR)
	{
		ft_strdel(&tmp);
		return (ERROR);
	}
	if (!arg[i] || (arg[i] && arg[i][0] == '~' && !arg[i][1]))
		ret = cd_home();
	else if (arg[i] && arg[i][0] == '-' && !arg[i][1])
	{
		if (tmp)
			ret = change_dir(tmp);
		else
			ft_putendl_fd("21sh: cd: no OLDPWD variable set.", 2);
	}
	else
		ret = change_dir(arg[i]);
	ft_strdel(&tmp);
	tmp = get_env("PWD");
	if (ret == TRUE)
	{
		change_env("OLDPWD", tmp);
		path = getcwd(path, MAX_PATH);
		change_env("PWD", path);
		ft_strdel(&tmp);
		ft_strdel(&path);
		return (0);
	}
	ft_strdel(&path);
	ft_strdel(&tmp);
	return (ERROR);
}
