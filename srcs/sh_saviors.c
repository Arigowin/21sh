#include "shell.h"
#include "libft.h"

char				*savior_tty(char *tty, int code)
{
	static char 		*save = NULL;

	if ((save == NULL && tty) || code == TRUE)
		save = tty;
	return (save);
}

t_duo				*savior(t_duo *env, int code)
{
	static t_duo		*save = NULL;

	if ((save == NULL && env) || code == TRUE)
		save = env;
	return (save);
}

t_line				*savior_stline(t_line *stline, int code)
{
	static t_line	*save = NULL; // ATTENTION STATIC !!

	if ((save == NULL && stline) || code == TRUE)
		save = stline;
	return (save);
}
