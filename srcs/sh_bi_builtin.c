#include <stdio.h>
#include "shell.h"
#include "libft.h"

int					is_builtin(char **cmd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- IS BUILTIN ------", 2);

	static const char	*bi[] = {"echo","cd", "setenv", "unsetenv", "env", "exit"};
	int					i;

	i = 0;
	if (cmd)
	while (i < 6)
	{
		if (ft_strcmp(cmd[0], bi[i]) == 0)
			return (i);
		i++;
	}
	/* -1 car code special pas une ERROR (cmd != bultin)*/
	return (-1);
}

int					handle_builtin(char **cmd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- HANDLE BUILTIN ------", 2);

	int					i;
	int					ret;
	t_duo				*env;
	static const char	*bi[] = {"echo", "cd", "setenv", "unsetenv", "env",
						"exit"};
	static int			(*tbl_bi[])(char **cmd, t_duo **env) = {&bi_echo,
						&bi_cd, &bi_setenv, &bi_unsetenv, &bi_env, &bi_exit};

	env = savior(NULL, FALSE);
	i = 0;
	while (i < 6 && ft_strcmp(cmd[0], bi[i]) != 0)
		i++;
	if (i < 6 && ft_strcmp(cmd[0], bi[i]) == 0)
	{
		if ((ret = tbl_bi[i](cmd, &env)) == ERROR)
			return (ERROR);
		else // pas necessaire ? puisqu ' on return avant
			return (ret);
	}
	if (cmd)
		free_tab(&cmd);
	return (FALSE);
}

int					check_builtin(int fd, char **cmd, int pipefd_tab[2][2],
					t_lst_fd **lstfd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- CHECK BUILTIN ------", 2);

	int					ret;

	if (fd == -1)
		return (FALSE);
	(void)*pipefd_tab;
	ret = -1;
	if (is_builtin(cmd) != -1)
	{
		if ((ret = handle_builtin(cmd)) == ERROR)
		{
			close_lstfd(lstfd);
			return (ERROR);
		}
		return (TRUE);
	}
	return (FALSE);
}
