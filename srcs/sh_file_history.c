#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#include "get_next_line.h"

static int			in_quote(char **buff, char *line)
{
	if (DEBUG_FILE_HIST == 1)
		ft_putendl_fd("------- IN QUOTE ------", 2);

	char				*tmp1;
	char				*tmp2;

	tmp1 = NULL;
	tmp2 = NULL;
	if (*buff && line)
	{
		if ((tmp1 = ft_strjoin(line, "\n")) == NULL)
			return (ERROR);
		if ((tmp2 = ft_strjoin(*buff, tmp1)) == NULL)
			return (dblstr_duo_ret(ERROR, &tmp1, NULL, NULL));
		dblstr_duo_ret(TRUE, &tmp1, buff, NULL);
		if ((*buff = ft_strdup(tmp2)) == NULL)
			return (dblstr_duo_ret(ERROR, &tmp2, NULL, NULL));
		ft_strdel(&tmp2);
	}
	else if (line && (*buff = ft_strjoin(line, "\n")) == NULL)
		return (ERROR);
	return (TRUE);
}

static int			line_manager(char **buff, char *line, int *quote,
					t_history **history)
{
	if (DEBUG_FILE_HIST == 1)
		ft_putendl_fd("------- LINE MANAGER ------", 2);

	char				*tmp;

	tmp = NULL;
	if (line[0] == 3 || ft_strlen(line) <= 0)
		return (FALSE);
	if (*quote != 0)
		return (in_quote(buff, line));
	else
	{
		if (*buff)
		{
			if ((tmp = ft_strjoin(*buff, line)) == NULL)
				return (sh_error(FALSE, 6, NULL, NULL));
			ft_strdel(buff);
			if ((*buff = ft_strdup(tmp)) == NULL)
				return (sh_error(FALSE, 6, NULL, NULL));
			add_history(history, *buff);
			dblstr_duo_ret(TRUE, &tmp, buff, NULL);
		}
		else
			add_history(history, line);
	}
	return (TRUE);
}

static int			get_line_in_file(int fd, t_history **history)
{
	if (DEBUG_FILE_HIST == 1)
		ft_putendl_fd("------- GET LINE IN FILE ------", 2);

	int					quote;
	char				*buff;
	char				*line;

	buff = NULL;
	quote = 0;
	line = NULL;
	while (fd > -1 && get_next_line(fd, &line) > 0)
	{
		quote = quote_is_close(&line) - quote_is_close(&buff);
		if (line == NULL || line_manager(&buff, line, &quote, history) == ERROR)
			return (dblstr_duo_ret(ERROR, &line, &buff, NULL));
		ft_strdel(&line);
	}
	if (line)
	{
		quote = quote_is_close(&line) - quote_is_close(&buff);
		if (line_manager(&buff, line, &quote, history) == ERROR)
			return (dblstr_duo_ret(ERROR, &line, &buff, NULL));
		ft_strdel(&line);
	}
	return (dblstr_duo_ret(TRUE, &buff, NULL, NULL));
}

int					load_history(t_history **history)
{
	if (DEBUG_FILE_HIST == 1)
		ft_putendl_fd("------- LOAD HISTORY ------", 2);

	char				*home;
	char				*path;
	int					fd;

	path = NULL;
	fd = -1;
	home = get_env("HOME");
	if (home)
		path = ft_strjoin(home, HISTORY_FILE_NAME);
	if (path && (fd = open(path, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR))
	== ERROR)
		return (dblstr_duo_ret(ERROR, &path, &home, NULL));
	dblstr_duo_ret(TRUE, &path, &home, NULL);
	if (get_line_in_file(fd, history) == ERROR)
		return (ERROR);
	if (fd > 2)
		close(fd);
	return (TRUE);
}

int					save_history(void)
{
	if (DEBUG_FILE_HIST == 1)
		ft_putendl_fd("------- SAVE HISTORY ------", 2);

	char				*home;
	char				*path;
	int					fd;
	t_history			*history;

	path = NULL;
	if ((history = *(savior_history(NULL, FALSE))) == NULL)
		return (FALSE);
	home = get_env("HOME");
	path = (home != NULL ? ft_strjoin(home, HISTORY_FILE_NAME) : NULL);
	if ((fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR))
	== ERROR && path)
		return (dblstr_hist_ret(ERROR, &path, &home, &history));
	while (history && history->prev)
		history = history->prev;
	while (history && history->line)
	{
		ft_putendl_fd(history->line, fd);
		history = history->next;
	}
	ft_putchar_fd(3, fd);
	if (fd > 2)
		close(fd);
	return (dblstr_hist_ret(TRUE, &path, &home, &history));
}
