#include <signal.h>
#include <term.h>
#include <unistd.h>
#include "shell.h"
#include "libft.h"

static void			fct_ctrl_c_hrd(int sig)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- FCT CTRL C HRD ------", 2);

	t_line				*stline;
	int					pfd[2];

	(void)sig;
	stline = savior_stline(NULL, FALSE);
	reset_stline(stline);
	stline->ctrl_c = TRUE;
	if (pipe(pfd) == ERROR)
		sh_error(FALSE, 4, NULL, NULL);
	close(pfd[0]);
	write(0, "\0", 1);
	if (dup2(pfd[1], 0) == ERROR)
		sh_error(FALSE, 7, NULL, NULL);
	close(pfd[1]);
	ft_putchar_color(RESET_COLOR, '\n');
}

static void			fct_m_ctrl_c(int sig)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- FCT M CTRL C ------", 2);

	t_line				*stline;
	t_history			**history;

	(void)sig;
	stline = savior_stline(NULL, FALSE);
	history = savior_history(NULL, FALSE);
	if (stline->copy.start != -1)
		fct_highlight(&(stline->line), &(stline->pos), stline, NULL);
	fct_end(&(stline->line), &(stline->pos), stline, NULL);
	while (history && *history && (*history)->next)
		*history = (*history)->next;
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
		signal(SIGWINCH, fct_ctrl_void);
		signal(SIGINT, fct_ctrl_void);
		signal(SIGQUIT, fct_ctrl_void);
		signal(SIGTSTP, fct_ctrl_void);
	}
	else if (loc == 4)
		signal(SIGINT, fct_ctrl_c_hrd);
	return (TRUE);
}
