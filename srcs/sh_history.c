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
	printf("\nHISTORY : [%s]\n", (*history)->line);
}

//int					nav_history(int key, t_history **history, t_line *stline)
//{
//	if (DEBUG_HISTORY == 1)
//		printf("------- HISTORY ------\n");
//	int		i;
//
//	if (history && *history)
//	{
//		i = (key == DOWN ? -1 : 0);
//		if (key == DOWN && *history && (*history)->next)
//		{
//			*history = (*history)->next;
//			i = 0;
//		}
//		spec_key(END, stline);
//		while (stline->pos_line > 0)
//			backspace(stline);
//		while (i >= 0 && ((*history)->line)[i])
//		{
//			insert(stline, ((*history)->line)[i], ++(stline->pos_line) - 1);
//			i++;
//		}
//		if (key == UP && *history && (*history)->prev)
//			*history = (*history)->prev;
//	}
//	return (0);
//}

static int			up_history(t_line *stline, t_history **history)
{
	if (DEBUG_HISTORY == 1)
		printf("---------------- UP HISTORY --------------------------\n");
	int		i;

	i = 0;
	if ((*history)->prev)
	{
		if (ft_strcmp(stline->line, (*history)->line) == 0)
			*history = (*history)->prev;
		else if (ft_strstr(stline->line, (*history)->line) != NULL)
			*history = (*history)->prev;
	}
	spec_key(END, stline);
	while ((stline->pos_line > 0 && stline->quote != 0 && stline->curs_x > 2)
	 || (stline->pos_line > 0 && stline->quote == 0))
		backspace(stline);
	while (((*history)->line)[i])
	{
		insert(stline, ((*history)->line)[i], ++(stline->pos_line) - 1);
		i++;
	}
	return (0);
}

static int			down_history(t_line *stline, t_history **history)
{
	if (DEBUG_HISTORY == 1)
		printf("---------------- DOWN HISTORY --------------------------\n");
	int		i;

	i = -1;
	if ((*history)->next)
	{
		*history = (*history)->next;
		i = 0;
	}
	spec_key(END, stline);
	while ((stline->pos_line > 0 && stline->quote != 0 && stline->curs_x > 2)
	 || (stline->pos_line > 0 && stline->quote == 0))
		backspace(stline);
	while (i >= 0 && ((*history)->line)[i])
	{
		insert(stline, ((*history)->line)[i], ++(stline->pos_line) - 1);
		i++;
	}
	return (0);
}

int					nav_history(int key, t_history **history, t_line *stline)
{
	if (DEBUG_HISTORY == 1)
		printf("---------------- NAV HISTORY --------------------------\n");

	if (key == UP && *history != NULL)
		up_history(stline, history);
	else if (key == DOWN && *history != NULL)
		down_history(stline, history);
	return (0);
}
