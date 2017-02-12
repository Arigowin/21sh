#include "shell.h"
#include "libft.h"

static int			bi_opt(char *arg, char *bi, int *no_more, char *handled_opt) //static ac check opt
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI OPT ------", 2);

	int					i;

	i = 1;
	if (*no_more == TRUE)
		return (FALSE);
	if (arg && arg[0] && arg[0] == '-' && arg[1] && arg[1] == '-')
		*no_more = TRUE;
	else if (arg && arg[0] && arg[0] == '-' && arg[1])
	{
		while (arg[i])
		{
			if (ft_strchr(handled_opt, arg[i]) == NULL)
				return (sh_error(22, &arg[i], bi));
			i++;
		}
	}
	return (TRUE);
}

int					check_opt(char **arg, int *i)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- CHECK OPT ------", 2);

	int					no_more;
	int					ret;

	ret = TRUE;
	no_more = FALSE;
	while (arg[*i] && arg[*i][0] && arg[*i][0] == '-' && arg[*i][1])
	{
		if ((ret = bi_opt(arg[*i], arg[0], &no_more, "")) != TRUE)
			break ;
		(*i)++;
	}
	if (ret == -2)
		return (ERROR);
	return (ret);
}
