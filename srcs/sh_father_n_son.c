#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

#include <errno.h>
#include <string.h>

int 				pfd_handler(int pipefd_tab[2][2])
{
	if (DEBUG == 1)
		ft_putendl_fd("------- PFD HANDLER ------", 2);

	if (pipefd_tab && pipefd_tab[0][0] < 0 && pipefd_tab[1][0] >= 0)
	{
		close(pipefd_tab[1][0]);
		if (dup2(pipefd_tab[1][1], 1) == ERROR)
			return (ERROR);
	}
	if (pipefd_tab && pipefd_tab[0][0] >= 0 && pipefd_tab[1][0] >= 0)
	{
		close(pipefd_tab[0][1]);
		if(dup2(pipefd_tab[0][0], 0) == ERROR)
			return (ERROR);
		close(pipefd_tab[1][0]);
		if (dup2(pipefd_tab[1][1], 1) == ERROR)
			return (ERROR);
	}
	if (pipefd_tab && pipefd_tab[0][0] >= 0 && pipefd_tab[1][0] < 0)
	{
		close(pipefd_tab[0][1]);
		if(dup2(pipefd_tab[0][0], 0) == ERROR)
			return (ERROR);
	}
	return (TRUE);
}

int					check_builtin(char **cmd, int pipefd_tab[2][2], t_lst_fd **lstfd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- CHECK BUILTIN ------", 2);
	(void)*pipefd_tab;

	int					ret;

	ret = -1;
	if (is_builtin(cmd) != -1)
	{
		if ((ret = handle_builtin(cmd)) == ERROR)
		{
			close_lstfd(lstfd);
			return (ERROR);
		}
		return (TRUE);
	}
	return (FALSE);
}

int 				pfd_close(int pipefd_tab[2][2])
{
	if (pipefd_tab)
	{
		close(pipefd_tab[0][0]);
		close(pipefd_tab[0][1]);
	}
	return (TRUE);
}

int					father(int pipefd_tab[2][2])
{
	if (DEBUG == 1)
		ft_putendl_fd("------- FATHER ------", 2);

	int						stat_loc;

	(void)pipefd_tab;
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

int					son(char **cmd, int pipefd_tab[2][2], t_lst_fd **lstfd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- SON ------", 2);

	pfd_handler(pipefd_tab);
	if (check_builtin(cmd, pipefd_tab, lstfd) == TRUE)
	{
		dprintf(2, "built-in in son");
		exit(EXIT_SUCCESS);
		return (TRUE);
	}
	check_signal(2);
	dprintf(2, "cmd : (%s)\n", cmd[0]);
	check_fct(cmd);

	// appeler la fonction d'erreur
	ft_putstr_fd("21sh: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putendl_fd(": command not found", 2);
	exit(EXIT_FAILURE);
	return (FALSE);
}

int					handle_fork(int pipefd_tab[2][2], t_node *tree, t_lst_fd **lstfd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- HANDLE FORK ------", 2);

	pid_t				fpid;
	char				**cmd;
	int					ret;

	fpid = -1;
	cmd = NULL;
	ret = 0;
	if ((cmd = format_cmd(tree)) == NULL)
		return (ERROR);
	if (tree->left && redirect(tree->left, lstfd) == ERROR)
	{
		//			close_fd_red(&lstfd, saved_std);
		return (ERROR);
	}
	if (pipefd_tab[0][0] < 0 && pipefd_tab[1][0] < 0 && (ret = check_builtin(cmd, pipefd_tab, lstfd)) == TRUE)
		return (TRUE);
	if (ret == ERROR)
		return (ERROR);
	if ((fpid = fork()) < 0)
		return (ERROR);
	reset_term();
	if (fpid == 0)
		son(cmd, pipefd_tab, lstfd);
	else
		father(pipefd_tab);
	init_term();
	return (TRUE);
}
