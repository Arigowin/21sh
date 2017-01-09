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
	init(&stline); //static , l23
	history = NULL;
	while (1)
	{
		reset_stline(&stline);
		check_signal(1);
		display_prompt();
		if (fct_read(&stline, &history) == -1)
			break ;
	}
	reset_term(); //sh_termcap
	free_duo(&env_cpy);
	free(stline.line);
	return (0);
}
