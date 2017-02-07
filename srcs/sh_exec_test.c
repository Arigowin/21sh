#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#include "get_next_line.h"

static char			*get_path(void)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- GET PATH ------", 2);

	char				*path;
	char				*tmp;
	char				*home;

	home = get_env("HOME");
	path = get_env("PWD");
	tmp = ft_strsub(path, 0, ft_strlen(home));
	if (home && ft_strcmp(home, tmp) == 0)
	{
		if (tmp)
			free(tmp);
		tmp = ft_strsub(path, ft_strlen(home), ft_strlen(path));
		if (path)
			free(path);
		path = ft_properjoin("~", tmp);
	}
	if (home)
		free(home);
	if (tmp)
		free(tmp);
	return (path);
}

static int			display_prompt_without_color(void)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- DISPLAY PROMPT ------", 2);

	char				*name;
	char				*path;

	path = get_path(); //free ok
	name = get_env("LOGNAME"); //free ok
	if (name)
		ft_putstr(name);
	if (path)
		ft_putstr(path);
	if (path || name)
		ft_putchar('\n');
	ft_putstr("> ");
	if (name)
		free(name);
	if (path)
		free(path);
	return (TRUE);
}

int					exec_test(int argc, char **argv, t_line *stline, t_history *history)
{
	int					fd;
	char				*line;
	int					i;
	t_duo				*save_env;
	t_line				*save_stline;

	line = NULL;
	save_env = savior(NULL, FALSE);
	save_stline = savior_stline(NULL, FALSE);
	if (argc != 2)
		return (FALSE);
	fd = open(argv[1], O_RDONLY);
	ft_putendl("------------- START TEST -------------");
	while (fd > -1 && get_next_line(fd, &line) > 0)
	{
		display_prompt_without_color();
		ft_putstr("[");
		reset_stline(stline);
		i = 0;
		while (line && line[i])
		{
			fct_insert(&(stline->line), &(stline->pos), line[i], stline);
			i++;
		}
		ft_putendl("]");
		check_after_read(stline, &history);
		ft_strdel(&line);
		ft_putendl("");
		ft_putendl("");
	}
	if (line)
	{
		display_prompt_without_color();
		ft_putstr("[");
		reset_stline(stline);
		i = 0;
		while (line && line[i])
		{
			fct_insert(&(stline->line), &(stline->pos), line[i], stline);
			i++;
		}
		ft_putendl("]");
		check_after_read(stline, &history);
		ft_strdel(&line);
		ft_putendl("");
	}
	ft_putendl("------------- END TEST -------------");
	ft_putendl("");
	close(fd);
	duo_del(&save_env);
	ft_strdel(&(stline->hrd.line));
	ft_strdel(&(stline->line));
	exit(EXIT_SUCCESS);
	return (TRUE);
}
