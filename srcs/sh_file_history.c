#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#include "get_next_line.h"

static int			in_quote(char **buff, char *line)
{
	char				*tmp1;
	char				*tmp2;

	tmp1 = NULL;
	tmp2 = NULL;
	if (*buff && line)
	{
		if ((tmp1 = ft_strjoin(line, "\n")) == NULL)
			return (ERROR);
		if ((tmp2 = ft_strjoin(*buff, tmp1)) == NULL)
			return (ERROR);
		ft_strdel(&tmp1);
		ft_strdel(buff);
		if ((*buff = ft_strdup(tmp2)) == NULL)
			return (ERROR);
		ft_strdel(&tmp2);
	}
	else if (line)
	{
		if ((*buff = ft_strjoin(line, "\n")) == NULL)
			return (ERROR);
	}
	return (TRUE);
}

static int			line_manager(char **buff, char *line, int *quote, t_history **history)
{
	char				*tmp;

	tmp = NULL;
	if (line == NULL || quote == NULL)
		return (ERROR);
	if (*quote != 0)
		return (in_quote(buff, line));
	else
	{
		if (*buff)
		{
			if ((tmp = ft_strjoin(*buff, line)) == NULL)
				return (ERROR);
			ft_strdel(buff);
			if ((*buff = ft_strdup(tmp)) == NULL)
				return (ERROR);
			ft_strdel(&tmp);
			add_history(history, *buff);
			ft_strdel(buff);
		}
		else
			add_history(history, line);
	}
	return (TRUE);
}

static int			get_line_in_file(int fd, t_history **history)
{
	int					quote;
	char				*buff;
	char				*line;

	buff = NULL;
	quote = 0;
	line = NULL;
	while (fd > -1 && get_next_line(fd, &line) > 0)
	{
		if (quote != DQUOTE && ft_strncount(line, QUOTE) % 2 != 0)
			quote = (quote == QUOTE ? 0 : QUOTE);
		else if (quote != QUOTE && ft_strncount(line, DQUOTE) % 2 != 0)
			quote = (quote == DQUOTE ? 0 : DQUOTE);
		if (line_manager(&buff, line, &quote, history) == ERROR)
			return (ERROR);
		ft_strdel(&line);
	}
	if (line)
	{
		if (line_manager(&buff, line, &quote, history) == ERROR)
			return (ERROR);
		ft_strdel(&line);
	}
	return (TRUE);
}

int					load_history(t_history **history)
{
	char				*home;
	char				*path;
	int					fd;

	path = NULL;
	fd = -1;
	home = get_env("HOME");
	if (home)
		path = ft_strjoin(home, HISTORY_FILE_NAME);
	if (path && (fd = open(path, O_RDONLY | O_CREAT,  S_IRUSR | S_IWUSR)) == ERROR)
		return (ERROR);
	ft_strdel(&path);
	ft_strdel(&home);
	if (get_line_in_file(fd, history) == ERROR)
		return (ERROR);
	if (fd > 2)
		close(fd);
	return (TRUE);
}

int					save_history(void)
{
	char				*home;
	char				*path;
	int					fd;
	t_history			*history;

	path = NULL;
	fd = -1;
	history = savior_history(NULL, FALSE);
	home = get_env("HOME");
	if (home)
		path = ft_strjoin(home, HISTORY_FILE_NAME);
	if (path && (fd = open(path,  O_WRONLY | O_CREAT,  S_IRUSR | S_IWUSR)) == ERROR)
		return (ERROR);
	ft_strdel(&path);
	ft_strdel(&home);
	while (history && history->prev)
		history = history->prev;
	while (history && history->line)
	{
		write(fd, history->line, ft_strlen(history->line));
		write(fd, "\n", 1);
		history = history->next;
	}
	if (fd > 2)
		close(fd);
	return (TRUE);
}
