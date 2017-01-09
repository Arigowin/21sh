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
	del_env(env_cpy, "OLDPWD");
	savior(*env_cpy, TRUE);
	free_tab(&cpy);
	return (0);
}

int					init(t_line *stline)
{
	ioctl(0, TIOCGWINSZ, &(stline->win));

	if ((stline->line = ft_strnew(BUFF_SIZE)) == NULL)
		return (ERROR);
	stline->copy.cpy = NULL;
	stline->copy.start = -1;
	stline->copy.bkup = NULL;
	stline->hrd.nb = 0;
	stline->hrd.deli = NULL;
	stline->hrd.ptr = NULL;
	savior_stline(stline, TRUE);
	return (TRUE);
}
