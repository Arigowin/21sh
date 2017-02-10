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
			/* RET: error EXIT: false MSG: "pipe fail" */
			return (ERROR);
		close(pfd[0]);
		write(pfd[1], "\0", 1);
		if (dup2(pfd[1], fd) == ERROR)
			/* RET: error EXIT: false MSG: "dup fail"*/
			return (ERROR);
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
	tbl_env = duo_to_tbl(env, "=");
	tmp = get_env("PATH");
	if (tmp == NULL || tbl_env == NULL)
	{
/* RET: error EXIT: true MSG: "env not set" */
		free_tab(&tbl_env);
		return (ERROR);
	}
	if ((path = ft_strsplit(tmp, ':')) == NULL)
	{
/* RET: error EXIT: true MSG: "split fail" */
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
				ft_putstr_fd("21sh: ", 2); //fct erreur
				ft_putstr_fd(cmd[0], 2);
				ft_putendl_fd(": Permission denied", 2);
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
	return (TRUE);
}
