/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/05 18:06:15 by avacher           #+#    #+#             */
/*   Updated: 2016/04/05 18:06:15 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"
#include "libft.h"

static int		cd_usage(char **arg, char **path, char *tmp_old_pwd)
{
	if (arg[1] && arg[2])
	{
		ft_putendl("minishell: cd: too many arguments.");
		return (-1);
	}
	if (arg[1] && arg[1][0] == '-' && arg[1][1])
	{
		ft_putstr("minishell: cd: -");
		ft_putchar(arg[1][1]);
		ft_putendl(": invalid option\ncd: usage: cd [dir]");
		return (-1);
	}
	if (arg[1] && arg[1][0] == '-')
	{
		if (((*path) = ft_strdup(tmp_old_pwd)) == NULL)
		{
			ft_putendl("minishell: cd: variable $OLDPWD not set");
			return (-2);
		}
	}
	return (0);
}

static int		access_home(char **arg, char *home, char *tmp)
{
	ft_putstr("minishell: cd: ");
	if (home == NULL && !arg[1])
		ft_putendl("no $HOME variable set");
	else if (home == NULL && arg[1] && arg[1][0] == '~')
	{
		ft_putstr(tmp);
		ft_putendl(": no $HOME variable set");
	}
	return (-1);
}

static int		cd_access(char **arg, char *path, t_duo *env)
{
	char		*tmp;
	char		*home;

	tmp = NULL;
	home = get_env(&env, "HOME");
	if (path == NULL && arg[1] && arg[1][0] != '-')
		tmp = ft_strdup(arg[1]);
	if (path)
		tmp = ft_strdup(path);
	if (home == NULL && (!arg[1] || (arg[1] && arg[1][0] == '~')))
		return (access_home(arg, home, tmp));
	if (arg[1] && chdir(tmp) == -1)
	{
		ft_putstr("minishell: cd: ");
		ft_putstr(tmp);
		if ((access(tmp, F_OK)) == -1)
			ft_putendl(": no such file or directory");
		else
			ft_putendl(": permission denied");
		return (-1);
	}
	free(home);
	ft_strdel(&tmp);
	return (0);
}

static int		cd_home(t_duo **env)
{
	char		*home;

	if ((home = get_env(env, "HOME")) == NULL)
	{
		ft_putendl("minishell: cd: no $HOME variable set");
		return (-1);
	}
	if (chdir(home) == -1)
		return (-1);
	free(home);
	return (0);
}

int				bi_cd(char **arg, t_duo **env)
{
	char		*tmp_pwd;
	char		*tmp_old_pwd;
	char		*path;
	int			i;

	if ((tmp_pwd = get_env(env, "PWD")) == NULL)
		return (-1);
	tmp_old_pwd = get_env(env, "OLDPWD");
	if (!arg[1])
		cd_home(env);
	i = 0;
	path = NULL;
	i += cd_usage(arg, &path, tmp_old_pwd);
	if (arg[1] && i != -2)
		i += cd_access(arg, path, *env);
	ft_strdel(&path);
	if (i < 0 || (path = getcwd(path, MAX_PATH)) == NULL)
		return (-1);
	change_env(env, "OLDPWD", tmp_pwd);
	change_env(env, "PWD", path);
	ft_strdel(&path);
	ft_strdel(&tmp_pwd);
	ft_strdel(&tmp_old_pwd);
	return (0);
}
