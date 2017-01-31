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

	if (tree->type == PIPE)
	{

		////ANTIBUG
		//if (DEBUG_ANTIBUG == 1)
		//{
		//	t_lst_fd *tmp = *lstfd;
		//	while(tmp){
		//		printf("in pipe [filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);
		//		tmp=tmp->next;
		//	}
		//	printf("next\n");
		//	tmp = tmp->next;
		//}
		////  fin ANTIBUG

		if ((ret = (pipe_function(pipefd_tab, tree, lstfd))) != TRUE)
			return (ret);
		if ((ret = (tree_traversal(tree->right, lstfd, pipefd_tab))) != TRUE)
			return (ret);
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


//int					fd_open(t_node *tree, t_lst_fd **lstfd, types type)
//{
//	if (DEBUG_TREE == 1)
//<<<<<<< HEAD
//		ft_putendl_fd("------- FD OPEN -------", 2);
//=======
//		ft_putendl_fd("------- NEW GLOBALFD -------", 2);
//
//	t_global_fd			*new;
//
//	new = NULL;
//	if ((new = (t_global_fd *)malloc(sizeof(t_global_fd))) == NULL)
//		return (NULL);
//	/* MSG ret: NULL exit: TRUE msg: "malloc fail" */
//	new->lstfd = NULL;
//	new->next = NULL;
//	return (new);
//}
//
//int					pushfront_globalfd(t_global_fd **globalfd)
//{
//	if (DEBUG_TREE == 1)
//		ft_putendl_fd("------- PUSHFRONT GLOBALFD -------", 2);
//>>>>>>> david
//
//	int					fd;
//	int					flags;
//	char 				*filename;
//
//	filename = NULL;
//	flags = (type == LRED ? O_RDONLY : 0);
//	flags = (type == RRED ?  O_WRONLY | O_TRUNC | O_CREAT : flags);
//	flags = (type == DRRED ?  O_WRONLY | O_APPEND | O_CREAT : flags);
//	if (tree && tree->data)
//		if ((filename = ft_strdup(tree->data)) == NULL)
//			return (ERROR);
//			/* MSG ret: ERROR exit: TRUE msg: "malloc fail" */
//			/* free : tree + lstfd */
//	if (tree && tree->data && tree->data[0] == '&')
//		fd = (ft_strcmp("&-", tree->data) == TRUE ? -42 : ft_atoi(filename + 1));
//	else
//	{
//		if (type == LRED && access(filename, F_OK) == ERROR)
//			return (ERROR);
//		if ((fd = open(filename, flags,	S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == ERROR)
//			fd = -1;
//	}
//<<<<<<< HEAD
//	lstfd_pushbck(lstfd, fd, filename);
//	if (fd == -1)
//		return (ERROR);
//		/* MSG ret: ERROR exit: FALSE msg: "filename + permission denied" */
//	free(filename);
//=======
//	return (TRUE);
//}
//
//int 				check_red_arg2(t_node *tree, t_global_fd **globalfd, types type)
//{
//	if (DEBUG_TREE == 1)
//		ft_putendl_fd("------- CHECK FD ARG -------", 2);
//
//	int					fd;
//	int					flags;
//	char 				*filename;
//
//	if (type == RRED)
//		flags = O_WRONLY | O_TRUNC | O_CREAT;
//	else if (type == DRRED)
//		flags = O_WRONLY | O_APPEND | O_CREAT;
//	else if (type == LRED)
//		flags = O_RDONLY;
//	if ((filename = ft_strdup(tree->data)) == NULL)
//		return(ERROR);
//	/* MSG ret: ERROR exit: TRUE msg: "malloc fail" */
//	/* free : tree + globalfd */
//	if (type == LRED && access(filename, F_OK) == ERROR)
//		return (ERROR);
//	/* MSG ret: ERROR exit: FALSE msg: "cannot access + filename + not such file or directory " */
//	if ((fd = open(filename, flags,	S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == ERROR)
//	{
//		lstfd_pushbck(&((*globalfd)->lstfd), -1, filename);
//		return (ERROR);
//		/* MSG ret: ERROR exit: FALSE msg: "filename + permission denied" */
//	}
//	lstfd_pushbck(&((*globalfd)->lstfd), fd, filename);
//	return (TRUE);
//}
//
//int 				check_fd_red(t_node *tree, t_global_fd **globalfd) //, types type)
//{
//	if (DEBUG_TREE == 1)
//		ft_putendl_fd("------- CHECK FD RED -------", 2);
//
//	int					fd;
//	char 				*filename;
//
//	if ((filename = ft_strdup(tree->data)) == NULL)
//		return (ERROR);
//	/* MSG ret: ERROR exit: TRUE msg: "malloc fail" */
//	/* free : tree + globalfd */
//	if (ft_strcmp("&-", tree->data) == TRUE)
//		fd = -42;
//	else
//		fd = ft_atoi(filename + 1);
//	lstfd_pushbck(&((*globalfd)->lstfd), fd, filename);
//>>>>>>> david
//	return (TRUE);
//}
//
//int					manage_red_fd(t_node *tree, t_lst_fd **lstfd, types type)
//{
//	if (DEBUG_TREE == 1)
//		ft_putendl_fd("------- MANAGE RED FD -------", 2);
//
//	int					ret;
//
//	ret = TRUE;
////	printf(" TREE DANS MANAGED RED FD : %p\n", tree);
//	if (tree == NULL)
//		return (FALSE);
//	if (tree && (tree->type == RRED || tree->type == DRRED || tree->type == LRED || tree->type == DLRED))
//	{
//		type = tree->type;
//<<<<<<< HEAD
//	if (tree && tree->type == RED_ARG && type != DLRED)
//		if ((ret = fd_open(tree, lstfd, type)) != TRUE)
//			return (ret);
//	if (tree && tree->left)
//		if (manage_red_fd(tree->left, lstfd, type) == ERROR)
//			return (ERROR);
//	if (tree && tree->right)
//		if ((ret = manage_red_fd(tree->right, lstfd, type)) != TRUE)
//			return (ret);
//=======
//	}
//	if (tree && tree->type == RED_ARG && type != DLRED)
//	{
//		if ((ret = fd_open(tree, globalfd, type)) != TRUE)
//			return (ret);
//	}
//	if (tree && tree->right)
//	{
//		if ((ret = manage_red_fd(tree->right, globalfd, type)) == ERROR)
//			return (ERROR); // au lieu de ret
//	}
//	if (tree && tree->type == PIPE && globalfd != NULL && *globalfd != NULL && (*globalfd)->lstfd != NULL)
//	{
//		if ((ret = pushfront_globalfd(globalfd)) != TRUE)
//			return (ret);
//	}
//	if (ret != FALSE && tree && tree->left)
//	{
//		if ((ret = manage_red_fd(tree->left, globalfd, NONE)) != TRUE)
//			return (ret);
//	}
//	if (globalfd && *globalfd && (*globalfd)->lstfd == NULL)
//		*globalfd = (*globalfd)->next;
//>>>>>>> david
//	return (TRUE);
//}
//
//// créer 3 fct tree_travers_semi tree_travers_pipe tree_travers_cmd
//int					tree_traversal(t_node *tree, t_lst_fd **lstfd, int pipefd_tab[2][2])
//{
//	if (DEBUG_TREE == 1)
//		ft_putendl_fd("------- TREE TRAVERSAL -------", 2);
//
//	int					ret;
//
//	ret = 0;
//
//	savior_tty(ttyname(0), TRUE);
//
//	if (tree && tree->type == SEMI)
//	{
//<<<<<<< HEAD
//		if (tree && tree->left)
//			if ((tree_traversal(tree->left, lstfd, pipefd_tab)) == ERROR)
//=======
////		printf("XXXXXX SEMI\n");
//		if (tree->left)
//			if ((tree_traversal(tree->left, globalfd, pipefd_tab)) == ERROR)
//>>>>>>> david
//				return (ERROR);
//		init_pipefd(pipefd_tab);
//		if (tree && tree->right)
//			if ((tree_traversal(tree->right, lstfd, pipefd_tab)) == ERROR)
//				return (ERROR);
//	}
//
//<<<<<<< HEAD
//	if (tree && tree->type != SEMI && lstfd && *lstfd == NULL)
//		manage_red_fd(tree, lstfd, NONE);
//=======
//	if (*globalfd == NULL && tree)
//		manage_red_fd(tree, globalfd, NONE);
//>>>>>>> david
//
//	if (tree->type == PIPE)
//	{
////		printf("XXXXXX PIPE\n");
//
//<<<<<<< HEAD
//	//ANTIBUG
//		if (DEBUG_ANTIBUG == 1)
//		{
//			t_lst_fd *tmp = *lstfd;
//=======
//		//ANTIBUG
//		if (DEBUG_ANTIBUG == 1)
//		{
//			t_global_fd *tmpglo = *globalfd;
//			t_lst_fd *tmp = NULL;
//			while (tmpglo)
//			{
//				tmp = tmpglo->lstfd;
//>>>>>>> david
//				while(tmp){
//					printf("in pipe [filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);
//					tmp=tmp->next;
//				}
//				printf("next\n");
//<<<<<<< HEAD
//				tmp = tmp->next;
//			}
//	//  fin ANTIBUG
//
//		if ((ret = (pipe_function(pipefd_tab, tree, lstfd))) != TRUE)
//				return (ret);
//		if ((ret = (tree_traversal(tree->right, lstfd, pipefd_tab))) != TRUE)
//				return (ret);
//=======
//				tmpglo = tmpglo->next;
//			}
//			//  fin ANTIBUG
//		}
//		if ((ret = (pipe_function(pipefd_tab, tree, globalfd))) != TRUE)
//			return (ret);
//		if ((ret = (tree_traversal(tree->right, globalfd, pipefd_tab))) != TRUE)
//			return (ret);
//>>>>>>> david
//		reset_std_fd();
//		close_lstfd(lstfd);
//	}
//
//	if (tree->type == CMD)// || (tree->type >= RRED && tree->type <= DLRED))
//	{
//<<<<<<< HEAD
////		if (tree->type == CMD)
//			if ((manage_cmd(pipefd_tab, tree, lstfd)) == ERROR)
//=======
//		// printf("XXXXXX CMD \n");
//		if (tree->type == CMD)
//			if ((manage_cmd(pipefd_tab, tree, globalfd)) == ERROR)
//>>>>>>> david
//				return (ERROR);
//		if (tree->left != NULL && (*lstfd))
//			(*lstfd) = (*lstfd)->next;
//		if (pipefd_tab[0][0] < 0 && pipefd_tab[1][0] < 0)
//		{
//			reset_std_fd();
//			close_lstfd(lstfd);
//		}
//	}
//	return (TRUE);
//}
