#include "shell.h"
#include "libft.h"

static char			*join_exe(char *s1, char *s2) //static ac check fct
{
	if (DEBUG == 1)
		ft_putendl_fd("------- JOIN EXE ------", 2);

	char				*rlt;
	char				*tmp;

	if (s2[0] == '/' || (s2[0] == '.' && s2[1] == '/'))
		return (ft_strdup(s2));
	tmp = ft_strjoin("/", s2);
	rlt = ft_strjoin(s1, tmp);
	ft_strdel(&tmp);
	return (rlt);
}

int			null_input(int fd) // static ac check fct
{
	if (DEBUG == 1)
		ft_putendl_fd("------- NULL INPUT ------", 2);

	int					pfd[2];

	if (fd == -1)
	{
		if (pipe(pfd) == ERROR)
			return (sh_error(TRUE, 4, NULL, NULL));
		close(pfd[0]);
		write(pfd[1], "\0", 1);
		if (dup2(pfd[1], fd) == ERROR)
			return (sh_error(TRUE, 7, NULL, NULL));
		close(pfd[1]);
		return (-2);
	}
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
		return (sh_error(TRUE, 6, NULL, NULL));
	if ((tmp = get_env("PATH")) == NULL)
	{
		free_tab(&tbl_env);
		return (sh_error(TRUE, 12, NULL, NULL));
	}
	if ((path = ft_strsplit(tmp, ':')) == NULL)
	{
		free_tab(&tbl_env);
		return (ERROR);
	}
	ft_strdel(&tmp);
	i = -1;
	while (path[++i])
	{
		tmp = join_exe(path[i], cmd[0]);
		if (access(tmp, F_OK) != ERROR)
		{
			if (access(tmp, X_OK) == ERROR)
			{
				sh_error(TRUE, 20, cmd[0], NULL);
				free_tab(&tbl_env);
				free_tab(&path);
				ft_strdel(&tmp);
				return (-2);
			}
			execve(tmp, cmd, tbl_env);
		}
		ft_strdel(&tmp);
	}
	free_tab(&tbl_env);
	free_tab(&path);
	ft_strdel(&tmp);
	return (FALSE);
}
