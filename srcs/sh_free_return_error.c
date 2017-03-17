#include "shell.h"
#include "libft.h"

int					error_clear_node(int ret, int index, char *err,
					t_node **to_free)
{
	if (to_free)
		clear_node(to_free);
	return (sh_error(ret, index, err, NULL));
}

int					error_clear_dblstr(int ret, int index, char **to_free,
					char **to_free_bis)
{
	int					tmp_ret;

	tmp_ret = (index <= 7 || index == 23 || index == 24 || index == 32
				? TRUE : ret);
	tmp_ret = sh_error(tmp_ret, index, NULL, NULL);
	if (to_free)
		ft_strdel(to_free);
	if (to_free_bis)
		ft_strdel(to_free_bis);
	if (index <= 7 || index == 23 || index == 24 || index == 32)
		exit_pgm(EXIT_FAILURE);
	return (tmp_ret);
}

int					error_clear_str(int ret, int index, char *err,
					char **to_free)
{
	int					tmp_ret;

	tmp_ret = (index <= 7 || index == 23 || index == 24 || index == 32
				? TRUE : ret);
	tmp_ret = sh_error(tmp_ret, index, err, NULL);
	if (to_free)
		ft_strdel(to_free);
	if (index <= 7 || index == 23 || index == 24 || index == 32)
		exit_pgm(EXIT_FAILURE);
	return (tmp_ret);
}

int					error_clear_tab(int ret, int index, char *err,
					char ***to_free)
{
	if (to_free)
		free_tab(to_free);
	return (sh_error(ret, index, err, NULL));
}

void				exit_clear_stline(int ret, t_line **stline)
{
	del_stline(stline);
	exit_pgm(ret);
}
