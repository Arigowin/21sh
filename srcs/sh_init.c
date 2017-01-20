#include "shell.h"
#include "libft.h"

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
		/* MSG ret: ERROR exit: TRUE msg: "ioctl request window size fail" */
		return (ERROR);

	if ((stline->line = ft_strnew(BUFF_SIZE)) == NULL)
		/* MSG ret: ERROR exit: TRUE msg: "malloc fail" */
		return (ERROR);
	if ((stline->hrd.line = ft_strnew(BUFF_SIZE)) == NULL)
		/* MSG ret: ERROR exit: TRUE msg: "malloc fail" 
		 * free: stline->line */
		return (ERROR);
	stline->copy.cpy = NULL;
	stline->copy.start = -1;
	stline->copy.bkup = NULL;
	stline->hrd.nb = 0;
	stline->hrd.pos = 0;
	stline->hrd.ptr = NULL;
	stline->hrd.deli = NULL;
	savior_stline(stline, TRUE);
	return (TRUE);
}

int					reset_stline(t_line *stline)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- RESET STLINE ------", 2);

	ft_bzero(stline->line, ft_strlen(stline->line));
	ft_bzero(stline->hrd.line, ft_strlen(stline->hrd.line));
	stline->pos = 0;
	stline->curs_x = PRT_LEN;
	stline->curs_y = 0;
	stline->quote = 0;
	stline->hrd.nb = 0;
	stline->hrd.pos = 0;
	stline->hrd.ptr = NULL;
	stline->hrd.deli = NULL;
	return (TRUE);
}
