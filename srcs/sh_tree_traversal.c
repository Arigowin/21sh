#include <fcntl.h>
#include "shell.h"
#include "libft.h"

int					fd_open(int	*fd, t_node *tree, t_lst_fd **lstfd)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- FD OPEN -------", 2);

	(void)lstfd;

	static int			fd_save = 0;
	int					ret;
	int					flags;
	char 				*filename;
	t_types				type;
	t_node				*node;

	filename = NULL;
	node = NULL;
	flags = -1;
	ret = 0;
	fd_save = (*fd == -2 ? 0 : fd_save);
	if (fd_save == -1)
	{
		if (*fd >= 0)
			close(*fd);
		*fd = -1;
	}
	fd_save = *fd;
	if ((type = tree->type) == DLRED)
		return (FALSE);
	if (tree && tree->right)
		node = (tree->right->type == RED_ARG ? tree->right : tree->right->right);
	flags = (type == RRED ?  O_WRONLY | O_TRUNC | O_CREAT : flags);
	flags = (type == DRRED ?  O_WRONLY | O_APPEND | O_CREAT : flags);
	flags = (type == LRED ? O_RDONLY : flags);
	if (*fd == -1)
		return (FALSE);
	if (node && node->data && (filename = node->data) == NULL)
		/* free : node + lstfd */
		return (ERROR);
	if (node && node->data && node->data[0] == '&')
		*fd = (ft_strcmp("&-", node->data) == 0 ? -42 : ft_atoi(filename + 1));
	else
	{
		if (type == LRED)
		   ret = access(filename, F_OK);
		*fd = ret;
		if (ret >= 0)
		{
			fd_save = open(filename, flags,	S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			*fd = fd_save;
		}
	}
	if (*fd == -1)
	{
		if (ret <= 0)
			return (sh_error(TRUE, 21, filename, NULL));
		else
			return (sh_error(TRUE, 20, filename, NULL));
	}
	return (TRUE);
}

int 				push_in_lstfd(t_node *tree, t_lst_fd **lstfd, int fd, int *fd_save)
{
	char				*filename;

	if (*fd_save == -1)
	{
		if (fd >= 0)
			close(fd);
		fd = -1;
	}
	*fd_save = fd;
	if (tree && (tree->type == RRED || tree->type == DRRED || tree->type == LRED) && tree->right)
	{
		filename = (tree->right->type == RED_ARG ? tree->right->data : tree->right->right->data);
		lstfd_pushfront(lstfd, fd, filename);
		if (fd == -1)
			return (FALSE);
	}
	return (TRUE);
}

int					manage_red_fd(int fd, t_node *tree, t_lst_fd **lstfd, t_types type)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- MANAGE RED FD -------", 2);

	int					ret;
	static int			fd_save = 0;
	t_lst_fd			*pipe_fd;
	t_lst_fd			*tmp;

	pipe_fd = NULL;
	fd_save = (fd == -21 ? 0 : fd_save);
	if (tree && tree->type == PIPE)
	{
		tmp = *lstfd;
		while (pipe_fd && tmp && tmp != pipe_fd)
			tmp = tmp->next;
		pipe_fd = *lstfd;
		fd = -21;
	}
	if (tree && (tree->type == RRED || tree->type == DRRED || tree->type == LRED || tree->type == DLRED))
		if ((ret = fd_open(&fd, tree, lstfd)) == ERROR)
			return (ret);
	if (tree && tree->right && tree->type == PIPE)
		if ((ret = manage_red_fd(fd, tree->right, lstfd, type)) ==  ERROR)
			fd = -1;
	if (tree && tree->left)
		if ((ret = manage_red_fd(fd, tree->left, lstfd, type)) == ERROR)
			fd = -1;
	return (push_in_lstfd(tree, lstfd, fd, &fd_save));
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
	savior_tty(ttyname(1), TRUE);

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
		manage_red_fd(-2, tree, lstfd, NONE);

		//ANTIBUG
		if (DEBUG_ANTIBUG == 1)
		{
			printf("lstfd :\n");
			t_lst_fd *tmp = *lstfd;
			while(tmp){
				printf("in pipe [filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);
				tmp=tmp->next;
			}
			//tmp = tmp->next;
		}
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
