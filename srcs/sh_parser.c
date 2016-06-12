#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int				parse_error(char *data)
{
	ft_putstr("21sh: parse error near \"");
	ft_putstr(data);
	ft_putendl("\"");
	return (ERROR);
}

int				check_red(t_e_list *l_expr, t_node **tree)
{
	t_node			*current_cmd;
	t_node			*tmp;

	current_cmd = *node;
	tmp = *node;
	//if (l_expr->type == RED)
	//	create_node() av red
	//if (l_expr->next != RED_ARG)
	//	return (parse_error());
	//create_node() ac RED_ARG
	//return (TRUE);
}

int				check_cmd(t_e_list *l_expr, t_node **tree)
{
	t_node			*current_cmd;
	t_node			*tmp;

	current_cmd = *node;
	tmp = *node;
	if (l_expr->type == CMD)
	{
		return (TRUE);
	}
	return (FALSE);
}

int				check_command(t_e_list *l_expr, t_node **tree)
{
	t_node			*node;

	node = NULL;
	if ((node = create_node(NULL, CMD)) == NULL)
		return (FALSE);
	check_red(l_expr, node);
	if (check_cmd(l_expr, node) == TRUE && list_next(l_expr))
	{
		// create_node() pr command
		// create_node() pr cmd_arg
		return (TRUE);
	}
	return (FALSE);
}

int				check_c_pipe(t_e_list *l_expr, t_node **tree)
{
	t_node			*node;

	node = NULL;
	if ((node = create_node(NULL, PIPE)) == NULL)
		return (FALSE);
	if (check_command(l_expr, node))
	{
		tree = node;
		return (TRUE);
	}
	return (FALSE);
}

int				check_expr(t_e_list *l_expr, t_node **tree)
{
	t_node			*node;

	node = NULL;
	if ((node = create_node(NULL, EXPR)) != NULL
			&& check_c_pipe(l_expr, &node->left) == TRUE)
	{
		*tree = node;
		return (TRUE);
	}
	return (FALSE);
}

t_node			*parser(t_e_list **l_expr)
{
	t_node			*node;

	if (l_expr == NULL)
		return (NULL);
	node = NULL;
	if ((check_expr(*l_expr, &node)) == FALSE)
	{
		//clear_tree()
	}
		return (node);
}
