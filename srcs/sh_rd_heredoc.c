#include "libft.h"
#include "shell.h"

static int			tree_trav_hrd(t_line *stline, t_node **tree,
					t_history **history)
{
	if (DEBUG_HEREDOC == 1)
		printf("------------ TREE TRAV HRD ----------\n");

	if (*tree && (*tree)->left)
	{
		printf("**tree_trav_hrd left**\n");
		if (heredoc_handler(stline, &((*tree)->left), history) == ERROR)
			return (ERROR);
	}
	if (*tree && (*tree)->right)
	{
		printf("**tree_trav_hrd right**\n");
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

	if (stline->hrd.nb <= 0)
		return (FALSE);
	if ((*tree)->type == DLRED)
	{
		stline->hrd.deli = ((*tree)->right->type == RED_ARG ? (*tree)->right
													: (*tree)->right->right);
		//printf("((%d))\n", stline->hrd.nb); // ANTIBUG!!!!!!!!!!
		stline->hrd.ptr = stline->hrd.line;
		mini_prt_handler(&(stline->hrd.line), &(stline->hrd.pos), stline);
		if (fct_read(TRUE, stline, history) == ERROR ||
		(stline->hrd.deli->right = create_node(DLRED_DOC)) == NULL ||
		(stline->hrd.deli->right->data = ft_strsub(stline->hrd.line, 0,
		(ft_strlen(stline->hrd.line) - (ft_strlen(stline->hrd.deli->data) + 1)))) == NULL)
			return (ERROR);

		//ANTIBUG
		if (stline->hrd.line)
			printf("\nline heredoc :[%s]\ntree (%s)\n", stline->hrd.line, stline->hrd.deli->right->data);
		ft_bzero(stline->hrd.line, ft_strlen(stline->hrd.line));
		stline->hrd.pos = 0;
		(stline->hrd.nb)--;
	}
	printf("YOPYOPYOP\n");
	if ((tree_trav_hrd(stline, tree, history)) == ERROR)
		return (ERROR);
	return (TRUE);
}

/*
   int					fill_heredoc(t_line *stline)
   {
   if (DEBUG_HEREDOC == 1)
   printf("------------ FILL HEREDOC ----------\n");
//
//	if (stline->pos > 1 && (stline->hrd.ptr) != NULL)
//		printf("adresse : %p, line : %c, nb : %d, ptr : [%c], [%s]\n", stline->hrd.ptr, stline->line[stline->pos - 1], stline->hrd.nb, *(stline->hrd.ptr), ft_strchr(IGN, stline->line[stline->pos - 1]));

char				*tmp;

tmp = NULL;
if (stline->line[stline->pos - 1] && stline->line[stline->pos - 2]
&& stline->line[stline->pos - 1] == '<'
&& stline->line[stline->pos - 2] == '<')
{
(stline->hrd.nb)++;
stline->hrd.ptr = &(stline->line[stline->pos - 1]);
}
else if (stline->hrd.ptr != NULL && *(stline->hrd.ptr) == '<'
&& ft_strchr(IGN, stline->line[stline->pos - 1]) == NULL)
{
stline->hrd.ptr = &(stline->line[stline->pos - 1]);
}
else if (stline->hrd.ptr != NULL && *(stline->hrd.ptr) != '<'
&& ft_strchr(SEP, stline->line[stline->pos - 1]) != NULL)
{
tmp = ft_strsub(stline->hrd.ptr, 0, ft_strlen(stline->hrd.ptr) - 1);
ft_lstadd(&(stline->hrd.deli), ft_lstnew(tmp));
ft_strdel(&tmp);
stline->hrd.ptr = NULL;
}
return (TRUE);
}

int			return_heredoc(t_line *stline)
{
if (DEBUG_HEREDOC == 1)
printf("------------ RETURN HEREDOC ----------\n");

if (stline->hrd.nb > 0)
stline->hrd.start = TRUE;
return (TRUE);
}
*/
