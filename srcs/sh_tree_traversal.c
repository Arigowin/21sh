#include <fcntl.h>
#include "shell.h"
#include "libft.h"

t_global_fd			*new_globalfd(void)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- NEW GLOBALFD -------", 2);

	t_global_fd			*new;

	new = NULL;
	if ((new = (t_global_fd *)malloc(sizeof(t_global_fd))) == NULL)
		return (NULL);
	/* MSG ret: NULL exit: TRUE msg: "malloc fail" */
	new->lstfd = NULL;
	new->next = NULL;
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
		if ((*globalfd = new_globalfd()) == NULL) // return useless
			return (ERROR);
	}
	else
	{
		if ((new = new_globalfd()) == NULL) // return useless
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
	else if (type == DRRED)
		flags = O_WRONLY | O_APPEND | O_CREAT;
	else if (type == LRED)
		flags = O_RDONLY;
	if ((filename = ft_strdup(tree->data)) == NULL)
		return(ERROR);
	/* MSG ret: ERROR exit: TRUE msg: "malloc fail" */
	/* free : tree + globalfd */
	if (type == LRED && access(filename, F_OK) == ERROR)
		return (ERROR);
	/* MSG ret: ERROR exit: FALSE msg: "cannot access + filename + not such file or directory " */
	if ((fd = open(filename, flags,	S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == ERROR)
	{
		lstfd_pushbck(&((*globalfd)->lstfd), -1, filename);
		return (ERROR);
		/* MSG ret: ERROR exit: FALSE msg: "filename + permission denied" */
	}
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
	/* MSG ret: ERROR exit: TRUE msg: "malloc fail" */
	/* free : tree + globalfd */
	if (ft_strcmp("&-", tree->data) == TRUE)
		fd = -42;
	else
		fd = ft_atoi(filename + 1);
	lstfd_pushbck(&((*globalfd)->lstfd), fd, filename);
	return (TRUE);
}

int					fd_open(t_node *tree, t_global_fd **globalfd, types type)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- FD OPEN -------", 2);

	int					ret;

	ret = FALSE;
	if (*globalfd == NULL)
	{
		if ((*globalfd = new_globalfd()) == NULL) // return useless
			return (ERROR);
	}
	if (tree->data && tree->data[0] == '&')
		ret = check_fd_red(tree, globalfd); //, type);
	else
		ret = check_red_arg2(tree, globalfd, type);
	return (ret);
}

int					manage_red_fd(t_node *tree, t_global_fd **globalfd, types type)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- MANAGE RED FD -------", 2);

	int					ret;

	ret = TRUE;
	ft_putendl("OOOOOOOOOOOOOOOOO 1");
	printf("0000 TREE : %p\n", tree);
	if (tree == NULL)
		return (FALSE);
	if (tree && (tree->type == RRED || tree->type == DRRED || tree->type == LRED || tree->type == DLRED))
	{
		ft_putendl("XXXXXXX CACA 1");
		type = tree->type;
	}
	ft_putendl("OOOOOOOOOOOOOOOOO 2");
	if (tree && tree->type == RED_ARG && type != DLRED)
	{
		ft_putendl("XXXXXXX CACA 2");
		if ((ret = fd_open(tree, globalfd, type)) != TRUE)
		{
			ft_putendl("XXXXXXX PROUT 2");
			return (ret);
		}
	}
	ft_putendl("OOOOOOOOOOOOOOOOO 3");
	if (tree && tree->right)
	{
		ft_putendl("XXXXXXX CACA 3");
		if ((ret = manage_red_fd(tree->right, globalfd, type)) == ERROR)
		{
			ft_putendl("XXXXXXX PROUT 3");
			return (ERROR); // au lieu de ret
		}
	}
	ft_putendl("OOOOOOOOOOOOOOOOO 4");
	if (tree && tree->type == PIPE && globalfd != NULL && *globalfd != NULL && (*globalfd)->lstfd != NULL)
	{
		ft_putendl("XXXXXXX CACA 4");
		if ((ret = pushfront_globalfd(globalfd)) != TRUE)
		{
			ft_putendl("XXXXXXX PROUT 4");
			return (ret);
		}
	}
	ft_putendl("OOOOOOOOOOOOOOOOO 5");
	if (ret != FALSE && tree && tree->left)
	{
		ft_putendl("XXXXXXX CACA 5");
		if ((ret = manage_red_fd(tree->left, globalfd, NONE)) != TRUE)
		{
			ft_putendl("XXXXXXX PROUT 5");
			return (ret);
		}
	}
	ft_putendl("OOOOOOOOOOOOOOOOO 6");
	if (globalfd && *globalfd && (*globalfd)->lstfd == NULL)
	{
		ft_putendl("XXXXXXX CACA 6");
		*globalfd = (*globalfd)->next;
	}
	return (TRUE);
}

// créer 3 fct tree_travers_semi tree_travers_pipe tree_travers_cmd
int					tree_traversal(t_node *tree, t_global_fd **globalfd, int pipefd_tab[2][2])
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- TREE TRAVERSAL -------", 2);

	int					ret;

	ret = 0;

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

		//ANTIBUG
		t_global_fd *tmpglo = *globalfd;
		t_lst_fd *tmp = NULL;
		while (tmpglo)
		{
			tmp = tmpglo->lstfd;
			while(tmp){
				printf("in pipe [filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);
				tmp=tmp->next;
			}
			printf("next\n");
			tmpglo = tmpglo->next;
		}
		//  fin ANTIBUG

		if ((ret = (pipe_function(pipefd_tab, tree, globalfd))) != TRUE)
			return (ret);
		if ((ret = (tree_traversal(tree->right, globalfd, pipefd_tab))) != TRUE)
			return (ret);
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
