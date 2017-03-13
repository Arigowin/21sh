#include <term.h>
#include "shell.h"
#include "libft.h"

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
			modif_history(history, *str, stline->mini_prt);
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

#include<term.h>
	char *res;
		tputs(tgetstr("sc", NULL), 1, my_outc);
		res = tgetstr("cm", NULL);
		tputs(tgoto(res, 10, 5), 1, my_outc);
		tputs(tgetstr("ce", NULL), 1, my_outc);
		ft_putstr("-----");
		ft_putstr("x :");
		ft_putnbr(stline->curs_x);
		ft_putstr(" y :");
		ft_putnbr(stline->curs_y);
		ft_putstr(" pos :");
		ft_putnbr(stline->pos);
		ft_putstr("-----");
		//ft_putstr(" mini prt :");
		//ft_putnbr(stline->mini_prt);
		tputs(tgetstr("rc", NULL), 1, my_outc);

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
