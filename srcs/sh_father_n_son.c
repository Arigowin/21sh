#include <stdlib.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

static int			father(int pipefd_tab[2][2])
{
	int					stat_loc;

	stat_loc = 0;
	check_signal(3);
	pfd_close(pipefd_tab);
	if (pipefd_tab[1][0] < 0)
		while (waitpid(-1, &stat_loc, WNOHANG) >= 0)
			;
	if (WIFSIGNALED(stat_loc))
		return (-2);
	if (WIFEXITED(stat_loc) != TRUE)
		return (ERROR);
	return (WEXITSTATUS(stat_loc));
}

static int			son(char **cmd, int pipefd_tab[2][2], t_node *tree,
					t_lst_fd **lstfd)
{
	int					ret;
	int					fd;

	fd = (lstfd && *lstfd && tree->left ? (*lstfd)->fd : -2);
	ret = TRUE;
	pfd_handler(pipefd_tab);
	if ((pipefd_tab[0][0] >= 0 || pipefd_tab[1][0] >= 0) && tree && tree->left
	&& lstfd && *lstfd && !(ret = redirect(tree->left, *lstfd)))
	{
		if (ret == ERROR)
			exit_pgm(EXIT_FAILURE);
		if (ret == FALSE)
			exit_pgm(EXIT_SUCCESS);
	}
	if (lstfd && check_builtin(fd, cmd, pipefd_tab, lstfd) == TRUE)
		exit_pgm(EXIT_SUCCESS);
	check_signal(2);
	if (check_fct(fd, cmd) == -2)
		exit_pgm(EXIT_FAILURE);
	return (sh_error(FALSE, 24, cmd[0], NULL));
}

int					handle_fork(int pipefd_tab[2][2], t_node *tree,
					t_lst_fd **lstfd, char **cmd)
{
	pid_t				fpid;

	fpid = -1;
	reset_term();
	if ((fpid = fork()) < 0)
		sh_error(FALSE, 5, NULL, NULL);
	if (fpid == 0)
		son(cmd, pipefd_tab, tree, lstfd);
	else
		father(pipefd_tab);
	init_term(FALSE);
	return (TRUE);
}
