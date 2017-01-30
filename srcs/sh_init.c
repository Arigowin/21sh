#include "shell.h"
#include "libft.h"

int					init_pipefd(int pipefd_tab[2][2])
{
	pipefd_tab[0][0] = -2;
	pipefd_tab[0][1] = -2;
	pipefd_tab[1][0] = -2;
	pipefd_tab[1][1] = -2;
	return (TRUE);
}

int					init_env(char **env, t_duo **env_cpy) //ok
{
	char				**cpy;

	cpy = NULL;
	if (tbl_len(env) == 0)
		fill_path(&cpy);
	if (cpy)
		*env_cpy = tbl_to_duo(cpy, '=');
	else
		*env_cpy = tbl_to_duo(env, '=');
	if (env_cpy == NULL && *env_cpy == NULL)
		/* MSG ret: ERROR exit: TRUE msg: "malloc fail"
		 * free: cpy */
		return (ERROR);
	del_env(env_cpy, "OLDPWD");
	savior(*env_cpy, TRUE);
	free_tab(&cpy);
	return (TRUE);
}

int					init_stline(t_line *stline)
{
	if (ioctl(0, TIOCGWINSZ, &(stline->win)) == ERROR)
		/* RET: error EXIT: true MSG: "ioctl: cannot get window size" */
		return (ERROR);
	if ((stline->line = ft_strnew(BUFF_SIZE)) == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
		return (ERROR);
	stline->mini_ptr = FALSE;
	stline->quote = 0;
	stline->pos = 0;
	stline->curs_y = 0;
	stline->curs_x = PRT_LEN;
	stline->curr_hist = NULL;
	stline->copy.cpy = NULL;
	stline->copy.bkup = NULL;
	stline->copy.start = -1;
	stline->hrd.nb = 0;
	stline->hrd.pos = 0;
	stline->hrd.ptr = NULL;
	stline->hrd.deli = NULL;
	if ((stline->hrd.line = ft_strnew(BUFF_SIZE)) == NULL)
		/* MSG ret: ERROR exit: TRUE msg: "malloc fail"
		 * free: stline->line */
		return (ERROR);
	savior_stline(stline, TRUE);
	return (TRUE);
}

int					reset_stline(t_line *stline)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- RESET STLINE ------", 2);

	ft_bzero(stline->line, ft_strlen(stline->line));
	stline->mini_ptr = FALSE;
	stline->quote = 0;
	stline->pos = 0;
	stline->curs_y = 0;
	stline->curs_x = PRT_LEN;
	ft_strdel(&( stline->curr_hist));
	stline->hrd.nb = 0;
	stline->hrd.pos = 0;
	stline->hrd.ptr = NULL;
	stline->hrd.deli = NULL;
	ft_bzero(stline->hrd.line, ft_strlen(stline->hrd.line));
	return (TRUE);
}
