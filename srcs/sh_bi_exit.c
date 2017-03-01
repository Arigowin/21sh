#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int 				exit_pgm(int exit_code)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- EXIT PGM ------", 2);

	t_node			*tree;
	t_history		**hist;
	t_line			*stline;
	t_duo			*env;

	tree = savior_tree(NULL, FALSE);
	hist = savior_history(NULL, FALSE);
	stline = savior_stline(NULL, FALSE);
	env = savior(NULL, FALSE);
	del_tree(&tree);
	save_history();
	duo_del(&env);
	reset_term();
	tree = NULL;
	del_history(hist);
	ft_strdel(&(stline->hrd.line));
	ft_strdel(&(stline->line));
	if (exit_code == EXIT_SUCCESS)
		ft_putendl("exit");
	exit(exit_code);
	return (0);
}

int					bi_exit(char **arg, t_duo **env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI EXIT ------", 2);

	int				i;

	i = 0;
	(void)env;
	if (arg && arg[1] && arg[2])
		return (sh_error(FALSE, 27, NULL, NULL));
	while (arg && arg[1] && arg[1][i])
	{
		if (ft_isdigit(arg[1][i]) == 0)
		{
			return (sh_error(FALSE, 28, arg[1], NULL));
			free_tab(&arg);
			exit_pgm(255);
		}
		i++;
	}
	i = (arg && arg[1] ? ft_atoi(arg[1]) : 0);
	free_tab(&arg);
	exit_pgm(i);
	return (0);
}
