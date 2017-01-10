#include "shell.h"
#include "libft.h"

static char			*join_exe(char *s1, char *s2)
{
	if (DEBUG == 1)
		printf("------- JOIN EXE ------\n");

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
		printf("------- CHECK FCT------\n");

	char				**path;
	char				*tmp;
	t_duo				*env;
	int					i;
	char				**tbl_env;

	path = NULL;
	env = savior(NULL, FALSE);
	tbl_env = duo_to_tbl(env, "=");
	tmp = get_env("PATH");
	if (tmp == NULL)
		return (-1);
	path = ft_strsplit(tmp, ':');
	free(tmp);
	i = 0;
	while (path[i])
	{
		tmp = join_exe(path[i], cmd[0]);
		execve(tmp, cmd, tbl_env);
		free(tmp);
		i++;
	}
	return (0);
}
