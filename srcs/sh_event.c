#include "shell.h"
#include "libft.h"

int					fct_return(char **str, int *pos, t_line *stline,
					t_history **history)
{
	if (DEBUG_KEY == 1)
		printf("------- FCT RETURN ------\n");

	fct_end(str, pos, stline, history);
	if (stline->quote != 0 || ((*str)[*pos - 1] && (*str)[*pos - 1] == '\\'))
	{
		fct_insert(str, pos, '\n', stline);
		ft_putstr("> ");
		stline->curs_x = 2;
		stline->curs_y = 0;
		return (CONTINUE);
	}
//	else if (stline->hrd.nb > 0)
//	{
//		return_heredoc(stline);
//		if (stline->hrd.nb > 0)
//			return (CONTINUE);
//		return (BREAK);
//	}
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

//passera a la norme en coupant les ternaires et virer les accolades
int					event(int k, t_line *stline, t_history **history)
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
		if (tbl_keys[i].key == k)
			return(tbl_keys[i].fct((stline->hrd.nb > 0 ? &(stline->hrd.line)
			: &(stline->line)),	(stline->hrd.nb > 0 ? &(stline->hrd.pos)
			: &(stline->pos)), stline, history));
	if (k != TAB && k > 31 && k < 128)
	{
		if (stline->hrd.nb == -1)
			handle_quote(k, &(stline->line), &(stline->pos), stline);
		fct_insert((stline->hrd.nb > 0 ? &(stline->hrd.line) : &(stline->line)),
		(stline->hrd.nb > 0 ? &(stline->hrd.pos) : &(stline->pos)), k, stline);
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
	ft_putstr(" pos :");
	ft_putnbr(stline->pos);
	tputs(tgetstr("rc", NULL), 1, my_outc);

	return (0);
}
