#include "shell.h"
#include "libft.h"

int				reset_stline(t_line *stline)
{
	ft_bzero(stline->line, ft_strlen(stline->line));
	stline->pos_line = 0;
	stline->curs_x = PRT_LEN - 1;
	stline->curs_y = 0;
	stline->quote = 0;
	return (TRUE);
}

int				event_return(int key, t_line *stline, t_history **history)
{
	spec_key(END, stline);
	if (key == RETURN && stline->quote == 0)
	{
		if (stline->copy != NULL && stline->cpy_start != -1)
			copy_paste(HIGHLIGHT, stline);

		if (stline->line && stline->line[0])
			add_history(history, stline->line);

		ft_putchar(key);
		return (1);
	}
	else if (key == RETURN && stline->quote != 0)
	{
		insert(stline, '\n', ++(stline->pos_line) - 1);
		ft_putstr("> ");
		stline->curs_x = 2;
		//	if (stline->quote == QUOTE)
		//	{
		//		ft_putstr("quote> ");
		//		stline->curs_x = 7;
		//	}
		//	if (stline->quote == DQUOTE)
		//	{
		//		ft_putstr("dquote> ");
		//		stline->curs_x = 8;
		//	}
		return (2);
	}
	return (0);
}

int				event(int key, t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		printf("------- EVENT ------\n");
	t_duo			*env;
	static const t_key		tbl_keys[] =
	{
		{RETURN, fct_return}, {BACKSPACE, fct_backspace}, {END, fct_end},
		{HOME, fct_home}, {DEL, fct_del}, {CTRL_D, fct_ctrl_d},
		{LEFT, fct_left}, {RIGHT, fct_right}, {UP, fct_up}, {DOWN, fct_down},
		{CTRL_LEFT, fct_ctrl_left}, {CTRL_RIGHT, fct_ctrl_right},
		{CTRL_UP, fct_ctrl_up}, {CTRL_DOWN, fct_ctrl_down}, {CUT, fct_cut},
		{HIGHLIGHT, fct_highlight}, {PASTE, fct_paste}, {COPY, fct_copy}
	};

	env = savior(NULL, FALSE);
	if (key == RETURN)
		return (event_return(key, stline, history));
	else if (key == BACKSPACE)
		backspace(stline);
	else if (key == CTRL_D && stline->line[0] == '\0')
		bi_exit(NULL, &env);
	else if (key == END || key == HOME || key == DEL || key == CTRL_D)
		spec_key(key, stline);
	else if (key == LEFT || key == RIGHT || key == OP_RIGHT || key == OP_LEFT)
		move(key, stline);
	else if (key == CTRL_UP || key == CTRL_DOWN)
		move_up_down(key, stline);
	else if (key == UP || key == DOWN)
		nav_history(key, history, stline);
	else if (key == HIGHLIGHT || key == PASTE || key == COPY || key == CUT)
		copy_paste(key, stline);
	else if (key != TAB && key < 256)
	{
		if (key == QUOTE || key == DQUOTE) //pb ac quote
		{
			if (stline->quote == key)
				stline->quote = 0;
			else if (stline->quote == 0)
				stline->quote = key;
		}
		insert(stline, key, ++(stline->pos_line) - 1);
	}

#include <term.h>
	char *res;
	tputs(tgetstr("sc", NULL), 1, my_outc);
	res = tgetstr("cm", NULL);
	tputs(tgoto(res, 75, 0), 1, my_outc);
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
