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

int					fct_up(t_line *stline, t_history **history)
{
	if (DEBUG_HISTORY == 1)
		printf("---------------- FCT UP --------------------------\n");
	int		i;

	if (*history == NULL)
		return (FALSE);
	i = 0;
	if ((*history)->prev)
	{
		if (ft_strcmp(stline->line, (*history)->line) == 0)
			*history = (*history)->prev;
		else if (ft_strstr(stline->line, (*history)->line) != NULL)
			*history = (*history)->prev;
	}
	fct_end(stline, history);
	while ((stline->pos_line > 0 && stline->quote != 0 && stline->curs_x > 2)
	 || (stline->pos_line > 0 && stline->quote == 0))
		fct_backspace(stline, history);
	while (((*history)->line)[i])
	{
		fct_insert(stline, ((*history)->line)[i]);
		i++;
	}
	return (0);
}

int				fct_down(t_line *stline, t_history **history)
{
	if (DEBUG_HISTORY == 1)
		printf("---------------- FCT DOWN --------------------------\n");
	int		i;

	if (*history == NULL)
		return (FALSE);
	i = -1;
	if ((*history)->next)
	{
		*history = (*history)->next;
		i = 0;
	}
	fct_end(stline, history);
	while ((stline->pos_line > 0 && stline->quote != 0 && stline->curs_x > 2)
	 || (stline->pos_line > 0 && stline->quote == 0))
		fct_backspace(stline, history);
	while (i >= 0 && ((*history)->line)[i])
	{
		fct_insert(stline, ((*history)->line)[i]);
		i++;
	}
	return (0);
}
