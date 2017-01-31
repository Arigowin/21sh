#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#include "get_next_line.h"

int					load_history(t_history **history)
{
//	t_duo				*env;
	char				*home;
	char				*path;
	char				*line;
	int					fd;

	path = NULL;
	line = NULL;
	fd = -1;
//	env = savior(NULL, FALSE);
	home = get_env("HOME");
	if (home)
		path = ft_strjoin(home, HISTORY_FILE_NAME);
	if (path && (fd = open(path, O_RDONLY | O_CREAT,  S_IRUSR | S_IWUSR)) == ERROR)
		return (ERROR);
	while (fd > -1 && get_next_line(fd, &line) > 0)
	{
		if (line)
			add_history(history, line);
	}
	if (line)
		add_history(history, line);
	return (TRUE);
}

int					save_history(void)
{
//	t_duo				*env;
	char				*home;
	char				*path;
	int					fd;
	t_history			*history;

	path = NULL;
	fd = -1;
	history = savior_history(NULL, FALSE);
//	env = savior(NULL, FALSE);
	home = get_env("HOME");
	if (home)
		path = ft_strjoin(home, HISTORY_FILE_NAME);
	if (path && (fd = open(path,  O_WRONLY | O_CREAT,  S_IRUSR | S_IWUSR)) == ERROR)
		return (ERROR);
	while (history && history->prev)
		history = history->prev;
	while (history && history->line)
	{
		write(fd, history->line, ft_strlen(history->line));
		write(fd, "\n", 1);
		history = history->next;
	}
	return (TRUE);
}
