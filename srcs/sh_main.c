#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

t_duo		*savior(t_duo *env)
{
	static t_duo	*save = NULL;

	if (save == NULL)
		save = env;
	return (save);
}

static int	init_env(char ***cpy, char **env, t_duo **env_cpy)
{
	*cpy = cpy_env(env);
	if (!(*env))
		fill_path(cpy);
	*env_cpy = tbl_to_duo(*cpy, '=');
	savior(*env_cpy);
	return (0);
}

int			main(int ac, char **av, char **env)
{
	char			**cpy;
	t_duo			*env_cpy;
	t_line			stline;
	t_history		*history;

	(void)ac;
	(void)av;
	init_env(&cpy, env, &env_cpy);
	stline.line = ft_strnew(BUFF_SIZE);
	init_term();
	history = NULL;
	printf("before while\n");
	while (1)
	{
		printf("totopouet titi\n");
		ft_bzero(stline.line, BUFF_SIZE + 1);
		check_signal(1);
		display_prompt(env_cpy);
		printf("fct read comming\n");
		fct_read(&stline, &env_cpy, &history);
	}
	reset_term();
	free_duo(&env_cpy);
	free_tab(&cpy);
	free(stline.line);
	return (0);
}
