#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int					bi_exit(char **arg, t_duo **env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI EXIT ------", 2);

	t_node			*tree;
	t_history		*hist;
	t_line			*stline;
	int				i;

	i = 0;
	tree = savior_tree(NULL, FALSE);
	hist = *(savior_history(NULL, FALSE));
	stline = savior_stline(NULL, FALSE);
	if (arg && arg[1] && arg[2])
	{
		ft_putendl("21sh: exit : too many arguments");
		return (FALSE);
	}
	if (tree)
		del_tree(&tree);
	while (arg && arg[1] && arg[1][i])
	{
		if (ft_isdigit(arg[1][i]) == 0)
		{
			ft_putstr("21sh :exit : ");
			ft_putstr(arg[1]);
			ft_putendl(": numeric argument required");
			exit(255);
		}
		i++;
	}
	i = (arg && arg[1] ? ft_atoi(arg[1]) : 0);
	if (arg)
		free_tab(&arg);
	save_history();
	duo_del(env);
	reset_term();
	tree = NULL;
	del_history(&hist);
	ft_strdel(&(stline->hrd.line));
	ft_strdel(&(stline->line));
	ft_putendl("exit");
	exit(i);
	return (0);
}
