#include <term.h>
#include "shell.h"
#include "libft.h"

// nom de fonction a modifier si necessaire
int					miniprt_reset_stline(t_line *stline)
{
	ft_putstr("> ");
	stline->curs_x = 2;
	stline->curs_y = 0;
	stline->mini_prt = TRUE;
	return (TRUE);
}

/*FICHIER A REPRENDRE -- RECLASSER LES FOCNTIONS PAR UTILITE */
int					mini_prt_handler(char **str, int *pos, t_line *stline)
{
	if (DEBUG_KEY == 1)
		ft_putendl_fd("------- MINI PRT HANDLER ------", 2);
	if ((*str)[0] != '\0')
		fct_insert(str, pos, '\n', stline);
	else if (!ft_strcmp(*str, ""))
		fct_insert(str, pos, '\n', stline);
	miniprt_reset_stline(stline);
	return (CONTINUE);
}

int					check_end_pipe(char **str, int *pos)
{
	if (DEBUG_KEY == 1)
		ft_putendl_fd("------- CHECK END PIPE ------", 2);

	int					i;
	int					j;

	i = *pos - 1;
	while (str && *str && (*str)[i] && i > 0)
	{
		if ((*str)[i] != ' ' && (*str)[i] != '\t'
				&& (*str)[i] != '\n' && (*str)[i] != '|')
			return (FALSE);
		if ((*str)[i] == '|' && (*str)[i - 1] == '|')
		{
			j = 1;
			i = ft_strlen(*str);
			while ((*str)[--i] == '|' && i > 0 && (*str)[i] == (*str)[i - 1])
				j++;
			if (j % 2 != 0)
				return (TRUE);
			return (FALSE);
		}
		if ((*str)[i] == '|' && (*str)[i - 1] != '\\')
			return (TRUE);
		i--;
	}
	return (FALSE);
}

int					check_nb_quote(char c, int back, int *quote, int to_fill)
{
	if (c && (c != '\\' || back % 2 == 0))
		*quote = (*quote != 0 ? 0 : to_fill);
	if (c == '\0')
		*quote = (*quote != 0 ? 0 : to_fill);
	return (TRUE);
}

int					quote_is_close(char **str)
{
	int					i;
	int					quote;
	int					back;

	i = -1;
	quote = 0;
	back = 0;
	while (str && *str && (*str)[++i])
	{
		if (quote != DQUOTE && (*str)[i] == QUOTE)
			check_nb_quote((*str)[i - 1], back, &quote, QUOTE);
		if (quote != QUOTE && (*str)[i] == DQUOTE)
			check_nb_quote((*str)[i - 1], back, &quote, DQUOTE);
		if ((*str)[i] == '\\')
			back++;
		else if (back > 0 && ((*str)[i] != '\\' || ((*str)[i] != QUOTE
		|| (*str)[i] != DQUOTE)))
			back = 0;
	}
	return (quote);
}

int					fct_return(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_KEY == 1)
		ft_putendl_fd("------- FCT RETURN ------", 2);

	fct_end(str, pos, stline, history);
	stline->quote = quote_is_close(str);
	if (stline->quote != 0 || ((*pos == 1 && (*str)[*pos - 1]
	&& (*str)[*pos - 1] == '\\') || (*pos > 1 && (*str)[*pos - 1] &&
	(*str)[*pos - 2] && (*str)[*pos - 1] == '\\' && (*str)[*pos - 2] != '\\'))
	|| stline->hrd.nb > 0 || (*pos > 0 && check_end_pipe(str, pos)))
	{
		if (stline->hrd.nb > 0 && (check_end_heredoc(stline) == BREAK))
			return (BREAK);
		ft_strdel(&(stline->curr_hist));
		if (*str && (*str)[0] && stline->quote != 0)
			modif_history(history, *str, FALSE);
		return (mini_prt_handler(str, pos, stline));
	}
	else
	{
		(*str)[*pos] = '\0';
		if (stline->copy.cpy != NULL && stline->copy.start != -1)
			fct_highlight(str, pos, stline, history);
		if (*str && (*str)[0])
			modif_history(history, *str, stline->mini_prt);
		ft_putchar('\n');
		return (BREAK);
	}
	return (FALSE);
}

