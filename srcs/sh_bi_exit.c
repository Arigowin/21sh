#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int					bi_exit(char **arg, t_duo **env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI EXIT ------", 2);

	int					i;

	i = 0;
	t_node *tree;
	tree = NULL;
	tree = savior_tree(NULL, FALSE);
	//printf("bi_exit : valeur tree_savd : %p\n", tree);
	if (tree)
		del_tree(&tree);
	if (arg && arg[1] && arg[2])
	{
		ft_putendl("21sh: exit : too many arguments");
		return (ERROR);
	}
	while (arg && arg[1] && arg[1][i])
	{
		if (ft_isdigit(arg[1][i]) == 0)
		{
			ft_putstr("21sh :exit : ");
			ft_putstr(arg[1]);
			ft_putendl(": numeric argument required");
			return (ERROR);
		}
		i++;
	}
	i = (arg && arg[1] ? ft_atoi(arg[1]) : 0);
	ft_putendl("exit");
	free_tab(&arg);
	save_history();
	duo_del(env);
	reset_term();
	tree = NULL;
	if (tree)
		ft_putendl("ARBRE EXISTE ENCORE");
	else
		ft_putendl("ARBRE COUPE");
	exit(i);
	return (0);
}
