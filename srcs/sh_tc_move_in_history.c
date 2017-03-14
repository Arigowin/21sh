#include "shell.h"
#include "libft.h"

int					dup_move_one(char **str)
{
	char				*tmp;

	if ((tmp = ft_strdup(*(str + 1))) == NULL)
		sh_error(FALSE, 6, NULL, NULL);
	ft_strdel(str);
	if ((*str = ft_strdup(tmp)) == NULL)
		sh_error(FALSE, 6, NULL, NULL);
	ft_strdel(&tmp);
	return (TRUE);
}

// nom a revoir
static int			history_up_prev(t_history **history, char *tmp, int *pos,
					t_line *stline) //static av history up
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("---------------- HISTORY UP PREV -----------------------", 2);

	char				*tmpchr;
	int					ret;
	int					len;

	if ((*history)->prev && tmp && *pos > 0)
	{
		len = ft_strlen((*history)->line);
		tmpchr = (stline->curs_y > 0 ? ft_strsub(tmp, ft_strlen(tmp) - len, len)
				: ft_strdup(ft_strrchr(tmp, '\n')));
		if (tmpchr != NULL && ft_strlen(tmpchr) > 1)
		{
			if (stline->curs_y == 0)
				dup_move_one(&tmpchr);
			ret = 0;
			if (ft_strcmp(tmpchr, (*history)->line) == 0)
				*history = (*history)->prev;
			*history = (*history)->prev;
		//	if (stline->mini_prt == FALSE // code a l'origine
		//	|| (ret = ft_strcmp(tmpchr, (*history)->line)) == 0)
		//	{
		//		*history = (*history)->prev;
		//	}
		}
		else
			if ((ret = ft_strcmp(tmp, (*history)->line)) == 0)
				*history = (*history)->prev;
		ft_strdel(&tmpchr);
	}
	return (TRUE);
}

static int			nb_line_total(char *str, t_line *stline)
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("---------------- NB LINE TOTAL -----------------------", 2);

	char				**line;
	int					i;
	int					len;
	int					nb;

	nb = ft_strncount(str, '\n');
	line = ft_strsplit(str, '\n');
	i = 0;
	while (line && line[i])
	{
		len = ft_strlen(line[i]);
		if (len > stline->win.ws_col)
		{
			nb += (len) / stline->win.ws_col;
			nb -= (len % stline->win.ws_col == 0 ? 1 : 0);
		}
		i++;
	}
	free_tab(&line);
	return (nb);
}

// nom a revoir
static int			reset_pos_x_y(char **str, int *pos, t_line *stline, //static ac les 2 suivantes
					t_history **history)
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("---------------- RESET POS X Y --------------------", 2);

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
	savior_history(history, TRUE);
	return (TRUE);
}

int					history_up(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_HISTORY == -1)
		ft_putendl_fd("---------------- HISTORY UP -----------------------", 2);

	int					i;
	char				*t;
	char				*tmpchr;

	//printf("\t\t\t\t\tline ((%s))\n\t\t\t\t\t curr hist ((%s)) \n\t\t\t\t\tnext ((%p))\n", (*history)->line, stline->curr_hist, (*history)->next);
	if ((history = savior_history(NULL, FALSE)) == NULL || *history == NULL)
		return (FALSE);
	fct_end(str, pos, stline, history);
	t = (*pos > 0 && (*str)[*pos - 1] == '\n' ? ft_strsub(*str, 0, *pos - 1)
			: ft_strdup(*str));
	if ((*history)->next == NULL && *pos > 0 && ft_strcmp(t, (*history)->line))
	{
		if ((tmpchr = ft_strrchr(t, '\n')) && ft_strlen(tmpchr) > 1)
			stline->curr_hist = ft_strdup(tmpchr + 1);
		else if (*str && *pos > 0 && (*str)[*pos - 1] != '\n')
			stline->curr_hist = ft_strdup(*str);
	}
	history_up_prev(history, t, pos, stline);
	while (*pos > 0	&& ((stline->curs_y == 0 && stline->curs_x > 2)
	|| stline->curs_y > 0))
		fct_backspace(str, pos, stline, history);
	i = -1;
	while (((*history)->line)[++i])
		fct_insert(str, pos, ((*history)->line)[i], stline);
	reset_pos_x_y(&((*history)->line), pos, stline, history);
	return (dblstr_duo_ret(TRUE, &t, NULL, NULL));
}

int					history_down(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_HISTORY == 1)
		ft_putendl_fd("---------------- HISTORY DOWN ---------------------", 2);

	int					i;

	if ((history = savior_history(NULL, FALSE)) == NULL || *history == NULL || ft_strcmp(stline->curr_hist, stline->line) == 0)
		return (FALSE);
	i = -2;
	if ((*history)->next)
	{
		*history = (*history)->next;
		i = -1;
	}
	fct_end(str, pos, stline, history);
	//printf("curr hist((%s)) line ((%s))\n", stline->curr_hist, stline->line);
	while (*pos > 0	&& ((stline->curs_y == 0 && stline->curs_x > 2)
	|| stline->curs_y > 0))
		fct_backspace(str, pos, stline, history);
	while (++i >= 0 && ((*history)->line)[i])
		fct_insert(str, pos, ((*history)->line)[i], stline);
	if (i == -1 && stline->curr_hist)
		while ((stline->curr_hist)[++i])
			fct_insert(str, pos, (stline->curr_hist)[i], stline);
	reset_pos_x_y(&((*history)->line), pos, stline, history);
	return (TRUE);
}
