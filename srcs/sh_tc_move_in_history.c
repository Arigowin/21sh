#include "shell.h"
#include "libft.h"

// nom a revoir
static int			history_up_prev(t_history **history, char *tmp, int *pos,
					t_line *stline) //static av history up
{
	char				*tmpchr;
	int					ret;
	int					len;

	if ((*history)->prev && tmp && *pos > 0)
	{
		if (stline->curs_y > 0)
		{
			len = ft_strlen((*history)->line);
			tmpchr = ft_strsub(tmp, ft_strlen(tmp) - len, len);
		}
		else
			tmpchr = ft_strrchr(tmp, '\n');
		if (tmpchr != NULL && ft_strlen(tmpchr) > 1)
		{
			if (stline->curs_y == 0)
				(tmpchr)++;
			ret = 0;
			if (stline->mini_prt == FALSE
			|| (ret = ft_strcmp(tmpchr, (*history)->line)) == 0)
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
static int			reset_pos_x_y(char **str, int *pos, t_line *stline) //static ac les 2 suivantes
{
	int					nb;
	int					len;
	int					nb_line;
	char				*tmp;

	tmp = NULL;
	nb = nb_line_total(*str, stline);
	if (nb > 0)
		stline->curs_y = nb;
	if (stline->curs_y > 0 && *str != NULL && *pos > 0)
	{
		tmp = ft_strrchr(*str, '\n');
		len = (tmp != NULL ? ft_strlen(tmp) - 1 : ft_strlen(*str));
		if (len > stline->win.ws_col)
		{
			len += PRT_LEN;
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
	char				*t;
	char				*tmpchr;

	if ((history = savior_history(NULL, FALSE)) == NULL)
		return (FALSE);
	fct_end(str, pos, stline, history);
	t = (*pos > 0 && (*str)[*pos - 1] == '\n' ? ft_strsub(*str, 0, *pos - 1)
			: ft_strdup(*str));
	if ((*history)->next == NULL && *pos > 0 && ft_strcmp(t, (*history)->line))
	{
		tmpchr = ft_strrchr(t, '\n');
		if (tmpchr && ft_strlen(tmpchr) > 1)
			stline->curr_hist = ft_strdup(tmpchr + 1);
		else if (*str && *pos > 0 && (*str)[*pos - 1] != '\n')
			stline->curr_hist = ft_strdup(*str);
	}
	history_up_prev(history, t, pos, stline);
	while (left_move_cdt(*pos, stline))
		fct_backspace(str, pos, stline, history);
	i = -1;
	while (((*history)->line)[++i])
		fct_insert(str, pos, ((*history)->line)[i], stline);
	reset_pos_x_y(&((*history)->line), pos, stline);
	savior_history(history, TRUE);
	return (dblstr_duo_ret(TRUE, &t, NULL, NULL));
}

int					history_down(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("---------------- HISTORY DOWN ---------------------", 2);

	int					i;

	if ((history = savior_history(NULL, FALSE)) == NULL)
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
		i = -1;
		while ((stline->curr_hist)[++i])
			fct_insert(str, pos, (stline->curr_hist)[i], stline);
	}
	savior_history(history, TRUE);
	return (TRUE);
}
