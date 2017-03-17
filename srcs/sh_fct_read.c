#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "shell.h"
#include "libft.h"

static int			read_n_check(int *nb_hrd, char *read_buff, t_node **tree)
{
	t_e_list			*l_expr;
	t_e_list			*save;
	static int			hrd = 0;
	int					ret;

	l_expr = NULL;
	save = NULL;
	read_buff = ft_strtrim(read_buff);
	if (tree == NULL || read_buff == NULL)
		return (FALSE);
	if ((ret = tokenizer(&hrd, read_buff, &l_expr)) != TRUE)
		return (str_dbltbl_ret(ret, &read_buff, NULL, NULL));
	save = l_expr;
	ft_strdel(&read_buff);
	if ((ret = lexer(&l_expr)) != TRUE)
		return (ret);
	if ((ret = parser(nb_hrd, &l_expr, tree)) != TRUE)
		return (telist_ret(ret, &save, NULL, NULL));
	savior_tree(*tree, TRUE);
	expr_del(&save);
	return (TRUE);
}

int					check_after_read(t_line *stline, t_history **history)
{
	t_node				*tree;
	t_node				*node;
	t_lst_fd			*lstfd;
	int					pipefd_tab[2][2];
	int					ret;

	lstfd = NULL;
	tree = NULL;
	node = NULL;
	init_pipefd(pipefd_tab);
	ret = read_n_check(&(stline->hrd.nb), stline->line, &tree);
	if (ret != TRUE)
		return (lstfd_node_ret(ret, &tree, NULL, NULL));
	node = tree;
	if ((ret = heredoc_handler(stline, &node, history)) == ERROR)
		return (FALSE);
	if ((ret = tree_traversal(tree, &lstfd, pipefd_tab)) == ERROR)
		return (lstfd_node_ret(ret, &tree, NULL, NULL));
	return (lstfd_node_ret(ret, &tree, &lstfd, NULL));
}

int					ctrl_c_hrd(t_line *stline)
{
	int					fd;

	fd = 0;
	if (stline->ctrl_c == TRUE)
	{
		if ((fd = open(ttyname(1), O_RDWR)) < 0)
			return (sh_error(FALSE, 2, ttyname(1), NULL));
		close(0);
		if (dup2(fd, STDIN_FILENO) < 0)
			return (sh_error(FALSE, 7, NULL, NULL));
		if (fd > STDERR_FILENO)
			close(fd);
		stline->ctrl_c = FALSE;
	}
	return (TRUE);
}

int					fct_read(int hrd, t_line *stline, t_history **history)
{
	int					key;
	int					ret;
	int					event_ret;

	ret = 0;
	key = 0;
	while ((ret = read(STDIN_FILENO, &key, sizeof(int))) > 0)
	{
		if ((event_ret = event(key, stline, history)) == BREAK)
			break ;
		else if (event_ret == CONTINUE)
			continue ;
		key = 0;
	}
	ctrl_c_hrd(stline);
	if (key == RETURN && (stline->line)[0] == '\0')
		return (FALSE);
	if (ret <= 0)
		return (ERROR);
	if (hrd == TRUE)
		return (TRUE);
	if (check_after_read(stline, history) == ERROR)
		return (ERROR);
	return (TRUE);
}
