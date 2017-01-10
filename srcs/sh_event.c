#include "shell.h"
#include "libft.h"

int					reset_stline(t_line *stline)
{
	if (DEBUG == 1)
		printf("------- RESET STLINE ------\n");

	ft_bzero(stline->line, ft_strlen(stline->line));
	stline->pos_line = 0;
	stline->multi_pos = 0;
	stline->curs_x = PRT_LEN;
	stline->curs_y = 0;
	stline->quote = 0;
	stline->hrd.nb = 0;
	stline->hrd.ptr = NULL;
	// liste a free hrd.del
	return (TRUE);
}

int					fct_return(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_KEY == 1)
		printf("------- FCT RETURN ------\n");

	fct_end(str, pos, stline, history);
	stline->multi_pos = 0;
	if (stline->quote != 0 || ((*str)[*pos - 1] && (*str)[*pos - 1] == '\\'))
	{
		fct_insert('\n', str, pos, stline);
		ft_putstr("> ");
		stline->curs_x = 2;
		stline->curs_y = 0;
		return (CONTINUE);
	}
	else if (stline->hrd.nb > 0)
	{
		return_heredoc(stline);
		if (stline->hrd.nb > 0)
			return (CONTINUE);
		return (BREAK);
	}
	else
	{
		if (stline->copy.cpy != NULL && stline->copy.start != -1)
			fct_highlight(str, pos, stline, history);
		if (*str && (*str)[0])
			add_history(history, *str);
		ft_putchar('\n');
		return (BREAK);
	}
	return (FALSE);
}

int					fct_ctrl_d(char **str, int *pos, t_line *stline, t_history **history)
{
	if (DEBUG_KEY == 1)
		printf("------- FCT CTRL D ------\n");

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
		printf("------- HANDLE QUOTE ------\n");

	if ((*str)[(*pos) - 1] &&  (*str)[(*pos) - 1] == '\\')
		return (FALSE);
	if (key == QUOTE || key == DQUOTE) //pb ac quote
	{
		if (stline->quote == key)
			stline->quote = 0;
		else if (stline->quote == 0)
			stline->quote = key;
	}
	return(TRUE);
}

int					event(int key, t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		printf("------- EVENT ------\n");

	int					i;
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
	while(++i < 18)
	{
		if (tbl_keys[i].key == key)
			return(tbl_keys[i].fct(&(stline->line), &(stline->pos_line),
						stline, history));
	}
	if (key != TAB && key > 31 && key < 128)
	{
		handle_quote(key, &(stline->line), &(stline->pos_line), stline);
		fct_insert(key, &(stline->line), &(stline->pos_line), stline);
		fill_heredoc(stline);
	}

#include <term.h>
	char *res;
	tputs(tgetstr("sc", NULL), 1, my_outc);
	res = tgetstr("cm", NULL);
	tputs(tgoto(res, 3, 0), 1, my_outc);
	tputs(tgetstr("ce", NULL), 1, my_outc);
	ft_putstr("x :");
	ft_putnbr(stline->curs_x);
	ft_putstr(" y :");
	ft_putnbr(stline->curs_y);
	ft_putstr(" pos_line :");
	ft_putnbr(stline->pos_line);
	tputs(tgetstr("rc", NULL), 1, my_outc);

	return (0);
}
