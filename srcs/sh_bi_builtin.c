#include "shell.h"
#include "libft.h"

int					is_builtin(char **cmd)
{
	static const char	*bi[] = {"echo", "cd", "setenv", "unsetenv",
								"env", "exit"};
	int					i;

	i = 0;
	if (cmd)
	{
		while (i < 6)
		{
			if (ft_strcmp(cmd[0], bi[i]) == 0)
				return (i);
			i++;
		}
	}
	return (-1);
}

int					handle_builtin(char **cmd)
{
	int					i;
	int					ret;
	t_duo				*env;
	static const char	*bi[] = {"echo", "cd", "setenv", "unsetenv", "env",
						"exit"};
	static int			(*tbl_bi[])(char **cmd, t_duo **env) = {&bi_echo,
						&bi_cd, &bi_setenv, &bi_unsetenv, &bi_env, &bi_exit};

	env = savior_env(NULL, FALSE);
	i = 0;
	while (i < 6 && ft_strcmp(cmd[0], bi[i]) != 0)
		i++;
	if (i < 6 && ft_strcmp(cmd[0], bi[i]) == 0)
	{
		if ((ret = tbl_bi[i](cmd, &env)) == ERROR)
			return (ERROR);
		return (ret);
	}
	if (cmd)
		free_tab(&cmd);
	return (FALSE);
}

int					check_builtin(int fd, char **cmd, int pipefd_tab[2][2],
					t_lst_fd **lstfd)
{
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
