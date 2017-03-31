#include "shell.h"
#include "libft.h"

static int			sh_lvl(void)
{
	char				*lvl;
	char				*new_lvl;

	lvl = NULL;
	if ((lvl = get_env("SHLVL")) == NULL || ft_strcmp(lvl, "-") == 0
	|| ft_isstrnum(lvl) == 0 || ft_strlen(lvl) > 10)
	{
		if ((lvl = ft_strdup("0")) == NULL)
			return (error_clear_str(FALSE, 6, NULL, &lvl));
	}
	new_lvl = ft_itoa(ft_atoi(lvl) + 1);
	if (new_lvl[0] == '-')
	{
		if ((new_lvl = ft_strdup("-")) == NULL)
			return (error_clear_str(FALSE, 6, NULL, &lvl));
	}
	change_env("SHLVL", new_lvl);
	ft_strdel(&lvl);
	ft_strdel(&new_lvl);
	return (TRUE);
}

int					init_pipefd(int pipefd_tab[2][2])
{
	pipefd_tab[0][0] = -2;
	pipefd_tab[0][1] = -2;
	pipefd_tab[1][0] = -2;
	pipefd_tab[1][1] = -2;
	return (TRUE);
}

int					init_env(char **env, t_duo **env_cpy)
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
		return (sh_error(FALSE, 6, NULL, NULL));
	del_env(env_cpy, "OLDPWD");
	savior_env(*env_cpy, TRUE);
	sh_lvl();
	savior_env(*env_cpy, TRUE);
	return (TRUE);
}

int					init_stline(t_line *stline)
{
	char				*s;

	ioctl(0, TIOCGWINSZ, &(stline->win));
	if ((s = ttyname(0)) != NULL)
		return (sh_error(FALSE, 1, NULL, NULL));
	if ((stline->line = ft_strnew(BUFF_SIZE)) == NULL)
		return (sh_error(FALSE, 6, NULL, NULL));
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
	stline->hrd.ctrl_d = FALSE;
	stline->hrd.ptr = NULL;
	stline->hrd.deli = NULL;
	if ((stline->hrd.line = ft_strnew(BUFF_SIZE)) == NULL)
		return (sh_error(FALSE, 6, NULL, NULL));
	savior_stline(stline, TRUE);
	stline->ctrl_c = FALSE;
	return (TRUE);
}

int					reset_stline(t_line *stline)
{
	ft_bzero(stline->line, ft_strlen(stline->line));
	stline->mini_prt = FALSE;
	stline->quote = 0;
	stline->pos = 0;
	stline->curs_y = 0;
	stline->curs_x = PRT_LEN;
	ft_strdel(&(stline->curr_hist));
	stline->ctrl_c = FALSE;
	stline->hrd.nb = 0;
	stline->hrd.pos = 0;
	stline->hrd.ctrl_d = FALSE;
	stline->hrd.ptr = NULL;
	stline->hrd.deli = NULL;
	ft_bzero(stline->hrd.line, ft_strlen(stline->hrd.line));
	return (TRUE);
}
