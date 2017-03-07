#include "shell.h"
#include "libft.h"

int					error_clear_node(int ret, int index, char *err,
	   				t_node **to_free)
{
	clear_node(to_free);
	return (sh_error(ret, index, err, NULL));
}

int					error_clear_str(int ret, int index, char *err,
	   				char **to_free)
{
	ft_strdel(to_free);
	return (sh_error(ret, index, err, NULL));
}

int					error_clear_tab(int ret, int index, char *err,
	   				char ***to_free)
{
	free_tab(to_free);
	return (sh_error(ret, index, err, NULL));
}

