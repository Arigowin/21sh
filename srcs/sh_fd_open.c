#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#define RA RED_ARG

static int			saved_fd(int *fd, int *fd_save)
{
	*fd_save = (*fd == -2 ? 0 : *fd_save);
	if (*fd_save == -1)
	{
		if (*fd >= 0)
			close(*fd);
		*fd = -1;
	}
	*fd_save = *fd;
	if (*fd == -1)
		return (FALSE);
	return (TRUE);
}

static int			flag(t_node *tree)
{
	t_types				type;
	int					flags;

	type = tree->type;
	flags = -1;
	flags = (type == RRED ? O_WRONLY | O_TRUNC | O_CREAT : flags);
	flags = (type == DRRED ? O_WRONLY | O_APPEND | O_CREAT : flags);
	flags = (type == LRED ? O_RDONLY : flags);
	flags = (type == RWRED ? O_RDWR | O_APPEND | O_CREAT : flags);
	return (flags);
}

static int			check_fd(int *fd, char *name, t_node *node, t_node *tree)
{
	int					retnum;
	int					ret;

	ret = TRUE;
	if (node && node->data && node->data[0] == '&')
	{
		*fd = (ft_strcmp("&-", node->data) == 0 ? -42
				: ft_atoi(ft_strdup_ignchar(name + 1, '\\')));
		if (*fd >= 0)
		{
			ret = fd_exist(*fd);
				printf("POUET((%d))\n", ret);
			if ((retnum = ft_isstrnum(name + 1)) == 0 || ret == -1)
			{
				*fd = -1;
			}
		printf("0 -- in check fct ret ((%d)) fd ((%d)) retnum ((%d))\n", ret, *fd, retnum);
			if (retnum != 0)
				return (ret);
		}
	}
	else
	{
		if (tree->type == LRED)
			ret = access(name, F_OK);
		*fd = (ret >= 0 ?
		open(name, flag(tree), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) : ret);
		printf("1 -- in check fct ret ((%d)) fd ((%d))\n", ret, *fd);
	}
	return (ret);
}

static int			fct_open(int *fd, int *fd_save, t_node *tree)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- FCT OPEN -------", 2);

	t_node				*node;
	char				*filename;
	int					ret;

	ret = 0;
	filename = NULL;
	node = NULL;
	if (tree && tree->right)
		node = (tree->right->type == RA ? tree->right : tree->right->right);
	if (node && node->data && (filename = node->data) == NULL)
	{printf("TROLOLO\n");
		return (lstfd_node_ret(ERROR, &node, NULL, NULL));
	}
	if ((ret = check_fd(fd, filename, node, tree)) != TRUE)
	{printf("TROLOLO ret ((%d))\n", ret);
		return (ret);
	}
	*fd_save = *fd;
		printf("0 -- ret in fct open ((%d)) fd((%d))\n", ret, *fd);
	if (*fd == -1)
	{
		printf("ret in fct open ((%d)) fd((%d))\n", ret, *fd);
		ret = (ret <= -1 ? 20 : 21);
		ret = ft_strcmp(ft_strdup_ignchar(filename + 1, '\\'), "-") ? ret : 29;
		return (sh_error(FALSE, ret, ft_strdup_ignchar(filename, '\\'), "42"));
	}
	return (TRUE);
}

int					fd_open(int *fd, t_node *tree)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- FD OPEN -------", 2);

	static int			fd_save = 0;

	if (saved_fd(fd, &fd_save) != TRUE)
		return (FALSE);
	if (tree->type == DLRED)
		return (FALSE);
	return (fct_open(fd, &fd_save, tree));
}