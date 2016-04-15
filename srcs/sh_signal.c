#include <signal.h>
#include "minishell.h"
#include "libft.h"

static void		fct_m_ctrl_c(int sig)
{
	t_duo			*env;

	(void)sig;
	env = NULL;
	ft_putendl("");
	env = savior(env);
	display_prompt(env);
	return ;
}

static void		fct_ctrl_void(int sig)
{
	(void)sig;
	ft_putendl("");
	return ;
}

static void		fct_true_void(int sig)
{
	(void)sig;
	return ;
}

int				check_signal(int loc)
{
	if (loc == 1)
	{
		signal(SIGQUIT, fct_true_void);
		signal(SIGTSTP, fct_true_void);
		signal(SIGINT, fct_m_ctrl_c);
	}
	else if (loc == 2)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
	else if (loc == 3)
	{
		signal(SIGINT, fct_ctrl_void);
		signal(SIGQUIT, fct_ctrl_void);
		signal(SIGTSTP, fct_ctrl_void);
	}
	return (0);
}
