#include "shell.h"
#include "libft.h"
#include "fcntl.h"

int				atoi_char(char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = 0;
	return (ft_atoi(tmp));
}

static int		rred(char *filename, int red_fd, int flags)
{
	if (DEBUG_RED == 1)
		printf("------- RRED -------\n");
	int		fd;

	if ((fd = open(filename, flags, 0644)) == ERROR)
		return (ERROR);

	if (red_fd == -1)
		red_fd = 1;
	else if (red_fd == '&')
	{
		red_fd = 1;
		if (dup2(fd, 2) == ERROR)
			return (ERROR);
	}
	else
		red_fd = atoi_char(red_fd);

	if (dup2(fd, red_fd) == ERROR)
		return (ERROR);

	return (red_fd);
}

static int		lred(char *filename, int red_fd)
{
	if (DEBUG_RED == 1)
		printf("------- LRED -------\n");
	int		fd;

	if (red_fd == -1)
		red_fd = 0;

	if ((access(filename, F_OK)) == ERROR)
	{
		ft_putstr_fd("21sh: no such file or directory: ", 2);
		ft_putendl_fd(filename, 2);
		return (ERROR);
	}

	if ((fd = open(filename, O_RDONLY)) == ERROR)
		return (ERROR);

	if (dup2(fd, red_fd) == ERROR)
		return (ERROR);

	return (red_fd);
}

static int		init_red(t_node *tree, char **filename)
{
	if (DEBUG_RED == 1)
		printf("------- INIT RED -------\n");
	int		red_fd;

	red_fd = -1;
	if (tree->right->type == RED_FD)
	{
		red_fd = (tree->right->data)[0];

		if ((tree->right->right->data)[0] == '&')
		{
			red_fd = '&';
			*filename = ft_strsub(tree->right->right->data, 1,
					ft_strlen(tree->right->right->data) - 1);
		}
		else
			*filename = ft_strdup(tree->right->right->data);
	}
	else if ((tree->right->data)[0] == '&')
	{
		red_fd = '&';
		*filename = ft_strsub(tree->right->data, 1,
				ft_strlen(tree->right->data) - 1);
	}
	else
		*filename = ft_strdup(tree->right->data);
	return (red_fd);
}

static int		red_if(int type, char *filename, int red_fd, t_intlst **lstfd)
{
	if (DEBUG_RED == 1)
		printf("------- RED IF -------\n");
	int		fd_ret;

	if (type == RRED)
	{
		fd_ret = rred(filename, red_fd, O_RDWR | O_CREAT);
		if (fd_ret != ERROR)
		{
			ft_intlst_add(lstfd, fd_ret);
			return (TRUE);
		}
		else
			return (ERROR);
	}
	else if (type == LRED)
	{
		fd_ret = lred(filename, red_fd);
		if (fd_ret != ERROR)
		{
			ft_intlst_add(lstfd, fd_ret);
			return (TRUE);
		}
		else
			return (ERROR);
	}
	return (FALSE);
}


static int		red_if2(int type, char *filename, int red_fd, t_intlst **lstfd)
{
	if (DEBUG_RED == 1)
		printf("------- RED IF 2 -------\n");
	int		fd_ret;

	if (type == DRRED)
	{
		fd_ret = rred(filename, red_fd, O_RDWR | O_CREAT | O_APPEND);
		if (fd_ret != ERROR)
		{
			ft_intlst_add(lstfd, fd_ret);
			return (TRUE);
		}
		else
			return (ERROR);
	}
	else if (type == DLRED)
	{
		return (TRUE);
	}

	return (FALSE);
}

int				red(t_node *tree, t_intlst **lstfd)
{
	if (DEBUG_RED == 1)
		printf("------- RED -------\n");
	char		red_fd;
	char		*filename;

	filename = NULL;

	red_fd = init_red(tree, &filename);
	if (tree->type == RRED || tree->type == LRED)
		red_if(tree->type, filename, red_fd, lstfd);
	else
		red_if2(tree->type, filename, red_fd, lstfd);

	if (tree->left)
	{
		red(tree->left, lstfd);
		return (TRUE);
	}
	else
		return(FALSE) ;
}
