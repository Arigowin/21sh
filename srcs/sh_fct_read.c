#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int				tree_traversal_verif(t_node *tree)
{
	if (DEBUG2 == 1)
		printf("------- TREE TRAVERSAL VERIF ------\n");
	printf("tree : %s - %d\n", tree->data, tree->type);
	if (tree && tree->left != NULL)
	{
		printf("{{%s left %s}}\n", tree->data, tree->left->data);
		tree_traversal_verif(tree->left);
	}
	if (tree && tree->right != NULL)
	{
		printf("{{%s right %s}}\n", tree->data, tree->right->data);
		tree_traversal_verif(tree->right);
	}
	//	ft_putstr("[");
	//	ft_putstr(tree->data);
	//	ft_putstr("]");
	//	ft_putstr(" --> ");
	return (0);
}

t_node          *read_n_check(char *special, char *read_buff, char **env)
{
	if (DEBUG == 1)
		printf("------- READ N CHECK ------\n");
	t_e_list		*l_expr;
	t_node			*tree;

	(void)special;
	(void)env;
	l_expr = NULL;
	lexer_1(read_buff, &l_expr);
	//	t_e_list *tmp = l_expr;
	lexer_2(&l_expr);
	//	tmp = l_expr;
	printf("avant parser\n");
	tree = parser(&l_expr);
	printf("apres parser\n");
	if (DEBUG2 == 1)
	{
		tree_traversal_verif(tree);
		printf("\napres tree traversal verif\n");
	}
	//	tbl = lst_to_tbl(arg);
	//	free_lst(&arg);
	//	return (tbl);
	return (tree);
}

int				check_home(char **cmd)
{
	if (DEBUG == 1)
		printf("------- CHECK HOME ------\n");
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
	if (DEBUG == 1)
		printf("------- CHECK AFTER READ ------\n");
	t_node          *tree;
	char            **env;
	int				i;

	i = 0;
	if ((tree = read_n_check(SEP, stline->line, NULL)) == NULL)
		return (-1);

	env = duo_to_tbl(*env_cpy, "=");
	tree_traversal(tree, env);
	/*
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
	   free_tab(&cmd);*/
	return (0);
}

int				fct_read(t_line *stline, t_duo **env_cpy, t_history **history)
{
	if (DEBUG == 1)
		printf("------- FCT READ ------\n");
	int				key;
	int				ret;

	ret = 0;
	stline->curs_x = 3;
	(void)history;
	while ((ret = read(0, &key, 7)) > 0)
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
