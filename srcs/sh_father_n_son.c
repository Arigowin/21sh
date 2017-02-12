#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

static int				father(int pipefd_tab[2][2])
{
	if (DEBUG == 1)
		ft_putendl_fd("------- FATHER ------", 2);

	int						stat_loc;

	stat_loc = 0;
	check_signal(3);
	pfd_close(pipefd_tab);
	if (pipefd_tab[1][0] < 0)
		while (waitpid(-1, &stat_loc, WNOHANG) >= 0)
			;
	if (WIFSIGNALED(stat_loc))
		return (-2); // revoir le code de retour
	if (WIFEXITED(stat_loc) != TRUE)
		return (ERROR);
	return (WEXITSTATUS(stat_loc));
}

static int			son(char **cmd, int pipefd_tab[2][2], t_node *tree,
					t_lst_fd **lstfd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- SON ------", 2);

	int 				ret;
	int 				fd;

	fd = (lstfd && *lstfd && tree->left ? (*lstfd)->fd : -2);
	ret = TRUE;
	pfd_handler(pipefd_tab);
	if ((pipefd_tab[0][0] >= 0 || pipefd_tab[1][0] >= 0) && tree && tree->left
	&& lstfd && *lstfd && !(ret = redirect(tree->left, *lstfd)))
	{
		if (ret == ERROR)
		{
/* RET: error EXIT: false MSG: "i don't know"  ==> si error onne remonte pas jusque là! en fait ca depend si on veut quitter le fork ou le pgm */
			exit(EXIT_FAILURE);
		}
		if (ret == FALSE)
			exit(EXIT_SUCCESS);
	}
	if (lstfd && check_builtin(fd, cmd, pipefd_tab, lstfd) == TRUE)
		exit(EXIT_SUCCESS);
	check_signal(2);
	if (check_fct(fd, cmd) == -2)
		exit(EXIT_FAILURE);
	return (sh_error(24, cmd[0], NULL));
//	ft_putstr_fd("21sh: ", 2);
//	ft_putstr_fd(cmd[0], 2);
//	ft_putendl_fd(": command not found", 2);
//	exit(EXIT_FAILURE);
//	return (FALSE);
}

int					handle_fork(int pipefd_tab[2][2], t_node *tree,
					t_lst_fd **lstfd, char **cmd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- HANDLE FORK ------", 2);

	pid_t				fpid;

	fpid = -1;
	reset_term();
	if ((fpid = fork()) < 0)
		sh_error(5, NULL, NULL);
		/* RET: error EXIT: true MSG: "fork fail" */
	if (fpid == 0)
		son(cmd, pipefd_tab, tree, lstfd);
	else
		father(pipefd_tab);
	init_term(FALSE);
	return (TRUE);
}