int					fct_ctrl_d(char **str, int *pos, t_line *stline,
		t_history **history)
{
	if (DEBUG_KEY == 1)
		ft_putendl_fd("------- FCT CTRL D ------", 2);

	t_duo				*env;

	env = savior(NULL, FALSE);
	if (*str[0] == '\0' && stline->hrd.nb == 0)
		bi_exit(NULL, &env);
	else if (stline->hrd.nb != 0 && (*str[0] == '\0'
	|| (*str)[ft_strlen(*str) - 1] == '\n'))
	{
		stline->hrd.ctrl_d = TRUE;
		ft_putendl("");
		return (sh_error(BREAK, 31, stline->hrd.deli->data, NULL));
	}
	else
		fct_del(str, pos, stline, history);
	return (TRUE);
}

int					handle_quote(int key, char **str, int *pos, t_line *stline)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- HANDLE QUOTE ------", 2);

	if (*pos > 0 && (*str)[(*pos) - 1] && (*str)[(*pos) - 1] == '\\')
		return (FALSE);
	if (key == QUOTE || key == DQUOTE)
	{
		if (stline->quote == key)
			stline->quote = 0;
		else if (stline->quote == 0)
			stline->quote = key;
	}
	return (TRUE);
}

static t_key_fct	*tbl_key_fill(void)
{
	static t_key_fct	tbl_keys[18] =
	{
		{RETURN, fct_return}, {BACKSPACE, fct_backspace}, {DOWN, history_down},
		{HOME, fct_home}, {DEL, fct_del}, {CTRL_D, fct_ctrl_d}, {END, fct_end},
		{LEFT, fct_left}, {RIGHT, fct_right}, {UP, history_up},
		{CTRL_LEFT, fct_ctrl_left}, {CTRL_RIGHT, fct_ctrl_right},
		{CTRL_UP, fct_up}, {CTRL_DOWN, fct_down}, {CUT, fct_cut},
		{HIGHLIGHT, fct_highlight}, {PASTE, fct_paste}, {COPY, fct_copy}
	};

	return (tbl_keys);
}

int					insert_key(int key_to_insrt, t_line *stline)
{
	if (key_to_insrt != TAB && key_to_insrt > 31 && key_to_insrt < 128)
	{
		if (stline->hrd.nb <= 0)
			handle_quote(key_to_insrt, &(stline->line), &(stline->pos), stline);
		fct_insert((stline->hrd.nb > 0 ? &(stline->hrd.line) :
			&(stline->line)), (stline->hrd.nb > 0 ? &(stline->hrd.pos) :
				&(stline->pos)), key_to_insrt, stline);
	}
	return (TRUE);
}

int					event(int key_to_insrt, t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- EVENT ------", 2);

	int					i;
	int					ret;
	t_key_fct			*tbl_keys;

	tbl_keys = tbl_key_fill();
	i = -1;
	ret = 0;
	tputs(tgetstr("vi", NULL), 1, my_outc);
	while (++i < 18)
	{
		if (tbl_keys[i].key == key_to_insrt)
		{
			ret = (tbl_keys[i].fct((stline->hrd.nb > 0 ? &(stline->hrd.line)
					: &(stline->line)), (stline->hrd.nb > 0 ? &(stline->hrd.pos)
					: &(stline->pos)), stline, history));
			tputs(tgetstr("ve", NULL), 1, my_outc);
			return (ret);
		}
	}
	insert_key(key_to_insrt, stline);
	tputs(tgetstr("ve", NULL), 1, my_outc);
	return (TRUE);
}
