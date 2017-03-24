#include <sys/stat.h>
#include "shell.h"
#include "libft.h"

static int			ft_is_dir(char *path)
{
	struct stat			file_stat;

	if (stat(path, &file_stat) == ERROR)
		return (sh_error(ERROR, 32, NULL, NULL));
	if (S_ISDIR(file_stat.st_mode))
		return (TRUE);
	return (FALSE);
}

static int			check_rights(char *tmp, char **path, char **cmd,
					char **tbl_env)
{
	if (access(tmp, F_OK) != ERROR)
	{
		if (access(tmp, X_OK) == ERROR)
		{
			sh_error(FALSE, 20, cmd[0], NULL);
			return (str_dbltbl_ret(-2, &tmp, &tbl_env, &path));
		}
		else if (ft_is_dir(tmp))
		{
			sh_error(FALSE, 33, cmd[0], NULL);
			return (str_dbltbl_ret(-2, &tmp, &tbl_env, &path));
		}
		execve(tmp, cmd, tbl_env);
	}
	ft_strdel(&tmp);
	return (TRUE);
}

static int			join_exe(int i, char **cmd, char **path, char **tbl_env)
{
	char				*rlt;
	char				*tmp;

	rlt = NULL;
	if (cmd[0] && (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] && cmd[0][1] == '/')))
	{
		if ((rlt = ft_strdup(cmd[0])) == NULL)
			return (sh_error(FALSE, 6, NULL, NULL));
	}
	else if (path && *path)
	{
		tmp = ft_strjoin("/", cmd[0]);
		rlt = ft_strjoin(path[i], tmp);
		ft_strdel(&tmp);
	}
	if (check_rights(rlt, path, cmd, tbl_env) == -2)
		return (-2);
	return (TRUE);
}

int					null_input(int fd)
{
	int					pfd[2];

	if (fd == -1)
	{
		if (pipe(pfd) == ERROR)
			return (sh_error(FALSE, 4, NULL, NULL));
		close(pfd[0]);
		write(pfd[1], "\0", 1);
		if (dup2(pfd[1], fd) == ERROR)
			return (sh_error(FALSE, 7, NULL, NULL));
		close(pfd[1]);
		return (-2);
	}
	return (TRUE);
}

int					check_fct(int fd, char **cmd)
{
	char				**path;
	char				*tmp;
	t_duo				*env;
	int					i;
	char				**tbl_env;

	tmp = NULL;
	path = NULL;
	null_input(fd);
	env = savior_env(NULL, FALSE);
	if ((tbl_env = duo_to_tbl(env, "=")) == NULL)
		return (sh_error(FALSE, 6, NULL, NULL));
	tmp = get_env("PATH");
	if (tmp && (path = ft_strsplit(tmp, ':')) == NULL)
		return (str_dbltbl_ret(ERROR, NULL, &tbl_env, NULL));
	ft_strdel(&tmp);
	i = -1;
	while (path && path[++i])
	{
		if (join_exe(i, cmd, path, tbl_env) == -2)
			return (-2);
	}
	if (path == NULL && join_exe(0, cmd, NULL, tbl_env) == -2)
		return (-2);
	return (str_dbltbl_ret(FALSE, NULL, &tbl_env, &path));
}
