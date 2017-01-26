#include <unistd.h>
#include "shell.h"
#include "libft.h"

static int			change_dir(char *path)
{
//	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- CHANGE DIR ------", 2);

		printf("****%p*****\n", path);
		printf("****%s*****\n", path);
	if (chdir(path) == -1)
	{
		printf("********\n");
		ft_putstr("21sh: cd: ");
		ft_putstr(path);
		if ((access(path, F_OK)) == ERROR)
			ft_putendl(": no such file or directory");
		else
			ft_putendl(": permission denied");
		return (ERROR);
	}
	ft_strdel(&path);
		printf("+++++++++\n");
	return (TRUE);
}

int					cd_home()
{
//	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- CD HOME ------", 2);

	char 				*path;
	int					ret;

	if ((path = get_env("HOME")) == NULL)
	{
		ft_putendl_fd("21sh: cd: no HOME variable set.", 2);
		return (ERROR);
	}
	ret = change_dir(path);
	printf("(((((%s)))))\n", path);
	return (ret);
}

int					bi_opt(char *arg, int *no_more, char *handled_opt)
{
//	if (DEBUG_BUILTIN == 1)
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

int					bi_cd(char **arg, t_duo **env)
{
//	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI CD ------", 2);

	char				*tmp_pwd;
	char				*tmp_old_pwd;
	char				*path;
	int					no_more;
	int					i;
	int					ret;

	i = 1;
	ret = 0;
	no_more = FALSE;
	(void)env;
	path = NULL;
	tmp_pwd = NULL;
	tmp_pwd = getcwd(tmp_pwd, MAX_PATH);
	tmp_old_pwd = get_env("OLDPWD");

	while (arg[i] && arg[i][0] && arg[i][0] == '-' && arg[i][1])
	{
		if ((ret = bi_opt(arg[i], &no_more, "")) != TRUE)
			break ;
		i++;
	}
	if (ret == ERROR)
		return (ERROR);
	if (arg[i])
		printf("{{{{%s}}}}\n", arg[i]);
	if (!arg[i] || (arg[i] && arg[i][0] == '~' && !arg[i][1]))
		ret = cd_home();
	else if (arg[i] && arg[i][0] == '-' && !arg[i][1])
	{
		if (tmp_old_pwd)
			ret = change_dir(tmp_old_pwd);
		else
			ft_putendl_fd("21sh: cd: no OLD_PWD variable set.", 2);
	}
	else
		ret = change_dir(arg[i]);
	if (ret == TRUE)
	{
		change_env("OLDPWD", tmp_pwd);
		path = getcwd(path, MAX_PATH);
		change_env("PWD", path);
		return (0);
	}
	return (ERROR);
}
