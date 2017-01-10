#include "shell.h"
#include "libft.h"

static int		red_travesal(t_lst_fd **lstfd, t_node **tree)
{
	// printf ("data : (%s-%d)\n", (*tree)->data, (*tree)->type);
	if (*tree && (*tree)->left)
	{
		if ((manage_red_file(lstfd, (*tree)->left)) == ERROR)
			return (ERROR);
	}
	if (*tree && (*tree)->right)
	{
		if ((manage_red_file(lstfd, (*tree)->right)) == ERROR)
			return (ERROR);
	}
	return (TRUE);
}

int					manage_red_file(t_lst_fd **lstfd, t_node *tree)
{
	if (DEBUG_RED == 1)
		printf("------- MANAGE RED FILE -------\n");

	t_node				*red_arg;

	red_arg = NULL;
	//printf("data: (%s)\n", tree->data);
	if (!tree || (tree->type == RRED || tree->type == DRRED
				|| tree->type == LRED || tree->type == DLRED))
	{
		if (!tree->right)
			return (FALSE);
		red_arg = (tree->right->type == RED_ARG ? tree->right
				: tree->right->right);
		if (tree->type == RRED || tree->type == DRRED)
			right_red_fd(lstfd, tree, red_arg);
		if (tree->type == LRED)
			left_red_fd(lstfd, red_arg);
		if (tree->type == DLRED)
			;
	}
	if (red_travesal(lstfd, &tree) == ERROR)
		return (ERROR);
	return (TRUE);
}
