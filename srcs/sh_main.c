#include "shell.h"
#include "libft.h"
#include <curses.h>
#include <term.h>

int					checktty(t_line *stline)
{
	if (DEBUG == 1)
		ft_putendl_fd("--------- CHECKTTY ------", 2);

	char				buff[BUFF_SIZE + 1];
	int					ret;
	char				*tmp;
	char				**cmd;
	int					i;

	ret = 0;
	tmp = NULL;
	if (!isatty(0))
	{
		while ((ret = read(0, &buff, BUFF_SIZE)) > 0)
		{
			buff[ret] = '\0';
			if (tmp == NULL)
				tmp = ft_strdup(buff);
			else
				tmp = ft_strjoin(tmp, buff);
		}
		if (tmp && ret >= 0)
		{
			cmd = ft_strsplit(tmp, '\n');
			i = 0;
			while (cmd[i])
			{
				stline->line = ft_strdup(cmd[i]);
				check_after_read(stline, NULL);
				i++;
			}
			exit(EXIT_SUCCESS);
		}
		exit(EXIT_FAILURE);
	}
	return (TRUE);
}

int					main(int argc, char **argv)
{
	if (DEBUG == 1)
		ft_putendl_fd("--------- MAIN ------", 2);

	extern char			**environ;
	t_duo	   	 		*env_cpy;
	t_line	   	 		stline;
	t_history  	 		*history;

	// return useless
	history = NULL;
	if (init_env(environ, &env_cpy) == ERROR)
		return (ERROR);
	init_stline(&stline);
	exec_test(argc, argv, &stline, history);
	load_history(&history);
	checktty(&stline);
	init_term();
	while (TRUE)
	{
		if (stline.line == NULL && stline.line[0] != '\0')
			reset_stline(&stline);
		check_signal(1);
		display_prompt();
		if (fct_read(FALSE, &stline, &history) == ERROR)
			break ;
	}
	reset_term();
	duo_del(&env_cpy);
	ft_strdel(&(stline.line));
	ft_strdel(&(stline.hrd).line);
	return (0);
}
