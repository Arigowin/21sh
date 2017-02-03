#include "shell.h"
#include "libft.h"

void				del_history(t_history **histo)
{
	t_history *trash;
	t_history *save;

	trash = NULL;
	save = *histo;
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
	*histo = NULL;
}

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
	{
		*history = new_history(line);
	}
	else
	{
		new = new_history(line);
		(*history)->next = new;
		new->prev = *history;
		*history = new;
	}
	savior_history(history, TRUE);
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
	if (*history == NULL || ((mini_prt == FALSE && ft_strncmp((*history)->line,
															  line, ft_strlen((*history)->line)) != 0) || (*history)->next != NULL))
	{
		add_history(history, line);
		return ;
	}
	ft_strdel(&((*history)->line));
	if (((*history)->line = ft_strdup(line)) == NULL)
		/* RET =  EXIT = true msg = "malloc fail" */
		return ;
}

// nom a revoir
int					history_up_prev(t_history **history, char *tmp, int *pos, t_line *stline)
{
	char				*tmpchr;
	int					ret;

	if ((*history)->prev && tmp && *pos > 0)
	{
		tmpchr = ft_strrchr(tmp, '\n');
		if (tmpchr != NULL && ft_strlen(tmpchr) > 1)
		{
			(tmpchr)++;
			if (stline->mini_prt == FALSE || (ret = ft_strcmp(tmpchr, (*history)->line)) == 0)
				*history = (*history)->prev;
		}
		else
		{
			if ((ret = ft_strcmp(tmp, (*history)->line)) == 0)
				*history = (*history)->prev;
		}
	}
	return (TRUE);
}

static int			nb_line_total(char *str, t_line *stline)
{
	char				**line;
	int					i;
	int					nb_line;
	int					len;
	int					nb;

	nb = ft_strncount(str, '\n');
	line = ft_strsplit(str, '\n');
	i = 0;
	nb_line = 0;
	while (line && line[i])
	{
		nb_line++;
		len = ft_strlen(line[i]);
		if (len > stline->win.ws_col)
		{
			nb_line += ((len) / stline->win.ws_col) - 1;
			nb++;
		}
		i++;
	}
	free_tab(&line);
	if (i - 1 < nb)
		nb_line += (nb - i) + 1;
	return (nb_line - 1);
}

// nom a revoir
int					reset_pos_x_y(char **str, int *pos, t_line *stline)
{
	int					nb;
	int					len;
	int					nb_line;

	nb = nb_line_total(*str, stline);
	if (nb > 0)
		stline->curs_y = nb;
	if (stline->curs_y > 0 && *str != NULL && *pos > 0)
	{
		len = ft_strlen(ft_strrchr(*str, '\n')) - 1;
		if (len > stline->win.ws_col)
		{
			nb_line = (len) / stline->win.ws_col;
			len = (len - (stline->win.ws_col * nb_line));
		}
		stline->curs_x = len;
	}
	return (TRUE);
}

int					history_up(char **str, int *pos, t_line *stline,
							   t_history **history)
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("---------------- HISTORY UP -----------------------", 2);

	int					i;
	char				*tmp;
	char				*tmpchr;

	if (*history == NULL)
		return (FALSE);
	fct_end(str, pos, stline, history);
	tmp = NULL;
	tmp = (*pos > 0 && (*str)[*pos - 1] == '\n' ? ft_strsub(*str, 0, *pos - 1) : ft_strdup(*str));
	if ((*history)->next == NULL && *pos > 0 && ft_strcmp(tmp, (*history)->line) != 0)
	{
		tmpchr = ft_strrchr(tmp, '\n');
		if (tmpchr && ft_strlen(tmpchr) > 1)
			stline->curr_hist = ft_strdup(tmpchr + 1);
		else if (*str && *pos > 0 && (*str)[*pos - 1] != '\n')
			stline->curr_hist = ft_strdup(*str);
	}
	history_up_prev(history, tmp, pos, stline);
	ft_strdel(&tmp);
	while (left_move_cdt(*pos, stline))
		fct_backspace(str, pos, stline, history);
	i = -1;
	while (((*history)->line)[++i])
		fct_insert(str, pos, ((*history)->line)[i], stline);
	reset_pos_x_y(str, pos, stline);
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
	if (i == -1 && stline->curr_hist)
	{
		i = 0;
		while ((stline->curr_hist)[i])
		{
			fct_insert(str, pos, (stline->curr_hist)[i], stline);
			i++;
		}
	}
	return (TRUE);
}
