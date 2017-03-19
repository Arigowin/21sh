#include "shell.h"
#include "libft.h"

char				*savior_tty(char *tty, int code)
{
	static char			*save_tty = NULL;

	if ((save_tty == NULL && tty) || code == TRUE)
		save_tty = tty;
	return (save_tty);
}

t_duo				*savior_env(t_duo *env, int code)
{
	static t_duo		*save = NULL;

	if ((save == NULL && env) || code == TRUE)
		save = env;
	return (save);
}

t_line				*savior_stline(t_line *stline, int code)
{
	static t_line		*save = NULL;

	if ((save == NULL && stline) || code == TRUE)
		save = stline;
	return (save);
}

t_node				*savior_tree(t_node *tree, int code)
{
	static t_node		*save = NULL;

	if (save == NULL || code == TRUE)
		save = tree;
	return (save);
}

t_history			**savior_history(t_history **history, int code)
{
	static t_history		*save = NULL;

	if ((save == NULL && history && *history) || code == TRUE)
		save = *history;
	return (&save);
}
