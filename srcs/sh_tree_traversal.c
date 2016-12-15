#include "shell.h"
#include "libft.h"
#include <fcntl.h>

int 			create_file_red(t_node *tree)
{
	int			fd;
	int 		flags;
	char		*filename;
	int			boolean;

	fd = -1;
	flags = 0;
	filename = NULL;
	boolean = TRUE;
	if (tree->type == RRED || tree->type == DRRED)
	{
		if (tree->type == RRED)
			flags = O_WRONLY | O_TRUNC | O_CREAT;
		else
			flags = O_WRONLY | O_APPEND | O_CREAT;
		if (tree->right->type == RED_FD)
		{
			if ((tree->right->right->data)[0] == '&')
			{
				filename = ft_strsub(tree->right->right->data, 1,
						ft_strlen(tree->right->right->data) - 1);
				if (filename[0] == '-' || ft_isstrnum(filename))
					boolean = FALSE;
			}
			else
				filename = ft_strdup(tree->right->right->data);
		}
		else
		{
			if ((tree->right->data)[0] == '&')
			{
				filename = ft_strsub(tree->right->data, 1,
						ft_strlen(tree->right->data) - 1);
				if (filename[0] == '-' || ft_isstrnum(filename))
					boolean = FALSE;
			}
		else
			filename = ft_strdup(tree->right->data);
		}
		if (boolean)
		{
			fd = open(filename, flags, 0644);
			close(fd);
		}
	}
	if (tree->left)
		create_file_red(tree->left);
	if (tree->right)
		create_file_red(tree->right);
	return (TRUE);
}

int				tree_traversal(t_node *tree, t_lst_fd **lstfd)
{
	if (DEBUG_TREE == 1)
		printf("------- TREE TRAVERSAL -------(%d)\n", tree->type);
	int			saved_std[3];

	if (tree->type == PIPE)
	{
		init_std_fd(&saved_std);
		pipe_function(tree, STDIN_FILENO);
		reset_std_fd(saved_std);
	}
	if (tree->type == SEMI)
	{
		if (tree->left)
			tree_traversal(tree->left, lstfd);
		else
			return (ERROR);
		if (tree->right)
			tree_traversal(tree->right, lstfd);
		else
			return (ERROR);
	}
	if (tree->type == CMD)
	{
		manage_cmd(tree);
	}

	return (TRUE);
}
