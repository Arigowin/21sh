#include "shell.h"
#include "libft.h"

static int		red_travesal(t_lst_fd **lstfd, t_lst_fd **tmpfd, t_node **tree)
{
//	if (DEBUG_RED == 1)
		ft_putendl_fd("------- RED TRAVERSAL -------", 2);

	if (tree && *tree && (*tree)->right && ((*tree)->right->type == CMD || (*tree)->right->type == PIPE))
	{
		if ((manage_red_file(lstfd, tmpfd, (*tree)->right)) == ERROR)
			return (ERROR);
	}
	if (tree && *tree && (*tree)->left && (*tree)->left->type == CMD)
	{
		if ((manage_red_file(lstfd, tmpfd, (*tree)->left)) == ERROR)
			return (ERROR);
	}
	return (TRUE);
}

int					manage_red_file(t_lst_fd **lstfd, t_lst_fd **tmpfd, t_node *tree)
{
//	if (DEBUG_RED == 1)
		ft_putendl_fd("------- MANAGE RED FILE -------", 2);

	t_node				*red_arg;
	t_node				*tmp;
	int					ret;
	int					insert;

	tmp = tree->left;
	red_arg = NULL;
	ret = -1;
	if (tree && tree->right && tree->right->type != CMD_ARG)
		manage_red_file(lstfd, tmpfd, tree->right);
	if (tmp != NULL && (tmp->type == RRED || tmp->type == DRRED
				|| tmp->type == LRED))
	{
		if (!tmp->right)
			return (FALSE);
		red_arg = (tmp->right->type == RED_ARG ? tmp->right
				: tmp->right->right);
		insert = (tree->type == CMD ? FALSE : TRUE);
		if (tmp->type == RRED || tmp->type == DRRED)
		{
			if ((ret = right_red_fd(lstfd, tmpfd, tmp, red_arg, insert)) == ERROR)
				return (ERROR);
			if (ret == SYS_CALL_FAIL)
				return (SYS_CALL_FAIL);
		}
		if (tmp->type == LRED)
		{
			if ((ret = left_red_fd(lstfd, tmpfd, red_arg, insert)) == ERROR)
				return (ERROR);
			if (ret == SYS_CALL_FAIL)
				return (SYS_CALL_FAIL);
		}
		if (tmp && (ret = manage_red_file(lstfd, tmpfd, tmp)) != TRUE)
			return (ret);
	}
	if (red_travesal(lstfd, tmpfd, &tree) == ERROR)
		return (ERROR);
	return (TRUE);
}