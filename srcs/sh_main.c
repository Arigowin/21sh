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

	init_env(environ, &env_cpy); //l27
	if (init_term() == ERROR) //sh_termcap
		return (ERROR);
	init_stline(&stline); //static , l23
	history = NULL;
	while (TRUE)
	{
		reset_stline(&stline);
		check_signal(1);
		display_prompt();
		if (fct_read(FALSE, &stline, &history) == ERROR)
			break ;
	}
	reset_term(); //sh_termcap
	free_duo(&env_cpy);
	free(stline.line);
	return (0);
}
