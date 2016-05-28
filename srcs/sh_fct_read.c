#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

static t_e_list			*expr_new(char *content)
{
	t_e_list		*new;

	if ((new = (t_e_list *)malloc(sizeof(t_e_list))) == NULL)
		return (NULL);
	new->data = NULL;
	new->type = NONE;
	new->next = NULL;
	if (content == NULL)
		return (NULL);
	new->data = ft_strdup(content);
	return (new);
}

static void				expr_pushbk(t_e_list **l_expr, char content[])
{
	t_e_list		*tmp;

	tmp = *l_expr;
	if (tmp == NULL)
	{
		*l_expr = expr_new(content);
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = expr_new(content);
}

t_e_list				lexer_1(char *read_buff)
{
	t_e_list		*l_expr;
	char			tmp[1024];
	char			**tbl;
	int				i;
	int				j;
	int				k;

	ft_bzero(tmp, 1024);
	l_expr = NULL;
	i = -1;
	k = 0;
	while (read_buff[++i])
	{
		j = 0;
		while (read_buff[i] != special[j] && special[j])
			j++;
		if (read_buff[i] == special[j] && tmp[0] != '\0')
		{
			expr_pushbk(&l_expr, tmp);
			ft_bzero(tmp, 1024);
			k = 0;
		}
		else if (read_buff[i] != special[j])
			tmp[k++] = read_buff[i];
	}
	if (ft_strlen(tmp))
		expr_pushbk(&l_expr, tmp);
}

char			**read_n_check(char *special, char *read_buff)
{
	lexer_1(read_buff);

	tbl = lst_to_tbl(arg);
	free_lst(&arg);
	return (tbl);
}

int				check_home(char **cmd)
{
	int			i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i][0] == '~')
		{
			ft_putstr("21sh: ");
			ft_putstr(cmd[0]);
			ft_putendl(": no $HOME variable set");
			return (-1);
		}
		i++;
	}
	return (0);
}

int				check_after_read(t_line *stline, t_duo **env_cpy)
{
	char			**cmd;
	int				i;

	i = 0;
	if ((cmd = read_n_check(SEP, stline->line)) == NULL || cmd[0] == NULL)
		return (-1);
	while (cmd[++i])
	{
		if (cmd[i][0] == '~')
			manage_tilde(env_cpy, &cmd[i]);
	}
	if (handle_builtin(cmd, env_cpy) != 0)
		return (-1);
	if (check_home(cmd) < 0)
		return (-1);
	father_n_son(cmd, env_cpy);
	free_tab(&cmd);
	return (0);
}

int				fct_read(t_line *stline, t_duo **env_cpy, t_history **history)
{
	int				key;
	int				ret;

	ret = 0;
	stline->curs_x = 3;
	while ((ret = read(0, &key, 8)) > 0)
	{
		printf("%d\n", key); // !!!!!!!!!!!!!!! PRINTF !!!!!!!!!!!!!!!!!!!
		if (key == CTRL_D && stline->line[0] == '\0')
			bi_exit(NULL, env_cpy);
		else if (key == CTRL_D)
			key = DEL;
		if (event(key, stline, history) == 1)
			break ;
	}
	if (ret <= 0)
		bi_exit(NULL, env_cpy);
	if (check_after_read(stline, env_cpy) == -1)
		return (-1);
	return (0);
}
