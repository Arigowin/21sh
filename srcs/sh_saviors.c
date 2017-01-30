#include "shell.h"
#include "libft.h"

char				*savior_tty(char *tty, int code, int in)
{
	if (DEBUG_SAVIOR == 1)
		ft_putendl_fd("------- SAVIOR TTY -------", 2);

	static char 		*save_in = NULL;
	static char 		*save_out = NULL;

	if (in == TRUE)
	{
		if ((save_in == NULL && tty) || code == TRUE)
			save_in = tty;
		return (save_in);
	}
	else if (in == FALSE)
	{
		if ((save_out == NULL && tty) || code == TRUE)
			save_out = tty;
		return (save_out);
	}
	return (NULL);
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

t_node				*savior_tree(t_node *tree, int code)
{
	if (DEBUG_SAVIOR == 1)
		ft_putendl_fd("------- SAVIOR TREE -------", 2);

	static t_node		*save = NULL;

	if (save == NULL || code == TRUE)
		save = tree;
	return (save);
}
