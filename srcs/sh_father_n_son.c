#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

static char		*join_exe(char *s1, char *s2)
{
	if (DEBUG == 1)
		printf("------- JOIN EXE ------\n");
	char		*rlt;
	char		*tmp;

	if (s2[0] == '/' || (s2[0] == '.' && s2[1] == '/'))
		return (s2);
	tmp = ft_strjoin("/", s2);
	rlt = ft_strjoin(s1, tmp);
	ft_strdel(&tmp);
	return (rlt);
}

static int		check_fct(char **cmd, char **env, t_duo **env_cpy)
{
	if (DEBUG == 1)
		printf("------- CHECK FCT------\n");
	char		**path;
	char		*tmp;
	int			i;

	tmp = get_env(env_cpy, "PATH");
	if (tmp == NULL)
		fill_path(&path);
	else
		path = read_n_check(":", tmp);
	i = 0;
	while (path[i])
	{
		tmp = join_exe(path[i], cmd[0]);
		// a execve tu doit lui passer la copie des variable d'env pas les variable d'en de base
		execve(tmp, cmd, env);
		i++;
	}
	free(tmp);
	return (0);
}

int				father_n_son(char **cmd, t_duo **env_cpy)
{
	if (DEBUG == 1)
		printf("------- FATHER N SON ------\n");
	pid_t		father;
	int			stat_loc;

	father = fork();
	if (father > 0)
	{
		check_signal(3);
		wait(&stat_loc);
	}
	if (father == 0)
	{
		check_signal(2);
		// transformer env_cpy en char**
		check_fct(cmd, NULL, env_cpy);
		ft_putstr("21sh: ");
		ft_putstr(cmd[0]);
		ft_putendl(": command not found");
		exit(EXIT_FAILURE);
	}
	return (0);
}
