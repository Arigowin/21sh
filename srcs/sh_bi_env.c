#include "libft.h"
#include "shell.h"

static int			print_env(t_duo *env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- PRINT ENV ------", 2);

	while (env)
	{
		ft_putstr(env->name);
		ft_putchar('=');
		ft_putendl(env->value);
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
	if (valid_env_name(key) != ERROR)
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
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- EXEC CMD ENV ------", 2);

	char				**cmd;
	int					pipefd_tab[2][2];
	int					j;

	j = 0;
	cmd = NULL;
	if ((cmd = (char **)malloc(sizeof(char *) * ((len - i) + 1))) == NULL)
		return (ERROR);
	printf("exec cmd : [");
	while (arg[i])
	{
		if ((cmd[j] = ft_strdup(arg[i])) == NULL)
			return (ERROR);
		printf("%s ", cmd[j]);
		j++;
		i++;
	}
	cmd[j] = NULL;
	printf("]\n");
	printf("[[%d]]\n", j);
	init_pipefd(pipefd_tab);
	if (check_builtin(-2, cmd, pipefd_tab, NULL) != FALSE)
		return (TRUE);
	handle_fork(pipefd_tab, savior_tree(NULL, FALSE), NULL, cmd);
	free_tab(&cmd);
	return (TRUE);
}

static int			modif_env(char **arg, t_duo *env, int len, int i)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- MODIF ENV ------", 2);

	int					nb;

	nb = 0;
	while (arg[i])
	{
		if (strchr(arg[i], '=') != NULL)
			format_env(arg[i], &nb);
		else
			break;
		i++;
	}
	if (i < len)
		exec_cmd_env(i, len, arg);
	else
		print_env(env);
	while (nb > 0)
	{
		last_duol_del(&env);
		nb--;
	}
	return (TRUE);
}

int					bi_env(char **arg, t_duo **env)
{
	if (DEBUG_BUILTIN == 1)
		ft_putendl_fd("------- BI ENV ------", 2);
	int					len;
	int					i;

	len = tbl_len(arg);
	i = 1;
	if (check_opt(arg, &i) == ERROR)
		return (ERROR);
	if (len > 1)
	{
		if (modif_env(arg, *env, len, i) == ERROR)
			return (ERROR);
	}
	else
		print_env(*env);
	return (TRUE);
}
