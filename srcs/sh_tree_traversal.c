#include <fcntl.h>
#include "shell.h"
#include "libft.h"

t_global_fd			*new_globalfd(void)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- NEW GLOBALFD -------", 2);

	t_global_fd			*new;

	new = NULL;
	new = (t_global_fd *)malloc(sizeof(t_global_fd));
	new->next = NULL;
	new->lstfd = NULL;
	return (new);
}

int					pushfront_globalfd(t_global_fd **globalfd)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- PUSHFRONT GLOBALFD -------", 2);

	t_global_fd			*new;

	new = NULL;
	if (globalfd == NULL)
		return (ERROR);
	if (*globalfd == NULL)
	{
		if ((*globalfd = new_globalfd()) == NULL)
			return (ERROR);
	}
	else
	{
		if ((new = new_globalfd()) == NULL)
			return (ERROR);
		new->next = *globalfd;
		*globalfd = new;
	}
	return (TRUE);
}

int 				check_red_arg2(t_node *tree, t_global_fd **globalfd, types type)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- CHECK FD ARG -------", 2);

	int					fd;
	int					flags;
	char 				*filename;

	if (type == RRED)
		flags = O_WRONLY | O_TRUNC | O_CREAT;
	else
		flags = O_WRONLY | O_APPEND | O_CREAT;
	if ((filename = ft_strdup(tree->data)) == NULL)
		return(ERROR);
	//filename = ft_strdup("/home/kimera/42_c/21sh/Makefile");
	if (type == LRED && access(filename, F_OK) != ERROR && access(filename, W_OK) == ERROR)
		return (ERROR);
	if ((fd = open(filename, flags,	S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == ERROR)
		return (ERROR);
	lstfd_pushbck(&((*globalfd)->lstfd), fd, filename);
	return (TRUE);
}

int 				check_fd_red(t_node *tree, t_global_fd **globalfd) //, types type)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- CHECK FD RED -------", 2);

	int					fd;
	char 				*filename;

	if ((filename = ft_strdup(tree->data)) == NULL)
		return (ERROR);
	if (ft_strcmp("&-", tree->data) == TRUE)
		fd = -42; //code pour signifier qu'il faut fermer le red_fd
	else
		fd = ft_atoi(filename + 1);
	lstfd_pushbck(&((*globalfd)->lstfd), fd, filename);
	return (TRUE);
}

int					fd_open(t_node *tree, t_global_fd **globalfd, types type)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- FD OPEN -------", 2);

	if (*globalfd == NULL)
		if ((*globalfd = new_globalfd()) == NULL)
			return (ERROR);
	if (tree->data && tree->data[0] == '&')
		check_fd_red(tree, globalfd); //, type);
	else
		check_red_arg2(tree, globalfd, type);
	return (TRUE);
}

int					manage_red_fd(t_node *tree, t_global_fd **globalfd, types type)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- MANAGE RED FD -------", 2);

	if (tree && (tree->type == RRED || tree->type == DRRED || tree->type == LRED || tree->type == DLRED))
		type = tree->type;
	if (tree->type == RED_ARG && type != DLRED)
		fd_open(tree, globalfd, type);
	if (tree && tree->right)
		manage_red_fd(tree->right, globalfd, type);
	if (tree->type == PIPE)
		pushfront_globalfd(globalfd);
	if (tree && tree->left)
		manage_red_fd(tree->left, globalfd, NONE);
	return (TRUE);
}

// créer 3 fct tree_travers_semi tree_travers_pipe tree_travers_cmd
int					tree_traversal(t_node *tree, t_global_fd **globalfd, int pipefd_tab[2][2])
{
	if (DEBUG_TREE == 0)
		ft_putendl_fd("------- TREE TRAVERSAL -------", 2);


	savior_tty(ttyname(0), TRUE, TRUE);
	savior_tty(ttyname(1), TRUE, FALSE);

	if (tree->type == SEMI)
	{
		if (tree->left)
			if ((tree_traversal(tree->left, globalfd, pipefd_tab)) == ERROR)
				return (ERROR);
		if (tree->right)
			if ((tree_traversal(tree->right, globalfd, pipefd_tab)) == ERROR)
				return (ERROR);
	}

	if (*globalfd == NULL)
		manage_red_fd(tree, globalfd, NONE);

	if (tree->type == PIPE)
	{

//	//ANTIBUG
//	t_global_fd *tmpglo = *globalfd;
//	t_lst_fd *tmp = NULL;
//	while (tmpglo)
//	{
//		tmp = tmpglo->lstfd;
//		while(tmp){
//			printf("in pipe [filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);
//			tmp=tmp->next;
//		}
//		printf("next\n");
//		tmpglo = tmpglo->next;
//	}
//		if ((manage_red_file(lstfd, tmpfd, tree)) == ERROR)
//			return (ERROR);
//		saved_lstfd = *lstfd;

		if ((pipe_function(pipefd_tab, tree, globalfd)) == ERROR)
				return (ERROR);
		if ((tree_traversal(tree->right, globalfd, pipefd_tab)) == ERROR)
				return (ERROR);
		reset_std_fd();
	}

	if (tree->type == CMD || (tree->type >= RRED && tree->type <= DLRED))
	{
		if (tree->type == CMD)
			if ((manage_cmd(pipefd_tab, tree, globalfd)) == ERROR)
				return (ERROR);
		if (tree->left != NULL && (*globalfd))
			(*globalfd) = (*globalfd)->next;
		if (pipefd_tab[0][0] < 0 && pipefd_tab[1][0] < 0)
		{
			reset_std_fd();
		}
	}
	return (TRUE);
}
