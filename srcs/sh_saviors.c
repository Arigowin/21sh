#include "shell.h"
#include "libft.h"

char				*savior_tty(char *tty, int code)
{
	if (DEBUG_SAVIOR == 1)
		ft_putendl_fd("------- SAVIOR TTY -------", 2);

	static char 		*save = NULL;

	if ((save == NULL && tty) || code == TRUE)
		save = tty;
	return (save);
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
