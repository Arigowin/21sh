#include "shell.h"
#include "libft.h"

static t_history	*new_history(char *line)
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("------- NEW HISTORY ------", 2);

	t_history			*new;

	if (line == NULL)
		return (NULL);
	if ((new = (t_history *)malloc(sizeof(t_history))) == NULL)
		sh_error(TRUE, 6, NULL, NULL);
	if ((new->line = ft_strdup(line)) == NULL)
	{
		free(new);
		sh_error(TRUE, 6, NULL, NULL);
	}
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

void				add_history(t_history **history, char *line)
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("------- ADD HISTORY ------", 2);

	t_history			*new;

	while (*history && (*history)->next != NULL)
		(*history) = (*history)->next;
	if (*history == NULL)
		*history = new_history(line);
	else
	{
		new = new_history(line);
		(*history)->next = new;
		new->prev = *history;
		*history = new;
	}
	savior_history(history, TRUE);
}

void				del_history(t_history **history)
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("------- DEL HISTORY ------", 2);

	t_history *trash;
	t_history *save;

	trash = NULL;
	if (history == NULL || *history == NULL)
		return ;
	save = *history;
	while (save && save->prev)
		save = save->prev;
	while (save)
	{
		trash = save;
		save = (save)->next;
		ft_strdel(&(trash->line));
		free(trash);
		trash = NULL;
	}
	*history = NULL;
}

static int			remove_backslach_eol(char **line)
{
	char				*tmp;

	if (line && *line && (tmp = ft_strstr(*line, "\\\n")) != NULL)
	{
		str_delleft(tmp);
		str_delleft(tmp);
	}
	return (TRUE);
}

void				modif_history(t_history **history, char *line, int mini_prt)
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("------- ADD HISTORY ------", 2);

	if (line == NULL)
		return ;
	remove_backslach_eol(&line);
	if (*history == NULL || ((mini_prt == FALSE && ft_strcmp((*history)->line,
	line) != 0) || (*history)->next != NULL))
	{
		add_history(history, line);
		return ;
	}
	ft_strdel(&((*history)->line));
	if (((*history)->line = ft_strdup(line)) == NULL)
		sh_error(TRUE, 6, NULL, NULL);
}
