/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_father_n_son.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/05 18:06:15 by avacher           #+#    #+#             */
/*   Updated: 2016/04/05 18:06:15 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft.h"

static char		*join_exe(char *s1, char *s2)
{
	char		*rlt;
	char		*tmp;

	if (s2[0] == '/' || (s2[0] == '.' && s2[1] == '/'))
		return (s2);
	tmp = ft_strjoin("/", s2);
	rlt = ft_strjoin(s1, tmp);
	ft_strdel(&tmp);
	return (rlt);
}

static int		check_fct(char **cmd, char **env, t_duo **env_cpy)
{
	char		**path;
	char		*tmp;
	int			i;

	tmp = get_env(env_cpy, "PATH");
	if (tmp == NULL)
		fill_path(&path);
	else
		path = read_n_check(":", tmp);
	i = 0;
	while (path[i])
	{
		tmp = join_exe(path[i], cmd[0]);
		execve(tmp, cmd, env);
		i++;
	}
	free(tmp);
	return (0);
}

int				father_n_son(char **cmd, char **env, t_duo **env_cpy)
{
	pid_t		father;
	int			stat_loc;

	father = fork();
	if (father > 0)
	{
		check_signal(3);
		wait(&stat_loc);
	}
	if (father == 0)
	{
		check_signal(2);
		check_fct(cmd, env, env_cpy);
		ft_putstr("minishell: ");
		ft_putstr(cmd[0]);
		ft_putendl(": command not found");
		exit(EXIT_FAILURE);
	}
	return (0);
}
