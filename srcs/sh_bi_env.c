#include <unistd.h>
#include "libft.h"
#include "shell.h"

static int			print_env(t_duo *env)
{
	while (env)
	{
		ft_putstr(env->name);
		ft_putchar('=');
		ft_putstr_print(env->value);
		ft_putchar('\n');
		env = env->next;
	}
	return (TRUE);
}

static int			format_env(char *arg, int *nb)
{
	int					j;
	char				*key;
	char				*value;

	j = 0;
	while (arg[j] != '=')
		j++;
	key = ft_strsub(arg, 0, j);
	if (valid_env_name(key, "env") != FALSE)
	{
		j++;
		value = ft_strsub(arg, j, ft_strlen(arg) - j);
		change_env(key, value);
		ft_strdel(&value);
		(*nb)++;
	}
	ft_strdel(&key);
	return (TRUE);
}

static int			exec_cmd_env(int i, int len, char **arg)
{
	char				**cmd;
	int					pipefd_tab[2][2];
	int					j;

	j = 0;
	cmd = NULL;
	if ((cmd = (char **)malloc(sizeof(char *) * ((len - i) + 1))) == NULL)
		return (sh_error(FALSE, 6, NULL, NULL));
	while (arg[i])
	{
		if ((cmd[j] = ft_strdup(arg[i])) == NULL)
			return (sh_error(FALSE, 6, NULL, NULL));
		j++;
		i++;
	}
	cmd[j] = NULL;
	init_pipefd(pipefd_tab);
	handle_fork(pipefd_tab, savior_tree(NULL, FALSE), NULL, cmd);
	free_tab(&cmd);
	return (TRUE);
}

static int			modif_env(char **arg, t_duo *env, int len, int i)
{
	int					nb;

	nb = 0;
	savior_env(env, TRUE);
	while (arg[i])
	{
		if (ft_strchr(arg[i], '=') != NULL)
			format_env(arg[i], &nb);
		else
			break ;
		i++;
	}
	if (i < len)
		exec_cmd_env(i, len, arg);
	else
		print_env(env);
	duo_del(&env);
	return (TRUE);
}

int					bi_env(char **arg, t_duo **env)
{
	int					len;
	int					i;

	len = tbl_len(arg);
	i = 1;
	if (check_opt(arg, &i) == ERROR)
		return (FALSE);
	if (len > 1)
	{
		if (modif_env(arg, cpy_duo(*env), len, i) == ERROR)
			return (ERROR);
	}
	else
		print_env(*env);
	savior_env(*env, TRUE);
	return (TRUE);
}
