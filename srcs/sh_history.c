#include "shell.h"
#include "libft.h"

static t_history	*new_history(char *line)
{
	if (DEBUG_HISTORY == 1)
		printf("------- NEW HISTORY ------\n");
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

void				add_history(t_history **history, char *line)
{
	if (DEBUG_HISTORY == 1)
		printf("------- ADD HISTORY ------\n");
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

int					nav_history(int key, t_history **history, t_line *stline)
{
	if (DEBUG_HISTORY == 1)
		printf("------- HISTORY ------\n");
	int		i;

	if (*history)
	{
		i = (key == DOWN ? -1 : 0);
		if (key == DOWN && *history && (*history)->next)
		{
			*history = (*history)->next;
			i = 0;
		}
		while ((stline->curs_x) < (int)ft_strlen(stline->line))
			move(RIGHT, stline);
		while (stline->curs_x > 0)
			backspace(stline);
		while (i >= 0 && ((*history)->line)[i])
		{
			insert(stline, ((*history)->line)[i], ++(stline->curs_x) - 1);
			i++;
		}
		if (key == UP && *history && (*history)->prev)
			*history = (*history)->prev;
	}
	return (0);
}
