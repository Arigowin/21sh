#include "shell.h"
#include "libft.h"

static char			*join_exe(char *s1, char *s2)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- JOIN EXE ------", 2);

	char				*rlt;
	char				*tmp;

	if (s2[0] == '/' || (s2[0] == '.' && s2[1] == '/'))
		return (s2);
	tmp = ft_strjoin("/", s2);
	rlt = ft_strjoin(s1, tmp);
	ft_strdel(&tmp);
	return (rlt);
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

	(void)fd;
	int					hrd_fd[2];

	if (fd == -1)
	{
		if (pipe(hrd_fd) == ERROR)
			/* RET: error EXIT: false MSG: "pipe fail"
			 * FREE: str */
			return (ERROR);
		write(hrd_fd[1], "", 0);
		dup2(hrd_fd[0], fd);
		close(hrd_fd[0]);
		close(hrd_fd[1]);
		return (-2);
	}
//	if (fd == -1)
//		return (FALSE);
	path = NULL;
	env = savior(NULL, FALSE);
	tbl_env = duo_to_tbl(env, "=");
	tmp = get_env("PATH");
	if (tmp == NULL || tbl_env == NULL)
		/* RET: error EXIT: true MSG: "env not set" */
		return (ERROR);
	if ((path = ft_strsplit(tmp, ':')) == NULL)
		/* RET: error EXIT: true MSG: "split fail" */
		return (ERROR);
	free(tmp);
	i = 0;
	while (path[i])
	{
		tmp = join_exe(path[i], cmd[0]);
		execve(tmp, cmd, tbl_env);
		free(tmp);
		i++;
	}
	return (TRUE);
}
