#include "libft.h"
#include "shell.h"

int					main(void)
{
	if (DEBUG == 1)
		ft_putendl_fd("--------- MAIN ------", 2);

	extern char			**environ;
	t_duo				*env_cpy;
	t_line				stline;
	t_history			*history;

	history = NULL;
	if (init_env(environ, &env_cpy) == ERROR)
		return (ERROR);
	init_stline(&stline);
	checktty(&stline);
	load_history(&history);
	init_term(TRUE);
	while (TRUE)
	{
		reset_stline(&stline);
		check_signal(1);
		display_prompt();
		if (fct_read(FALSE, &stline, &history) == ERROR)
			break ;
	}
	reset_term();
}
