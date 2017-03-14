#include <stdio.h>
#include "shell.h"
#include "libft.h"

static char			*get_path(void)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- GET PATH ------", 2);

	char				*path;
	char				*tmp;
	char				*home;

	home = get_env("HOME");
	if ((path = get_env("PWD")) == NULL)
		return (NULL);
	tmp = ft_strsub(path, 0, ft_strlen(home));
	if (home && ft_strcmp(home, tmp) == 0)
	{
		ft_strdel(&tmp);
		tmp = ft_strsub(path, ft_strlen(home), ft_strlen(path));
		ft_strdel(&path);
		path = ft_properjoin("~", tmp);
	}
	ft_strdel(&home);
	ft_strdel(&tmp);
	return (path);
}

// nom de fonction a modifier si necessaire
int					miniprt_reset_stline(t_line *stline)
{
	ft_putstr("> ");
	stline->curs_x = 2;
	stline->curs_y = 0;
	stline->mini_prt = TRUE;
	return (TRUE);
}

/*FICHIER A REPRENDRE -- RECLASSER LES FOCNTIONS PAR UTILITE */
int					mini_prt_handler(char **str, int *pos, t_line *stline)
{
	if (DEBUG_KEY == 1)
		ft_putendl_fd("------- MINI PRT HANDLER ------", 2);

	if ((*str)[0] != '\0')
		fct_insert(str, pos, '\n', stline);
	else if (!ft_strcmp(*str, ""))
		fct_insert(str, pos, '\n', stline);
	else if (quote_is_close(str) == 0)
		fct_insert(str, pos, '\n', stline);
	miniprt_reset_stline(stline);
	return (CONTINUE);
}

int					display_prompt(void)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- DISPLAY PROMPT ------", 2);

	char				*name;
	char				*path;
	char				*shlvl;

	path = get_path();
	name = get_env("LOGNAME");
	if (name)
		ft_putstr_color("\033[34;1m", name);
	if ((shlvl = get_env("SHLVL")))
	{
		ft_putchar_color("\033[31m", '[');
		ft_putstr_color("\033[31m", shlvl);
		ft_putchar_color("\033[31m", ']');
	}
	if (name && path)
		ft_putstr(":");
	if (path)
		ft_putstr_color("\033[32;1m", path);
	if (path || name)
		ft_putchar('\n');
	ft_putstr_color("\033[36m", "> ");
	ft_strdel(&name);
	ft_strdel(&path);
	ft_strdel(&shlvl);
	return (TRUE);
}

int					fill_path(char ***env)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- FILL PATH ------", 2);

	char				*tmp;

	tmp = NULL;
	if (((*env) = (char **)malloc(sizeof(char *) * 4)) == NULL)
		return (sh_error(FALSE, 6, NULL, NULL));
	if ((tmp = getcwd(tmp, MAX_PATH)) == NULL)
		return (sh_error(FALSE, 6, NULL, NULL));
	if (((*env)[0] =
	ft_strdup("PATH=/usr/local/bin:/usr/local/sbin\
:/usr/bin:/usr/sbin:/bin:/sbin:.")) == NULL)
		return (sh_error(FALSE, 6, NULL, NULL));
	if (((*env)[1] = ft_properjoin("PWD=", tmp)) == NULL)
	{
		ft_strdel(&tmp);
		return (sh_error(FALSE, 6, NULL, NULL));
	}
	if (((*env)[2] = ft_strdup("TERM=xterm")) == NULL)
		return (sh_error(FALSE, 6, NULL, NULL));
	(*env)[3] = NULL;
	ft_strdel(&tmp);
	return (TRUE);
}
