#include "libft.h"
#include "shell.h"

static int			checktty_tool(char **tmp)
{
	char				buff[BUFF_SIZE + 1];
	int					ret;
	char				*tmp2;

	while ((ret = read(0, &buff, BUFF_SIZE)) > 0)
	{
		buff[ret] = '\0';
		if (*tmp != NULL)
		{
			if ((tmp2 = ft_strjoin(*tmp, buff)) == NULL)
				return (sh_error(FALSE, 6, NULL, NULL));
			ft_strdel(tmp);
			if ((*tmp = ft_strdup(tmp2)) == NULL)
			{
				ft_strdel(&tmp2);
				return (sh_error(FALSE, 6, NULL, NULL));
			}
			ft_strdel(&tmp2);
		}
		else if ((*tmp = ft_strdup(buff)) == NULL)
			return (sh_error(FALSE, 6, NULL, NULL));
	}
	if (ret == ERROR)
		return (ERROR);
	return (TRUE);
}

static int			checktty_tool2(t_line *stline, char **cmd)
{
	int					i;

	i = 0;
	while (cmd[i])
	{
		ft_strdel(&(stline->line));
		if ((stline->line = ft_strdup(cmd[i])) == NULL)
			return (sh_error(FALSE, 6, NULL, NULL));
		if (check_after_read(stline, NULL) == ERROR)
			return (ERROR);
		i++;
	}
	return (TRUE);
}

int					checktty(t_line *stline)
{
	if (DEBUG == 0)
		ft_putendl_fd("--------- CHECKTTY ------", 2);

	char				*tmp;
	char				**cmd;

	tmp = NULL;
	if (!isatty(0))
	{
		if (checktty_tool(&tmp) == ERROR)
			exit_clear_stline(EXIT_FAILURE, &stline);
		if (tmp)
		{
			if ((cmd = ft_strsplit(tmp, '\n')) == NULL)
				exit_clear_stline(EXIT_FAILURE, &stline);
			ft_strdel(&tmp);
			if (checktty_tool2(stline, cmd) == ERROR)
				exit_clear_stline(EXIT_FAILURE, &stline);
			free_tab(&cmd);
			exit_clear_stline(EXIT_SUCCESS, &stline);
		}
		ft_strdel(&tmp);
		exit_clear_stline(EXIT_FAILURE, &stline);
	}
	return (TRUE);
}
