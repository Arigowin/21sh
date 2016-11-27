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

t_line		*savior_stline(t_line *stline, int code)
{
	static t_line	*save = NULL;

	if ((save == NULL && stline) || code == TRUE)
		save = stline;
	return (save);
}

static int	init_env(char **env, t_duo **env_cpy)
{
	char			**cpy;

	cpy = NULL;
	if (tbl_len(env) == 0)
		fill_path(&cpy);
	if (cpy)
		*env_cpy = tbl_to_duo(cpy, '=');
	else
		*env_cpy = tbl_to_duo(env, '=');
	savior(*env_cpy, TRUE);
	free_tab(&cpy);
	return (0);
}

static int	init(t_line *stline)
{
	ioctl(0, TIOCGWINSZ, &(stline->win));

	if ((stline->line = ft_strnew(BUFF_SIZE)) == NULL)
		return (ERROR);
	stline->copy = NULL;
	stline->cpy_start = -1;
	savior_stline(stline, TRUE);
	return (TRUE);
}

int			main(void)
{
	extern char		**environ;
	t_duo			*env_cpy;
	t_line			stline;
	t_history		*history;

	init_env(environ, &env_cpy);
	if (init_term() == ERROR)
		return (ERROR);
	init(&stline);
	history = NULL;
	while (1)
	{
		reset_stline(&stline);
		check_signal(1);
		display_prompt();
		if (fct_read(&stline, &history) == -1)
			break ;
	}
	reset_term();
	free_duo(&env_cpy);
	free(stline.line);
	return (0);
}
