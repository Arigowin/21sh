#include <sys/stat.h>
#include "shell.h"
#include "libft.h"

static char			*join_exe(char *s1, char *s2) //static ac check fct
{
	if (DEBUG == 1)
		ft_putendl_fd("------- JOIN EXE ------", 2);

	char				*rlt;
	char				*tmp;

	if (s2 && (s2[0] == '/' || (s2[0] == '.' && s2[1] && s2[1] == '/')))
	{
		if ((tmp = ft_strdup(s2)) == NULL)
			sh_error(FALSE, 6, NULL, NULL);
		return (tmp);
	}
	tmp = ft_strjoin("/", s2);
	rlt = ft_strjoin(s1, tmp);
	ft_strdel(&tmp);
	return (rlt);
}

int					null_input(int fd) // static ac check fct
{
	if (DEBUG == 1)
		ft_putendl_fd("------- NULL INPUT ------", 2);

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

int					ft_is_dir(char *path)
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

int					check_fct(int fd, char **cmd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- CHECK FCT------", 2);

	char				**path;
	char				*tmp;
	t_duo				*env;
	int					i;
	char				**tbl_env;

	null_input(fd);
	env = savior(NULL, FALSE);
	if ((tbl_env = duo_to_tbl(env, "=")) == NULL)
		return (sh_error(FALSE, 6, NULL, NULL));
	if ((tmp = get_env("PATH")) == NULL)
		return (error_clear_tab(FALSE, 12, NULL, &tbl_env));
	if ((path = ft_strsplit(tmp, ':')) == NULL)
		return (str_dbltbl_ret(ERROR, NULL, &tbl_env, NULL));
	ft_strdel(&tmp);
	i = -1;
	while (path[++i])
	{
		tmp = join_exe(path[i], cmd[0]);
		check_rights(tmp, path, cmd, tbl_env);
	}
	return (str_dbltbl_ret(FALSE, &tmp, &tbl_env, &path));
}
