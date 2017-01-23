#include "shell.h"
#include "libft.h"

static t_history	*new_history(char *line)
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("------- NEW HISTORY ------", 2);

	t_history			*new;

	if ((new = (t_history *)malloc(sizeof(t_history))) == NULL)
		/* RET: error EXIT: true msg: "malloc fail"
		* FREE : history */
		return (NULL);
	if (line == NULL)
		return (NULL);
	if ((new->line = ft_strdup(line)) == NULL)
		/* RET: error EXIT: true msg: "malloc fail"
		 * FREE : new history */
		return (NULL);
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
}

int					history_up(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("---------------- HISTORY UP -----------------------", 2);

	int					i;

	if (*history == NULL)
		return (FALSE);
	if ((*history)->next == NULL && ft_strcmp(*str, (*history)->line) != 0)
		stline->cur_hist = ft_strdup(*str);
	i = 0;
	if ((*history)->prev)
	{
		if (ft_strcmp(*str, (*history)->line) == 0 &&
			ft_strcmp(*str, stline->cur_hist) != 0)
			*history = (*history)->prev;
		else if (ft_strstr(*str, (*history)->line) != NULL &&
				 ft_strstr(*str, stline->cur_hist) == NULL)
			*history = (*history)->prev;
	}
	fct_end(str, pos, stline, history);
	while (left_move_cdt(*pos, stline))
		fct_backspace(str, pos, stline, history);
	while (((*history)->line)[i])
	{
		fct_insert(str, pos, ((*history)->line)[i], stline);
		i++;
	}
	return (TRUE);
}

int					history_down(char **str, int *pos, t_line *stline,
								 t_history **history)
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("---------------- HISTORY DOWN ---------------------", 2);

	int					i;

	if (*history == NULL)
		return (FALSE);
	i = -1;
	if ((*history)->next)
	{
		*history = (*history)->next;
		i = 0;
	}
	fct_end(str, pos, stline, history);
	while (left_move_cdt(*pos, stline))
		fct_backspace(str, pos, stline, history);
	while (i >= 0 && ((*history)->line)[i])
	{
		fct_insert(str, pos, ((*history)->line)[i], stline);
		i++;
	}
	if (i == -1 && stline->cur_hist)
	{
		i = 0;
		while ((stline->cur_hist)[i])
		{
			fct_insert(str, pos, (stline->cur_hist)[i], stline);
			i++;
		}
	}
	return (TRUE);
}
