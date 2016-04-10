#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft.h"

t_duo		*savior(t_duo *env)
{
	static t_duo	*save = NULL;

	if (save == NULL)
		save = env;
	return (save);
}

int			main(int ac, char **av, char **env)
{
	char			*read_buff;
	char			**cpy;
	t_duo			*env_cpy;

	(void)ac;
	(void)av;
	cpy = cpy_env(env);
	if (!(*env))
		fill_path(&cpy);
	env_cpy = tbl_to_duo(cpy, '=');
	savior(env_cpy);
	read_buff = ft_strnew(BUFF_SIZE);
	while (1)
	{
		ft_bzero(read_buff, BUFF_SIZE + 1);
		check_signal(1);
		display_prompt(env_cpy);
		fct_read(read_buff, cpy, &env_cpy);
	}
	free_duo(&env_cpy);
	free_tab(&cpy);
	free(read_buff);
	return (0);
}
