#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"


int					main(void)
{
	extern char			**environ;
	t_duo	   	 		*env_cpy;
	t_line	   	 		stline;
	t_history  	 		*history;

	if (init_term() == ERROR || init_env(environ, &env_cpy))
		return (ERROR);
	init_stline(&stline);
	history = NULL;
	while (TRUE)
	{
		if (stline.line)
			reset_stline(&stline);
		check_signal(1);
		if (!display_prompt())
			/* MSG ret: ERROR exit: TRUE msg: "Could not display prompt." */
		if (fct_read(FALSE, &stline, &history) == ERROR)
			break ;
	}
	reset_term(); //sh_termcap
	free_duo(&env_cpy);
	free(stline.line);
	return (0);
}
