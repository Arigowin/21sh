#include "shell.h"
#include "libft.h"

int				reset_stline(t_line *stline)
{
	if (DEBUG == 1)
		printf("------- RESET STLINE ------\n");
	ft_bzero(stline->line, ft_strlen(stline->line));
	stline->pos_line = 0;
	stline->curs_x = PRT_LEN - 1;
	stline->curs_y = 0;
	stline->quote = 0;
	return (TRUE);
}

int				fct_return(t_line *stline, t_history **history)
{
	if (DEBUG_KEY == 1)
		printf("------- FCT RETURN ------\n");
	fct_end(stline, history);
	if (stline->quote == 0)
	{
		if (stline->copy != NULL && stline->cpy_start != -1)
			fct_highlight(stline, history);
		if (stline->line && stline->line[0])
			add_history(history, stline->line);
		ft_putchar('\n');
		return (BREAK);
	}
	else
	{
		fct_insert(stline, '\n', ++(stline->pos_line) - 1);
		ft_putstr("> ");
		stline->curs_x = 2;
		return (CONTINUE);
	}
	return (0);
}

int						fct_ctrl_d(t_line *stline, t_history **history)
{
	if (DEBUG_KEY == 1)
		printf("------- FCT CTRL D ------\n");
	t_duo			*env;

	env = savior(NULL, FALSE);
	if (stline->line[0] == '\0')
		bi_exit(NULL, &env);
	else
		fct_del(stline, history);
	return (TRUE);
}

int				handle_quote(int key, t_line *stline)
{
	if (key == QUOTE || key == DQUOTE) //pb ac quote
	{
		if (stline->quote == key)
			stline->quote = 0;
		else if (stline->quote == 0)
			stline->quote = key;
	}
	return(TRUE);
}

int				event(int key, t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		printf("------- EVENT ------\n");
	int				i;
	static t_key_fct		tbl_keys[18] =
	{
		{RETURN, fct_return}, {BACKSPACE, fct_backspace}, {END, fct_end},
		{HOME, fct_home}, {DEL, fct_del}, {CTRL_D, fct_ctrl_d},
		{LEFT, fct_left}, {RIGHT, fct_right}, {UP, fct_up}, {DOWN, fct_down},
		{CTRL_LEFT, fct_ctrl_left}, {CTRL_RIGHT, fct_ctrl_right},
		{CTRL_UP, fct_ctrl_up}, {CTRL_DOWN, fct_ctrl_down}, {CUT, fct_cut},
		{HIGHLIGHT, fct_highlight}, {PASTE, fct_paste}, {COPY, fct_copy}
	};

	i = 0;
	while(i < 18)
	{
		if (tbl_keys[i].key == key)
			return(tbl_keys[i].fct(stline, history));
		i++;
	}
	if (key != TAB && key > 31 && key < 128)
	{
		handle_quote(key, stline);
		fct_insert(stline, key, ++(stline->pos_line) - 1);
	}
	return (0);
}

//#include <term.h>
//	char *res;
//	tputs(tgetstr("sc", NULL), 1, my_outc);
//	res = tgetstr("cm", NULL);
//	tputs(tgoto(res, 75, 0), 1, my_outc);
//	tputs(tgetstr("ce", NULL), 1, my_outc);
//	ft_putstr("x :");
//	ft_putnbr(stline->curs_x);
//	ft_putstr(" y :");
//	ft_putnbr(stline->curs_y);
//	ft_putstr(" pos_line :");
//	ft_putnbr(stline->pos_line);
//	tputs(tgetstr("rc", NULL), 1, my_outc);

