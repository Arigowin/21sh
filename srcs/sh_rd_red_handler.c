#include "shell.h"
#include "libft.h"

static int		red_travesal(t_lst_fd **lstfd, t_node **tree)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- RED TRAVERSAL -------", 2);

	dprintf (2, "data : (%s-%d)\n", (*tree)->data, (*tree)->type);
	if (*tree && (*tree)->right)
	{
		if ((manage_red_file(lstfd, (*tree)->right)) == ERROR)
			return (ERROR);
	}
	if (*tree && (*tree)->left)
	{
		if ((manage_red_file(lstfd, (*tree)->left)) == ERROR)
			return (ERROR);
	}
	return (TRUE);
}

int					manage_red_file(t_lst_fd **lstfd, t_node *tree)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- MANAGE RED FILE -------", 2);

	t_node				*red_arg;
	int					ret;

	red_arg = NULL;
	ret = -1;
	//ft_putendl_fd("data: (%s)\n", tree->data);
	if (!tree || (tree->type == RRED || tree->type == DRRED
				|| tree->type == LRED))
	{
		if (!tree->right)
			return (FALSE);
		red_arg = (tree->right->type == RED_ARG ? tree->right
				: tree->right->right);
		if (tree->type == RRED || tree->type == DRRED)
		{
			if ((ret = right_red_fd(lstfd, tree, red_arg)) == ERROR)
				return (ERROR);
			if (ret == SYS_CALL_FAIL)
				return (SYS_CALL_FAIL);
		}
		if (tree->type == LRED)
		{
			printf("left red\n");
			if ((ret = left_red_fd(lstfd, red_arg)) == ERROR)
			{printf("left red error\n");
				return (ERROR);
			}
//			if (ret == SYS_CALL_FAIL)
//				return (SYS_CALL_FAIL);
		}
	}
	if (red_travesal(lstfd, &tree) == ERROR)
		return (ERROR);

	//ANTIBUG
	t_lst_fd	*tmp = *lstfd;
	while (tmp)
	{
		printf("fd name (%s)->(%d)\n", tmp->filename, tmp->fd);
		tmp = tmp->next;
	}
	return (TRUE);
}
