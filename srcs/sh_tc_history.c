#include "shell.h"
#include "libft.h"

static t_history	*new_history(char *line)
{
	t_history			*new;

	if (line == NULL)
		return (NULL);
	if ((new = (t_history *)malloc(sizeof(t_history))) == NULL)
		sh_error(FALSE, 6, NULL, NULL);
	if ((new->line = ft_strdup(line)) == NULL)
	{
		ft_strdel(&line);
		free(new);
		new = NULL;
		sh_error(FALSE, 6, NULL, NULL);
	}
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

void				add_history(t_history **history, char *line)
{
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
		new = NULL;
	}
	savior_history(history, TRUE);
}

void				del_history(t_history **history)
{
	t_history *save;

	if (history == NULL || *history == NULL)
		return ;
	while (*history && (*history)->prev)
		*history = (*history)->prev;
	save = *history;
	while (history && *history)
	{
		save = *history;
		*history = (*history)->next;
		ft_strdel(&(save->line));
		save->prev = NULL;
		save->next = NULL;
		free(save);
		save = NULL;
	}
	free(history);
	history = NULL;
}

static int			remove_backslash_eol(char **line)
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
	if (line == NULL)
		return ;
	remove_backslash_eol(&line);
	if (*history == NULL
	|| ((mini_prt == FALSE && ft_strcmp((*history)->line, line) != 0))
	|| (*history)->next != NULL)
	{
		add_history(history, line);
		return ;
	}
	ft_strdel(&((*history)->line));
	if (((*history)->line = ft_strdup(line)) == NULL)
		sh_error(FALSE, 6, NULL, NULL);
}
