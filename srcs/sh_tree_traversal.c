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
			{
				filename = ft_strdup(tree->right->right->data);
			}
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
			{
				filename = ft_strdup(tree->right->data);
			}
		}
		if (boolean)
		{
			fd = open(filename, flags, 0644);
			close(fd);
		}
	}
	replace_dollar(&(tree->data));
	if (tree->left)
	{
		create_file_red(tree->left);
	}
	if (tree->right)
	{
		create_file_red(tree->right);
	}
	return (TRUE);
}

int				tree_traversal(t_node *tree)
{
	if (DEBUG_TREE == 1)
		printf("------- TREE TRAVERSAL -------(%d)\n", tree->type);
	int			saved_std[3];

	if (tree->type == PIPE)
	{
		create_file_red(tree);
		saved_std[0] = dup(STDIN_FILENO);
		saved_std[1] = dup(STDOUT_FILENO);
		saved_std[2] = dup(STDERR_FILENO);
		pipe_function(tree, STDIN_FILENO);
		dup2(saved_std[0], STDIN_FILENO);
		dup2(saved_std[1], STDOUT_FILENO);
		dup2(saved_std[2], STDOUT_FILENO);
		close(saved_std[0]);
		close(saved_std[1]);
		close(saved_std[2]);
	}
	if (tree->type == SEMI)
	{
		create_file_red(tree->left);
		if (tree->left)
			tree_traversal(tree->left);
		else
			return (FALSE);
		if (tree->right)
			tree_traversal(tree->right);
		else
			return (FALSE);
	}
	if (tree->type == CMD)
	{
		create_file_red(tree);
		manage_cmd(tree);
	}

	return (TRUE);
}
