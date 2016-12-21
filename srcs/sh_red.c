#include "shell.h"
#include "libft.h"
#include <fcntl.h>

/*
   int				atoi_char(char c)
   {
   char	tmp[2];

   tmp[0] = c;
   tmp[1] = 0;
   return (ft_atoi(tmp));
   }
   */

int				fd_exist(int fd)
{
	if (DEBUG_RED == 1)
		printf("------------ FD EXIST ------------\n");

	if (isatty(fd) == 0)
	{
		ft_putstr("21sh: ");
		ft_putstr(ft_itoa(fd));
		ft_putendl(": Bad file descriptor");
		return (FALSE);
	}

	return (TRUE);
}
/*
   static int		rred(char *filename, int red_fd[2], int flags)
   {
   if (DEBUG_RED == 1)
   printf("------- RRED -------\n");
   int		fd;

   if (filename[0] == '-' && red_fd[1] == 3)
   {
   if (red_fd[0] == -1)
   red_fd[0] = STDOUT_FILENO;
   else if (red_fd[0] == '&')
   {
   red_fd[0] = STDOUT_FILENO;
   close(STDERR_FILENO);
   }
   else
   red_fd[0] = atoi_char(red_fd[0]);

   close(red_fd[0]);
   return (-1);
   }

   if (red_fd[1] == 2)
   {
   fd = ft_atoi(filename);
   if (fd_exist(fd) == FALSE)
   return (ERROR);
   }
   else
   {
   if ((fd = open(filename, flags, 0644)) == ERROR)
   return (ERROR);
   }

   if (red_fd[0] == -1)
   red_fd[0] = STDOUT_FILENO;
   else if (red_fd[0] == '&')
   {
   red_fd[0] = STDOUT_FILENO;
   if (dup2(fd, STDERR_FILENO) == ERROR)
   return (ERROR);
   }
   else
   red_fd[0] = atoi_char(red_fd[0]);

   if (fd_exist(red_fd[0]) == FALSE)
   return (ERROR);
   if (dup2(fd, red_fd[0]) == ERROR)
   return (ERROR);
   if (fd < 0 || fd > 2)
   close(fd);

   return (red_fd[0]);
   }

   static int		lred(char *filename, int red_fd[2])
   {
   if (DEBUG_RED == 1)
   printf("------- LRED -------\n");
   int		fd;

   if (red_fd[0] == -1)
   red_fd[0] = 0;

   if (red_fd[1] == 2)
   {
   fd = ft_atoi(filename);
   if (red_fd[0] == '&')
   red_fd[0] = 0;
   if (fd_exist(red_fd[0]) == FALSE)
return (FALSE);
if (fd_exist(fd) == FALSE)
	return (FALSE);
	}
else
{
	if ((access(filename, F_OK)) == ERROR)
	{
		ft_putstr_fd("21sh: no such file or directory: ", 2);
		ft_putendl_fd(filename, 2);
		return (ERROR);
	}
	if ((fd = open(filename, O_RDONLY)) == ERROR)
		return (ERROR);
}
if (dup2(fd, red_fd[0]) == ERROR)
	return (ERROR);

	return (red_fd[0]);
	}

static int		init_red(t_node *tree, char **filename, int red_fd[2])
{
	if (DEBUG_RED == 1)
		printf("------- INIT RED -------\n");

	red_fd[0] = -1;
	if (tree->right->type ==  RED_FD)
	{
		red_fd[0] = (tree->right->data)[0];
		if (red_fd[0] == '&')
			red_fd[1] = 1;

		if ((tree->right->right->data)[0] == '&')
		{
			red_fd[0] = '&';
			if (ft_isdigit((tree->right->right->data)[1]) ||
					(tree->right->right->data)[1] == '-')
			{
				red_fd[0] = (tree->right->data)[0];
				red_fd[1] = 2;
				if ((tree->right->right->data)[1] == '-')
					red_fd[1] = 3;
			}
			*filename = ft_strsub(tree->right->right->data, 1,
					ft_strlen(tree->right->right->data) - 1);
		}
		else
			*filename = ft_strdup(tree->right->right->data);
	}
	else if ((tree->right->data)[0] == '&')
	{
		red_fd[0] = '&';
		if (ft_isdigit((tree->right->data)[1]))
			red_fd[1] = 2;
		else if ((tree->right->data)[1] == '-')
			red_fd[1] = 3;
		*filename = ft_strsub(tree->right->data, 1,
				ft_strlen(tree->right->data) - 1);
	}
	else
		*filename = ft_strdup(tree->right->data);
	return (TRUE);
}

static int		red_if(int type, char *filename, int red_fd[2], t_intlst **lstfd)
{
	if (DEBUG_RED == 1)
		printf("------- RED IF -------\n");
	int		fd_ret;

	if (type == RRED)
	{
		fd_ret = rred(filename, red_fd, O_WRONLY | O_TRUNC | O_CREAT);
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


static int		red_if2(int type, char *filename, int red_fd[2], t_intlst **lstfd)
{
	if (DEBUG_RED == 1)
		printf("------- RED IF 2 -------\n");
	int		fd_ret;

	if (type == DRRED)
	{
		fd_ret = rred(filename, red_fd, O_WRONLY | O_APPEND | O_CREAT);
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

int				left_red(t_node *tree, t_lst_fd **lstfd)
{
	if (DEBUG_RED == 1)
		printf("------- RIGHT RED -------\n");

	int				fd;

	fd = STDIN_FILENO;
	//	if (tree->type == RED_FD && ft_strcmp(tree->data, "&") != 0)
	//		fd = ft_atoi(tree->data);
	//	else if (tree->type == RED_FD && ft_strcmp(tree->data, "&") == 0)
	return (ERROR);
	//	if (tree->right && tree->type == RED_FD)
	//		tree = tree->right;
	//	if ((*lstfd)->fd == -1)
	//	{
	//		close(fd);
	//		return (TRUE);
	//	}
	//	if (dup2((*lstfd)->fd, fd) == ERROR)
	//		return (ERROR);
	close((*lstfd)->fd);
	//	(*lstfd)->fd = fd;
	//	return (TRUE);
}
*/

