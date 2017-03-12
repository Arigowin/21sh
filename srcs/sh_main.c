#include <term.h>
#include "libft.h"
#include "shell.h"

static int			checktty_tool(char **tmp)
{
	char				buff[BUFF_SIZE + 1];
	int					ret;
	char				*tmp2;

	while ((ret = read(0, &buff, BUFF_SIZE)) > 0)
	{
		buff[ret] = '\0';
		if (*tmp != NULL)
		{
			if ((tmp2 = ft_strjoin(*tmp, buff)) == NULL)
				return (sh_error(FALSE, 6, NULL, NULL));
			ft_strdel(tmp);
			if ((*tmp = ft_strdup(tmp2)) == NULL)
			{
				ft_strdel(&tmp2);
				return (sh_error(FALSE, 6, NULL, NULL));
			}
			ft_strdel(&tmp2);
		}
		else if ((*tmp = ft_strdup(buff)) == NULL)
			return (sh_error(FALSE, 6, NULL, NULL));
	}
	if (ret == ERROR)
		return (ERROR);
	return (TRUE);
}

static int			checktty_tool2(t_line *stline, char **cmd)
{
	int					i;

	i = 0;
	while (cmd[i])
	{
		ft_strdel(&(stline->line));
		if ((stline->line = ft_strdup(cmd[i])) == NULL)
			return (sh_error(FALSE, 6, NULL, NULL));
		if (check_after_read(stline, NULL) == ERROR)
			return (ERROR);
		i++;
	}
	return (TRUE);
}

int					checktty(t_line *stline)
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
//		if (stline.line != NULL && stline.line[0] != '\0')
		reset_stline(&stline);
		check_signal(1);
		display_prompt();
		if (fct_read(FALSE, &stline, &history) == ERROR)
			break ;
	}
	reset_term();
	return (dblstr_duo_ret(0, &(stline.line), &(stline.hrd).line, &env_cpy));
}
