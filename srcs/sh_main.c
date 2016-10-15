#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

t_duo		*savior(t_duo *env, int code)
{
	static t_duo	*save = NULL;

	if ((save == NULL && env) || code == TRUE)
		save = env;
	return (save);
}

static int	init_env(char **env, t_duo **env_cpy)
{
	char			**cpy;

	cpy = NULL;
	if (!(env))
		fill_path(&cpy);
	if (cpy)
		*env_cpy = tbl_to_duo(cpy, '=');
	else
		*env_cpy = tbl_to_duo(env, '=');
	savior(*env_cpy, TRUE);
	free_tab(&cpy);
	return (0);
}

int			main(int ac, char **av, char **env)
{
	t_duo			*env_cpy;
	t_line			stline;
	t_history		*history;

	(void)ac;
	(void)av;
	init_env(env, &env_cpy);
	stline.line = ft_strnew(BUFF_SIZE);
	init_term();
	history = NULL;
//	printf("before while\n");
	while (1)
	{
//		printf("totopouet titi\n");
		ft_bzero(stline.line, BUFF_SIZE + 1);
		check_signal(1);
		display_prompt();
//		printf("fct read comming\n");
		fct_read(&stline, &history);
//		break ;
	}
	reset_term();
	free_duo(&env_cpy);
	free(stline.line);
	return (0);
}
