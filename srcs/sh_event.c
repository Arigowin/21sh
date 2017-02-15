#include <term.h>
#include "shell.h"
#include "libft.h"

/*FICHIER A REPRENDRE -- RECLASSER LES FOCNTIONS PAR UTILITE */
int 				mini_prt_handler(char **str, int *pos, t_line *stline)
{
	if (DEBUG_KEY == 1)
		ft_putendl_fd("------- MINI PRT HANDLER ------", 2);

	if ((*str)[0] != '\0')
		fct_insert(str, pos, '\n', stline);
	ft_putstr("> ");
	stline->curs_x = 2;
	stline->curs_y = 0;
	stline->mini_prt = TRUE;
	return (CONTINUE);
}

int					check_end_pipe(char **str, int *pos)
{
	if (DEBUG_KEY == 1)
		ft_putendl_fd("------- CHECK END PIPE ------", 2);

	int					i;
	int					j;

	i = *pos - 1;
	while (str && *str && (*str)[i] && i > 1)
	{
		if ((*str)[i] != ' ' && (*str)[i] != '\t' && (*str)[i] != '\n'
		&& (*str)[i] != '|')
			return (FALSE);
		if ((*str)[i] == '|' && (*str)[i - 1] == '|')
		{
			j = 1;
			i = ft_strlen(*str);
			while ((*str)[--i] == '|' && (*str)[i] == (*str)[i - 1])
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

int					quote_is_close(char **str)
{
	int					i;
	int					quote;
	int					back;

	i = 0;
	quote = 0;
	back = 0;
	while (str && *str && (*str)[i])
	{
		if (quote != DQUOTE && (*str)[i] == QUOTE && (*str)[i - 1]
				&& ((*str)[i - 1] != '\\' || back % 2 == 0))
			quote = (quote != 0 ? 0 : QUOTE);
		if (quote != QUOTE && (*str)[i] == DQUOTE && (*str)[i - 1]
				&& ((*str)[i - 1] != '\\' || back % 2 == 0))
			quote = (quote != 0 ? 0 : DQUOTE);
		if ((*str)[i] == '\\')
			back++;
		else if (back > 0 && ((*str)[i] != '\\' || ((*str)[i] != QUOTE
						|| (*str)[i] != DQUOTE)))
			back = 0;
		i++;
	}
	return (quote);

}

int					fct_return(char **str, int *pos, t_line *stline, // changer pos en p (norme)
					t_history **history)
{
	if (DEBUG_KEY == 1)
		ft_putendl_fd("------- FCT RETURN ------", 2);

	fct_end(str, pos, stline, history);
	stline->quote = quote_is_close(str);
	if (stline->quote != 0 || (*pos > 0 && (*str)[*pos - 1]
	&& (*str)[*pos - 1] == '\\') || stline->hrd.nb > 0
	|| (*pos > 0 && check_end_pipe(str, pos))) // ajout si 'pipe' a la fin de la ligne
	{
		if (stline->hrd.nb > 0)
		{
			if (check_end_heredoc(stline) == BREAK)
				return (BREAK);
		}
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
	if (*str[0] == '\0')
		bi_exit(NULL, &env);
	else
		fct_del(str, pos, stline, history);
	return (TRUE);
}

int					handle_quote(int key, char **str, int *pos, t_line *stline)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- HANDLE QUOTE ------", 2);

	if (*pos > 0 && (*str)[(*pos) - 1] &&  (*str)[(*pos) - 1] == '\\')
		return (FALSE);
	if (key == QUOTE || key == DQUOTE)
	{
		if (stline->quote == key)
			stline->quote = 0;
		else if (stline->quote == 0)
			stline->quote = key;
	}
	return(TRUE);
}

int					event(int k, t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- EVENT ------", 2);

	int					i;
	int					ret;
	static t_key_fct	tbl_keys[18] =
	{
		{RETURN, fct_return}, {BACKSPACE, fct_backspace}, {DOWN, history_down},
		{HOME, fct_home}, {DEL, fct_del}, {CTRL_D, fct_ctrl_d}, {END, fct_end},
		{LEFT, fct_left}, {RIGHT, fct_right}, {UP, history_up},
		{CTRL_LEFT, fct_ctrl_left}, {CTRL_RIGHT, fct_ctrl_right},
		{CTRL_UP, fct_up}, {CTRL_DOWN, fct_down}, {CUT, fct_cut},
		{HIGHLIGHT, fct_highlight}, {PASTE, fct_paste}, {COPY, fct_copy}
	};

	i = -1;
	ret = 0;
	tputs(tgetstr("vi", NULL), 1, my_outc);

//	char *res;
//	tputs(tgetstr("sc", NULL), 1, my_outc);
//	res = tgetstr("cm", NULL);
//	tputs(tgoto(res, 3, 0), 1, my_outc);
//	tputs(tgetstr("ce", NULL), 1, my_outc);
//	ft_putstr("x :");
//	ft_putnbr(stline->curs_x);
//	ft_putstr(" y :");
//	ft_putnbr(stline->curs_y);
//	ft_putstr(" pos :");
//	ft_putnbr(stline->pos);
//	ft_putstr(" mp :");
//	ft_putnbr(stline->mini_prt);
//	tputs(tgetstr("rc", NULL), 1, my_outc);

	while(++i < 18)
	{
		if (tbl_keys[i].key == k)
		{
			ret = tbl_keys[i].fct((stline->hrd.nb > 0 ? &(stline->hrd.line)
						: &(stline->line)),	(stline->hrd.nb > 0 ? &(stline->hrd.pos)
						: &(stline->pos)), stline, history);
			tputs(tgetstr("ve", NULL), 1, my_outc);
			return(ret);
		}
	}
	if (k != TAB && k > 31 && k < 128)
	{
		if (stline->hrd.nb <= 0)
			handle_quote(k, &(stline->line), &(stline->pos), stline);
		fct_insert((stline->hrd.nb > 0 ? &(stline->hrd.line) : &(stline->line)),
				(stline->hrd.nb > 0 ? &(stline->hrd.pos) : &(stline->pos)), k, stline);
	}
	tputs(tgetstr("ve", NULL), 1, my_outc);
	return (TRUE);
}
