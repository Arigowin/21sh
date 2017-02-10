#include <curses.h>
#include <term.h>
#include "shell.h"
#include "libft.h"

static int				checktty_tool(char **tmp)
{
	char				buff[BUFF_SIZE + 1];
	int					ret;
	char				*tmp2;

	while ((ret = read(0, &buff, BUFF_SIZE)) > 0)
	{
		buff[ret] = '\0';
		if (*tmp == NULL)
			*tmp = ft_strdup(buff);
		else
		{
			tmp2 = ft_strjoin(*tmp, buff);
			ft_strdel(tmp);
			*tmp = ft_strdup(tmp2);
			ft_strdel(&tmp2);
		}
	}
	if (ret == ERROR)
		return (ERROR);
	return (TRUE);
}

static int				checktty_tool2(t_line *stline, char **cmd)
{
	int		i;

	i = 0;
	while (cmd[i])
	{
		if (i>0)
			ft_strdel(&(stline->line));
		stline->line = ft_strdup(cmd[i]);
		if (check_after_read(stline, NULL) == ERROR)
			return (ERROR);
		i++;
	}
	return (TRUE);
}

int						checktty(t_line *stline)
{
	if (DEBUG == 1)
		ft_putendl_fd("--------- CHECKTTY ------", 2);

	char				*tmp;
	char				**cmd;

	tmp = NULL;
	if (!isatty(0))
	{
		if (checktty_tool(&tmp) == ERROR)
			exit(EXIT_FAILURE);
		if (tmp)
		{
			if ((cmd = ft_strsplit(tmp, '\n')) == NULL)
				exit(EXIT_FAILURE);
			ft_strdel(&tmp);
			if (checktty_tool2(stline, cmd) == ERROR)
				exit(EXIT_FAILURE);
			free_tab(&cmd);
			exit(EXIT_SUCCESS);
		}
		ft_strdel(&tmp);
		exit(EXIT_FAILURE);
	}
	return (TRUE);
}

int					main(void)
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
