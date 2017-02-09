#include <curses.h>
#include <term.h>
#include "shell.h"
#include "libft.h"

int					checktty(t_line *stline)
{
	if (DEBUG == 1)
		ft_putendl_fd("--------- CHECKTTY ------", 2);

	char				buff[BUFF_SIZE + 1];
	int					ret;
	char				*tmp;
	char				*tmp2;
	char				**cmd;
	int					i;

	ret = 0;
	tmp = NULL;
	tmp2 = NULL;
	if (!isatty(0))
	{
		while ((ret = read(0, &buff, BUFF_SIZE)) > 0)
		{
			buff[ret] = '\0';
			if (tmp == NULL)
				tmp = ft_strdup(buff);
			else
			{
				tmp2 = ft_strjoin(tmp, buff);
				ft_strdel(&tmp);
				tmp = ft_strdup(tmp2);
				ft_strdel(&tmp2);
			}
		}
		ft_strdel(&tmp2);
		if (tmp && ret >= 0)
		{
			cmd = ft_strsplit(tmp, '\n');
			ft_strdel(&tmp);
			i = 0;
			while (cmd[i])
			{
				if (i>0)
					ft_strdel(&(stline->line));
				stline->line = ft_strdup(cmd[i]);
				check_after_read(stline, NULL);
				i++;
			}
			free_tab(&cmd);
			exit(EXIT_SUCCESS);
		}
		ft_strdel(&tmp);
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
	checktty(&stline);
	load_history(&history);
	init_term(TRUE);
	while (TRUE)
	{
		if (stline.line != NULL && stline.line[0] != '\0')
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
