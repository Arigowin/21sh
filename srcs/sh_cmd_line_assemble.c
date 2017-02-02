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

int					check_fct(char **cmd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- CHECK FCT------", 2);

	char				**path;
	char				*tmp;
	t_duo				*env;
	int					i;
	char				**tbl_env;

	path = NULL;
	env = savior(NULL, FALSE);
	tbl_env = duo_to_tbl(env, "=");
	tmp = get_env("PATH");
	if (tmp == NULL || tbl_env == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
		return (ERROR);
	if ((path = ft_strsplit(tmp, ':')) == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
		return (ERROR);
	free(tmp);
	i = 0;
	while (path[i])
	{
		tmp = join_exe(path[i], cmd[0]);
		if (access(tmp, F_OK) != ERROR && access(tmp, X_OK) == ERROR)
		{
			ft_putstr_fd("21sh: ", 2);
			ft_putstr_fd(cmd[0], 2);
			ft_putendl_fd(": Permission denied", 2);
		}
		execve(tmp, cmd, tbl_env);
		free(tmp);
		i++;
	}
	return (TRUE);
}
