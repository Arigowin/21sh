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

//static int			up_history(t_line *stline, t_history **history)
//{
//	if (DEBUG_HISTORY == 1)
//		printf("------- UP HISTORY ------\n");
//	int		i;
//
//	i = 0;
//	while (stline->curs_x > 3)
//		backspace(stline);
//	while (((*history)->line)[i])
//	{
//		insert(stline, ((*history)->line)[i], ++(stline->curs_x) - 4);
//		i++;
//	}
//	if ((*history)->prev)
//		*history = (*history)->prev;
//	return (0);
//}
//


static int			nav2_history(t_line *stline, t_history **history, int up)
{
	if (DEBUG_HISTORY == 1)
		printf("------- HISTORY ------\n");
	int		i;

	i = -1;
	if (up == 0 && (*history)->next)
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
	if (up == 1 && (*history)->prev)
		*history = (*history)->prev;
	return (0);
}

int					nav_history(int key, t_history **history, t_line *stline)
{
	if (DEBUG_HISTORY == 1)
		printf("------- NAV HISTORY ------\n");
	if (key == UP && *history != NULL)
		nav2_history(stline, history, 1);
	else if (key == DOWN && *history != NULL)
		nav2_history(stline, history, 0);
	return (0);
}

/**************************************** autre possibilité ???
 
int					nav_history(int key, t_history **history, t_line *stline)
{
	if (DEBUG_HISTORY == 1)
		printf("------- HISTORY ------\n");
	int		i;

	i = -1;
	if (key == UP && *history && (*history)->next)
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
	if (key == DOWN && *history && (*history)->prev)
		*history = (*history)->prev;
	return (0);
}

*********************************************************/
