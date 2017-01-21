#include <signal.h>
#include <term.h>
#include "shell.h"
#include "libft.h"

//static void			win_resize(int sig)
//{
//	t_line				*stline;
//	char 				*save_line;
//	int 				save_pos;
//	int					i;
//
//	(void)sig;
//	i = -1;
//	stline = savior_stline(NULL, FALSE);
//	save_line = ft_strdup(stline->line);
//	save_pos = stline->pos;
//	fct_end(&(stline->line), &(stline->pos), stline, NULL);
//	while (stline->pos > 0)
//		fct_backspace(&(stline->line), &(stline->pos), stline, NULL);
//	ioctl(0, TIOCGWINSZ, &(stline->win));
////	while (save_line[++i])
////		fct_insert(&(stline->line), &(stline->pos), save_line[i], stline);
////	while (stline->pos > save_pos)
////		fct_left(&(stline->line), &(stline->pos), stline, NULL);
//	free(save_line);
//}

static void			fct_m_ctrl_c(int sig)
{
	t_line				*stline;

	(void)sig;
	stline = savior_stline(NULL, FALSE);
	if (stline->copy.start != -1)
		fct_highlight(&(stline->line), &(stline->pos), stline, NULL);
	fct_end(&(stline->line), &(stline->pos), stline, NULL);
	reset_stline(stline);
	ft_putchar_color(RESET_COLOR, '\n');
	display_prompt();
}

static void			fct_ctrl_void(int sig)
{
	(void)sig;
	ft_putendl("");
}

static void			fct_true_void(int sig)
{
	(void)sig;
}

int					check_signal(int loc)
{
	if (loc == 1)
	{
//		signal(SIGWINCH, win_resize);
		signal(SIGWINCH, SIG_DFL);
		signal(SIGQUIT, fct_true_void);
		signal(SIGTSTP, fct_true_void);
		signal(SIGINT, fct_m_ctrl_c);
	}
	else if (loc == 2)
	{
		signal(SIGWINCH, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
	else if (loc == 3)
	{
		signal(SIGWINCH,fct_ctrl_void);
		signal(SIGINT, fct_ctrl_void);
		signal(SIGQUIT, fct_ctrl_void);
		signal(SIGTSTP, fct_ctrl_void);
	}
	return (TRUE);
}
