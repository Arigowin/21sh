#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int				tree_traversal(t_node *tree)
{
//	printf("tree : %p\n", tree);
	if (tree->left != NULL)
//	{printf("toto2\n");
		tree_traversal(tree->left);
//	}
//	printf("toto1/2\n");
	if (tree->right != NULL)
		tree_traversal(tree->right);
//	ft_putstr(tree->data);
	ft_putstr("-->");
	return (0);
}

char			**read_n_check(char *special, char *read_buff)
{
	t_e_list		*l_expr;
	t_node			*tree;

	(void)special;
	l_expr = NULL;
	lexer_1(read_buff, &l_expr);
	printf("list:\n");
	t_e_list *tmp = l_expr;
	while (tmp != NULL){printf("(((%s -- %d)))\n",tmp->data, tmp->type); tmp=tmp->next;}
	lexer_2(&l_expr);
	printf("list ap enum:\n");
	tmp = l_expr;
	while (tmp != NULL){printf("(((%s --> %d)))\n",tmp->data, tmp->type); tmp=tmp->next;}
	tree = parser(&l_expr);
	tree_traversal(tree);
	//	tbl = lst_to_tbl(arg);
	//	free_lst(&arg);
	//	return (tbl);
	return (NULL);
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
	printf("trololopouetpouet....\n");
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
	ft_putstr("la fct read....\n");
	int				key;
	int				ret;

	ret = 0;
	stline->curs_x = 3;
	(void)history;
	while ((ret = read(0, &key, 8)) > 0)
	{
		//printf("%d\n", key); // !!!!!!!!!!!!!!! PRINTF !!!!!!!!!!!!!!!!!!!
		if (key == CTRL_D && stline->line[0] == '\0')
			bi_exit(NULL, env_cpy);
		else if (key == CTRL_D)
			key = DEL;
		if (event(key, stline, history) == 1)
			break ;
		key = 0;
	}
	if (ret <= 0)
		bi_exit(NULL, env_cpy);
	if (check_after_read(stline, env_cpy) == -1)
		return (-1);
	return (0);
}
