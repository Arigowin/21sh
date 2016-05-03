#include "shell.h"
#include "libft.h"

static t_history	*new_history(char *line)
{
	t_history	*new;

	if ((new = (t_history *)malloc(sizeof(t_history))) == NULL)
		return (NULL);
	if (line == NULL)
		return (NULL);
	if ((new->line = ft_strdup(line)) == NULL)
		return (NULL);
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

void			add_history(t_history **history, char *line)
{
	t_history	*new;

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
}

char		*nav_history(int key, t_history **history, t_line *stline)
{
	int		i;

	i = 0;
	if (key == UP && *history != NULL)
	{
		while (stline->curs_x > 3)
			backspace(stline);
		while (((*history)->line)[i])
		{
			insert(stline, ((*history)->line)[i], ++(stline->curs_x) - 4);
			i++;
		}
		if ((*history)->prev)
			*history = (*history)->prev;
	}
	else if (key == DOWN && *history != NULL)
	{
		i = -1;
		if ((*history)->next)
		{
			*history = (*history)->next;
			i = 0;
		}
		while (stline->curs_x > 3)
			backspace(stline);
		while (i >= 0 && ((*history)->line)[i])
		{
			insert(stline, ((*history)->line)[i], ++(stline->curs_x) - 4);
			i++;
		}
	}
	return (NULL);
}
