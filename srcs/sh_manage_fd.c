#include <fcntl.h>
#include "shell.h"
#include "libft.h"

int					init_std_fd(int (*std_fd)[])
{
	(*std_fd)[0] = dup(STDIN_FILENO);
	(*std_fd)[1] = dup(STDOUT_FILENO);
	(*std_fd)[2] = dup(STDERR_FILENO);
	return (TRUE);
}	

int					reset_std_fd(int std_fd[])
{
	dup2(std_fd[0], STDIN_FILENO);
	dup2(std_fd[1], STDOUT_FILENO);
	dup2(std_fd[2], STDERR_FILENO);
	close(std_fd[0]);
	close(std_fd[1]);
	close(std_fd[2]);
	return (TRUE);
}

static t_lst_fd		*lstfd_new(int fd, char *filename)
{
	t_lst_fd	*new;

	if ((new = (t_lst_fd*)malloc(sizeof(t_lst_fd))) == NULL)
		return (NULL);
	new->fd = fd;
	if ((new->filename = ft_strdup(filename)) == NULL)
		return (NULL);
	new->next = NULL;
	return (new);
}

int					lstfd_pushbck(t_lst_fd **lstfd, int fd, char *filename)
{
	t_lst_fd *new;
	t_lst_fd *tmp;

	new = NULL;
	tmp = *lstfd;
	if (!(lstfd && *lstfd))
		*lstfd = lstfd_new(fd, filename);
	else
	{
		while (tmp && tmp->next)
			tmp = tmp->next;
		new = lstfd_new(fd, filename);
		tmp->next = new;
	}
	return (TRUE);
}

char				*check_file_name(char *str)
{
	char *file_name;

	if ((file_name = ft_strsub(str, 1, ft_strlen(str))) == NULL)
		return (NULL);
	if (ft_isstrnum(file_name))
		return (NULL);
	return (file_name);
}

int					right_red_fd(t_lst_fd **lstfd, t_node *tree)
{
	int 		flags;
	t_node		*red_arg;
	char		*file_name;
	

	(void)lstfd;
	file_name = NULL;
	red_arg = (tree->right->type == RED_ARG ? tree->right : tree->right->right); 
	if (tree->type == RRED)
		flags = O_WRONLY | O_TRUNC | O_CREAT;
	else
		flags = O_WRONLY | O_APPEND | O_CREAT;
	if (red_arg->data[0] == '&')
		file_name = check_file_name(red_arg->data);
	return (TRUE);
}

int					left_red_fd(t_lst_fd **lstfd, t_node *tree)
{
	(void)lstfd;
	(void)tree;
	return (TRUE);
}

int					manage_red_file(t_lst_fd **lstfd, t_node *tree)
{
	if (tree->type == RRED || tree->type == DRRED)
		right_red_fd(lstfd, tree);
	else if (tree->type == LRED)
		;
	else if (tree->type == DLRED)
		;

	if (tree->left)
		manage_red_file(lstfd, tree->left);
	if (tree->right)
		manage_red_file(lstfd, tree->right);
	return (TRUE);
}	
