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
	{
		*env_cpy = tbl_to_duo(cpy, '=');
		free_tab(&cpy);
	}
	else
		*env_cpy = tbl_to_duo(env, '=');
	if (env_cpy == NULL && *env_cpy == NULL)
		return (sh_error(TRUE, 6, NULL, NULL));
	del_env(env_cpy, "OLDPWD");
	savior(*env_cpy, TRUE);
	return (TRUE);
}

int					init_stline(t_line *stline)
{
	if (ttyname(0) != NULL && ioctl(0, TIOCGWINSZ, &(stline->win)) == ERROR)
		return (sh_error(TRUE, 1, NULL, NULL));
	if ((stline->line = ft_strnew(BUFF_SIZE)) == NULL)
		return (sh_error(TRUE, 6, NULL, NULL));
	stline->mini_prt = FALSE;
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
		return (sh_error(TRUE, 6, NULL, NULL));
		 /* free: stline->line */
	savior_stline(stline, TRUE);
	stline->ctrl_c = FALSE;
	return (TRUE);
}

int					reset_stline(t_line *stline)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- RESET STLINE ------", 2);

	ft_bzero(stline->line, BUFF_SIZE);
	stline->mini_prt = FALSE;
	stline->quote = 0;
	stline->pos = 0;
	stline->curs_y = 0;
	stline->curs_x = PRT_LEN;
	ft_strdel(&( stline->curr_hist));
	stline->hrd.nb = 0;
	stline->hrd.pos = 0;
	stline->hrd.ptr = NULL;
	stline->hrd.deli = NULL;
	ft_bzero(stline->hrd.line, BUFF_SIZE);
	return (TRUE);
}
