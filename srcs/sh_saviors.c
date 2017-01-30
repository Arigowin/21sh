#include "shell.h"
#include "libft.h"

char				*savior_tty(char *tty, int code)
{
	if (DEBUG_SAVIOR == 1)
		ft_putendl_fd("------- SAVIOR TTY -------", 2);

	static char 		*save_tty = NULL;

		if ((save_tty == NULL && tty) || code == TRUE)
			save_tty = tty;
		return (save_tty);
}

t_duo				*savior(t_duo *env, int code)
{
	if (DEBUG_SAVIOR == 1)
		ft_putendl_fd("------- SAVIOR -------", 2);

	static t_duo		*save = NULL;

	if ((save == NULL && env) || code == TRUE)
		save = env;
	return (save);
}

t_line				*savior_stline(t_line *stline, int code)
{
	if (DEBUG_SAVIOR == 1)
		ft_putendl_fd("------- SAVIOR STLINE -------", 2);

	static t_line		*save = NULL;

	if ((save == NULL && stline) || code == TRUE)
		save = stline;
	return (save);
}


t_node				*savior_node(t_node *node, int code)
{
	if (DEBUG_SAVIOR == 1)
		ft_putendl_fd("------- SAVIOR NODE -------", 2);

	static t_node		*save = NULL;

	if ((save == NULL && node) || code == TRUE)
		save = node;
	return (save);
}

t_node				*savior_tree(t_node *tree, int code)
{
	if (DEBUG_SAVIOR == 1)
		ft_putendl_fd("------- SAVIOR TREE -------", 2);

	static t_node		*save = NULL;

	if ((save == NULL && tree) || code == TRUE)
		save = tree;
	return (save);
}

t_history			*savior_history(t_history *env, int code)
{
	if (DEBUG_SAVIOR == 1)
		ft_putendl_fd("------- SAVIOR HISTORY -------", 2);

	static t_history		*save = NULL;

	if ((save == NULL && env) || code == TRUE)
		save = env;
	return (save);
}
