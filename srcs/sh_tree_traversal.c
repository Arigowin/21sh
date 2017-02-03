#include <fcntl.h>
#include "shell.h"
#include "libft.h"

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
	flags = -1;
	flags = (type == RRED ?  O_RDWR | O_TRUNC | O_CREAT : flags);
	flags = (type == DRRED ?  O_RDWR | O_APPEND | O_CREAT : flags);
	flags = (type == LRED ? O_RDWR : flags);
	if (tree && tree->data)
		if ((filename = ft_strdup(tree->data)) == NULL)
			return (ERROR);
	/* MSG ret: ERROR exit: TRUE msg: "malloc fail" */
	/* free : tree + lstfd */
	if (tree && tree->data && tree->data[0] == '&')
		fd = (ft_strcmp("&-", tree->data) == 0 ? -42 : ft_atoi(filename + 1));
	else
	{
		if (type == LRED && access(filename, F_OK) == ERROR)
		{
			ft_strdel(&filename);
			return (ERROR);
		}
		fd = open(filename, flags,	S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}
	lstfd_pushbck(lstfd, fd, filename);
	ft_strdel(&filename);
	if (fd == -1)
		return (ERROR);
	/* MSG ret: ERROR exit: FALSE msg: "filename + permission denied" */
	return (TRUE);
}

int					file_exist(t_node *tree)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- FILE EXIST -------", 2);

	if (tree && tree->type == LRED)
	{
		if (tree->right && access(tree->right->data, F_OK) == ERROR)
		{
			ft_putstr_fd("21sh LRED : ", 2);
			ft_putstr_fd(tree->right->data, 2);
			ft_putendl_fd(": no such file or directory", 2);
			return (ERROR);
		}
	}
	if (tree && tree->right)
	{
		if (file_exist(tree->right) == ERROR)
			return (ERROR);
	}
	if (tree && tree->left)
	{
		if (file_exist(tree->left) == ERROR)
			return (ERROR);
	}
	return (TRUE);
}

int					manage_red_fd(t_node *tree, t_lst_fd **lstfd, types type)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- MANAGE RED FD -------", 2);

	int					ret;

	ret = TRUE;
	file_exist(tree);
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

//	créer 3 fct tree_travers_semi tree_travers_pipe tree_travers_cmd
int					tree_traversal(t_node *tree, t_lst_fd **lstfd, int pipefd_tab[2][2])
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- TREE TRAVERSAL -------", 2);

	int					ret;
	t_lst_fd			*tmpfd;

	ret = 0;
	tmpfd = NULL;
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

		//ANTIBUG
//		if (DEBUG_ANTIBUG == 0)
//		{
//			t_lst_fd *tmp = *lstfd;
//			while(tmp){
//				printf("in pipe [filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);
//				tmp=tmp->next;
//			}
//			printf("next\n");
//			//tmp = tmp->next;
//		}
		//  fin ANTIBUG

	if (tree->type == PIPE)
	{
		if ((ret = (pipe_function(pipefd_tab, tree, lstfd))) != TRUE)
			return (ret);
		if ((ret = (tree_traversal(tree->right, lstfd, pipefd_tab))) != TRUE)
			return (ret);
		if (pipefd_tab[0][0] != -2)
			close (pipefd_tab[0][0]);
		if (pipefd_tab[0][1] != -2)
			close (pipefd_tab[0][1]);
		if (pipefd_tab[1][0] != -2)
			close (pipefd_tab[1][0]);
		if (pipefd_tab[1][1] != -2)
			close (pipefd_tab[1][1]);
		reset_std_fd();
		close_lstfd(lstfd);
		del_lstfd(lstfd);
	}

	if (tree->type == CMD)// || (tree->type >= RRED && tree->type <= DLRED))
	{
		if ((manage_cmd(pipefd_tab, tree, lstfd)) == ERROR)
			return (ERROR);
		if (tree->left != NULL && (*lstfd))
		{
			tmpfd = (*lstfd)->next;
			if ((*lstfd)->fd > 2)
				close((*lstfd)->fd);
			ft_strdel(&((*lstfd)->filename));
			free(*lstfd);
			*lstfd = tmpfd;
		}
		if (pipefd_tab[0][0] < 0 && pipefd_tab[1][0] < 0)
		{
			reset_std_fd();
			close_lstfd(lstfd);
			del_lstfd(lstfd);
		}
	}
	return (TRUE);
}
