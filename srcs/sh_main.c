#include "shell.h"
#include "libft.h"

int					main(void)
{
	extern char			**environ;
	t_duo	   	 		*env_cpy;
	t_line	   	 		stline;
	t_history  	 		*history;

	init_env(environ, &env_cpy);
	init_term();
	init_stline(&stline);
	history = NULL;
	while (TRUE)
	{
		if (stline.line)
			reset_stline(&stline);
		check_signal(1);
		display_prompt();
		if (fct_read(FALSE, &stline, &history) == ERROR)
			break ;
	}
	reset_term();
	free_duo(&env_cpy);
	free(stline.line);
	return (0);
}
