#include "shell.h"
#include "libft.h"

static int			tree_trav_semi(t_node *tree, t_lst_fd **lstfd,
					int pipefd_tab[2][2]) //static ac tree traversal
{
	if (tree && tree->left)
		if ((tree_traversal(tree->left, lstfd, pipefd_tab)) == ERROR)
			return (ERROR);
	init_pipefd(pipefd_tab);
	if (tree && tree->right)
		if ((tree_traversal(tree->right, lstfd, pipefd_tab)) == ERROR)
			return (ERROR);
	return (TRUE);
}

static int			tree_trav_pipe(t_node *tree, t_lst_fd **lstfd,
					int pipefd_tab[2][2]) //static ac tree traversal
{
	int					ret;

	ret = TRUE;
	if ((ret = (pipe_function(pipefd_tab, tree, lstfd))) != TRUE)
		return (ret);
	if ((ret = (tree_traversal(tree->right, lstfd, pipefd_tab))) != TRUE)
		return (ret);
	if (pipefd_tab[0][0] != -2)
		close(pipefd_tab[0][0]);
	if (pipefd_tab[0][1] != -2)
		close(pipefd_tab[0][1]);
	if (pipefd_tab[1][0] != -2)
		close(pipefd_tab[1][0]);
	if (pipefd_tab[1][1] != -2)
		close(pipefd_tab[1][1]);
	reset_std_fd();
	close_lstfd(lstfd);
	del_lstfd(lstfd);
	return (ret);
}

static int			tree_trav_cmd(t_node *tree, t_lst_fd **lstfd) //static ac tree traversal
{
	t_lst_fd			*tmpfd;

	tmpfd = NULL;
	if (tree->left != NULL && (*lstfd))
	{
		tree = tree->left;
		while (tree)
		{
			if (tree->type != DLRED)
			{
				tmpfd = (*lstfd)->next;
				if ((*lstfd)->fd > 2)
					close((*lstfd)->fd);
				ft_strdel(&((*lstfd)->filename));
				free(*lstfd);
				*lstfd = tmpfd;
			}
			tree = tree->left;
		}
	}
	return (TRUE);
}

// sans les antibug norme OK
int					tree_traversal(t_node *tree, t_lst_fd **lstfd,
					int pipefd_tab[2][2])
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- TREE TRAVERSAL -------", 2);

	int					ret;

	ret = 0;
	savior_tty(ttyname(1), TRUE);
	if (tree && tree->type == SEMI)
		if (tree_trav_semi(tree, lstfd, pipefd_tab) == ERROR)
			return (ERROR);
	if (tree && tree->type != SEMI && lstfd && *lstfd == NULL)
		red_fd(-2, tree, lstfd, NONE);
	//ANTIBUG -11 lignes
	if (DEBUG_ANTIBUG == 1)
	{
		printf("lstfd :\n");
		t_lst_fd *tmp = *lstfd;
		while(tmp){
			printf("in pipe [filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);
			tmp=tmp->next;
		}
		printf("end lstfd :\n");
	}
	//  fin ANTIBUG
	if (tree->type == PIPE &&
	(ret == tree_trav_pipe(tree, lstfd, pipefd_tab)) != TRUE)
		return (ret);
	if (tree->type == CMD)
	{
		if ((manage_cmd(pipefd_tab, tree, lstfd)) == ERROR)
			return (ERROR);
		tree_trav_cmd(tree, lstfd);
		if (pipefd_tab[0][0] < 0 && pipefd_tab[1][0] < 0)
		{
			reset_std_fd();
			close_lstfd(lstfd);
			del_lstfd(lstfd);
		}
		//ANTIBUG
		if (DEBUG_ANTIBUG == 1)
		{
			printf("lstfd2 :\n");
			t_lst_fd *tmp = *lstfd;
			while(tmp){
				printf("[filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);
				tmp=tmp->next;
			}
		}
		//  fin ANTIBUG
	}
	return (TRUE);
}
