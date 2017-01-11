#include "libft.h"
#include "shell.h"

static int			tree_trav_hrd(t_line *stline, t_node **tree,
					t_history **history)
{
	if (DEBUG_HEREDOC == 1)
		printf("------------ TREE TRAV HRD ----------\n");

	if (*tree && (*tree)->left)
	{
		if (heredoc_handler(stline, &((*tree)->left), history) == ERROR)
			return (ERROR);
	}
	if (*tree && (*tree)->right)
	{
		if (heredoc_handler(stline, &((*tree)->right), history) == ERROR)
			return (ERROR);
	}
	return (TRUE);
}

int					check_end_heredoc(t_line *stline)
{
	if (DEBUG_HEREDOC == 1)
		printf("------------ CHECK END HEREDOC ----------\n");

	if (ft_strcmp(stline->hrd.deli->data, stline->hrd.ptr) == 0)
		return (BREAK);
	stline->hrd.ptr = &((stline->hrd.line)[stline->hrd.pos + 1]);
	return (CONTINUE);
}

int					heredoc_handler(t_line *stline, t_node **tree,
					t_history **history)
{
	if (DEBUG_HEREDOC == 1)
		printf("------------ HEREDOC HANDLER ----------\n");
	int 				len;

	len = 0;
	if (stline->hrd.nb <= 0)
		return (FALSE);
	if ((*tree)->type == DLRED)
	{
		stline->hrd.deli = ((*tree)->right->type == RED_ARG ? (*tree)->right
													: (*tree)->right->right);
		//printf("((%d))\n", stline->hrd.nb); // ANTIBUG!!!!!!!!!!
		stline->hrd.ptr = stline->hrd.line;
		mini_prt_handler(&(stline->hrd.line), &(stline->hrd.pos), stline);
		if (fct_read(TRUE, stline, history) == ERROR)
			return (ERROR);
		if ((stline->hrd.deli->right = create_node(DLRED_DOC)) == NULL)
			return (ERROR);
		len = (ft_strlen(stline->hrd.line) - (ft_strlen(stline->hrd.deli->data) + 1));
		if (tree && (*tree) && (*tree)->right && (*tree)->right->type == RED_ARG)
			(*tree)->right->right->data = ft_strsub(stline->hrd.line, 0, len);
		else
			(*tree)->right->right->right->data = ft_strsub(stline->hrd.line, 0, len);

		//ANTIBUG
		if (stline->hrd.line)
		ft_bzero(stline->hrd.line, ft_strlen(stline->hrd.line));
		stline->hrd.pos = 0;
		(stline->hrd.nb)--;
	}
	if ((tree_trav_hrd(stline, tree, history)) == ERROR)
		return (ERROR);
	return (TRUE);
}
