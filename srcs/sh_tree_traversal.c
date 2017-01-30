#include <fcntl.h>
#include "shell.h"
#include "libft.h"

int					fd_open(t_node *tree, t_lst_fd **lstfd, types type)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- FD OPEN -------", 2);

	int					fd;
	int					flags;
	char 				*filename;

	filename = NULL;
	flags = (type == LRED ? O_RDONLY : 0);
	flags = (type == RRED ?  O_WRONLY | O_TRUNC | O_CREAT : flags);
	flags = (type == DRRED ?  O_WRONLY | O_APPEND | O_CREAT : flags);
	if (tree && tree->data)
		if ((filename = ft_strdup(tree->data)) == NULL)
			return (ERROR);
			/* MSG ret: ERROR exit: TRUE msg: "malloc fail" */
			/* free : tree + lstfd */
	if (tree && tree->data && tree->data[0] == '&')
		fd = (ft_strcmp("&-", tree->data) == TRUE ? -42 : ft_atoi(filename + 1));
	else
	{
		if (type == LRED && access(filename, F_OK) == ERROR)
			return (ERROR);
		if ((fd = open(filename, flags,	S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == ERROR)
			fd = -1;
	}
	lstfd_pushbck(lstfd, fd, filename);
	if (fd == -1)
		return (ERROR);
		/* MSG ret: ERROR exit: FALSE msg: "filename + permission denied" */
	free(filename);
	return (TRUE);
}

int					manage_red_fd(t_node *tree, t_lst_fd **lstfd, types type)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- MANAGE RED FD -------", 2);

	int					ret;

	ret = TRUE;
	if (tree && (tree->type == RRED || tree->type == DRRED || tree->type == LRED || tree->type == DLRED))
		type = tree->type;
	if (tree && tree->type == RED_ARG && type != DLRED)
		if ((ret = fd_open(tree, lstfd, type)) != TRUE)
			return (ret);
	if (tree && tree->left)
		if (manage_red_fd(tree->left, lstfd, type) == ERROR)
			return (ERROR);
	if (tree && tree->right)
		if ((ret = manage_red_fd(tree->right, lstfd, type)) != TRUE)
			return (ret);
	return (TRUE);
}

// créer 3 fct tree_travers_semi tree_travers_pipe tree_travers_cmd
int					tree_traversal(t_node *tree, t_lst_fd **lstfd, int pipefd_tab[2][2])
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- TREE TRAVERSAL -------", 2);

	int					ret;

	ret = 0;

	savior_tty(ttyname(0), TRUE);

	if (tree && tree->type == SEMI)
	{
		if (tree && tree->left)
			if ((tree_traversal(tree->left, lstfd, pipefd_tab)) == ERROR)
				return (ERROR);
		init_pipefd(pipefd_tab);
		if (tree && tree->right)
			if ((tree_traversal(tree->right, lstfd, pipefd_tab)) == ERROR)
				return (ERROR);
	}

	if (tree && tree->type != SEMI && lstfd && *lstfd == NULL)
		manage_red_fd(tree, lstfd, NONE);

	if (tree->type == PIPE)
	{

	//ANTIBUG
		if (DEBUG_ANTIBUG == 1)
		{
			t_lst_fd *tmp = *lstfd;
				while(tmp){
					printf("in pipe [filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);
					tmp=tmp->next;
				}
				printf("next\n");
				tmp = tmp->next;
			}
	//  fin ANTIBUG

		if ((ret = (pipe_function(pipefd_tab, tree, lstfd))) != TRUE)
				return (ret);
		if ((ret = (tree_traversal(tree->right, lstfd, pipefd_tab))) != TRUE)
				return (ret);
		reset_std_fd();
		close_lstfd(lstfd);
	}

	if (tree->type == CMD)// || (tree->type >= RRED && tree->type <= DLRED))
	{
//		if (tree->type == CMD)
			if ((manage_cmd(pipefd_tab, tree, lstfd)) == ERROR)
				return (ERROR);
		if (tree->left != NULL && (*lstfd))
			(*lstfd) = (*lstfd)->next;
		if (pipefd_tab[0][0] < 0 && pipefd_tab[1][0] < 0)
		{
			reset_std_fd();
			close_lstfd(lstfd);
		}
	}
	return (TRUE);
}
