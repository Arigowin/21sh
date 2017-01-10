#include "shell.h"
#include "libft.h"

char				*savior_tty(char *tty, int code)
{
	if (DEBUG_SAVIOR == 1)
		printf("------- SAVIOR TTY -------\n");

	static char 		*save = NULL;

	if ((save == NULL && tty) || code == TRUE)
		save = tty;
	return (save);
}

t_duo				*savior(t_duo *env, int code)
{
	if (DEBUG_SAVIOR == 1)
		printf("------- SAVIOR -------\n");

	static t_duo		*save = NULL;

	if ((save == NULL && env) || code == TRUE)
		save = env;
	return (save);
}

t_line				*savior_stline(t_line *stline, int code)
{
	if (DEBUG_SAVIOR == 1)
		printf("------- SAVIOR STLINE -------\n");

	static t_line	*save = NULL;

	if ((save == NULL && stline) || code == TRUE)
		save = stline;
	return (save);
}