int				simple_red(t_node *tree, t_lst_fd **lstfd, int stdfd)
{
	if (DEBUG_RED == 1)
		printf("------- RIGHT RED -------\n");

	int				fd;

	fd = stdfd;
	if (tree->type == RED_FD && ft_strcmp(tree->data, "&") != 0)
		fd = ft_atoi(tree->data);
	else if (tree->type == RED_FD && ft_strcmp(tree->data, "&") == 0)
	{
		// if STDIN_FILENO
		// 		bash: file: ambiguous redirect
		if (stdfd == STDIN_FILENO || dup2((*lstfd)->fd, STDERR_FILENO) == ERROR)
			return (ERROR);
	}
	if (tree->right && tree->type == RED_FD)
		tree = tree->right;
	if ((*lstfd)->fd == -1)
	{
		close(fd);
		return (TRUE);
	}
	if (dup2((*lstfd)->fd, fd) == ERROR)
		return (ERROR);
	if ((*lstfd)->fd > STDERR_FILENO && (stdfd == STDOUT_FILENO
	|| (((*lstfd)->filename)[0] != '&' && (*lstfd)->fd != -1)))
		close((*lstfd)->fd);
	(*lstfd)->fd = fd;
	return (TRUE);
}

int				redirect(t_node *tree, t_lst_fd **lstfd)
{
	if (DEBUG_RED == 1)
		printf("------- RED -------\n");

	int				fd;

	fd = (tree->type == LRED ? STDIN_FILENO : STDOUT_FILENO);
	if (tree && tree->right && (tree->type != DLRED ))//|| tree->type == DRRED))
	{
		if (simple_red(tree->right, lstfd, fd) == ERROR)
			return (ERROR);
	}
	else if (tree && tree->right && tree->type == DLRED)
		;

	if (tree && tree->left && (*lstfd)->next)
	{
		*lstfd = (*lstfd)->next;
		if (redirect(tree->left, lstfd) == ERROR)
			return (ERROR);
	}

	return (TRUE);
	//	int			red_fd[2];
	//	char		*filename;
	//
	//	filename = NULL;
	//	red_fd[0] = -1;
	//	red_fd[1] = -1;
	//
	//	init_red(tree, &filename, red_fd);
	//	if (tree->type == RRED || tree->type == LRED)
	//	{
	//		if (red_if(tree->type, filename, red_fd, lstfd) == ERROR)
	//			return (ERROR);
	//	}
	//	else
	//	{
	//		if (red_if2(tree->type, filename, red_fd, lstfd) == ERROR)
	//			return (ERROR);
	//	}
	//
	//	if (tree->left)
	//	{
	//		if (red(tree->left, lstfd) == ERROR)
	//			return (ERROR);
	//		return (TRUE);
	//	}
	//	else
	//		return(FALSE);
}
