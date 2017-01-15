#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

#include <errno.h>
#include <string.h>

int					check_builtin(char **cmd, int pipefd_tab[2][2])
{
	if (DEBUG == 1)
		ft_putendl_fd("------- CHECK BUILTIN ------", 2);
	(void)cmd;
	(void)*pipefd_tab;
//
//	int					ret;
//
//	ret = -1;
//	if (is_builtin(cmd) != -1)
//	{
//		if (*pipefd_tab[0] && *pipefd_tab[1])
//		{
//	ft_putendl_fd("*pipefd_tab[0]", 2);
//	ft_putnbr_fd(*pipefd_tab[0], 2);
//	ft_putendl_fd("", 2);
//	ft_putendl_fd("*pipefd_tab[1]", 2);
//	ft_putnbr_fd(*pipefd_tab[1], 2);
//	ft_putendl_fd("", 2);
//			if (*pipefd_tab[1] != STDOUT_FILENO)
//			{
//				ft_putendl_fd("TATA2", 2);
//				if (dup2(*pipefd_tab[1], STDOUT_FILENO) == ERROR)
//					ft_putendl_fd("ERROR4", 2);
//				close(*pipefd_tab[1]);
//			}
//			if (*pipefd_tab[0] != STDIN_FILENO)
//			{
//				ft_putendl_fd("TATA1", 2);
//				if (dup2(*pipefd_tab[0], STDIN_FILENO) == ERROR)
//					ft_putendl_fd("ERROR3", 2);
//				close(*pipefd_tab[0]);
//			}
//		}
//		if ((ret = handle_builtin(cmd)) == ERROR)
//		{
//			//close_lstfd(lstfd);
//			return (ERROR);
//		}
//		return (TRUE);
//	}
	return (FALSE);
}

int					reset_stdin(int fd_in)
{
	if (fd_in != STDIN_FILENO)
	{
		if ((fd_in = open(savior_tty(NULL, FALSE, FALSE), O_RDWR)) == -1)
			return (FALSE);
		if (dup2(fd_in, STDIN_FILENO) == -1)
			return (FALSE);
		if (fd_in > STDERR_FILENO)
			close(fd_in);
	}
	return (TRUE);
}

int					reset_stdout(int fd_out)
{
	if (fd_out != STDOUT_FILENO)
	{
		if ((fd_out = open(savior_tty(NULL, FALSE, TRUE), O_RDWR)) == -1)
			return (FALSE);
		if (dup2(fd_out, STDOUT_FILENO) == -1)
			return (FALSE);
		if (fd_out > STDERR_FILENO)
			close(fd_out);
	}
	return (TRUE);
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
	//if (*pipefd_tab && *pipefd_tab[1])
	//reset_stdout(*pipefd_tab[1][0]);
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

int 				pfd_handler(int pipefd_tab[2][2])
{
dprintf(2, "debut handler case 0 :(%d)(%d)\n", pipefd_tab[0][0], pipefd_tab[0][1]);
dprintf(2, "debut handler case 1 :(%d)(%d)\n", pipefd_tab[1][0], pipefd_tab[1][1]);
	if (pipefd_tab && pipefd_tab[0][0] >= 0 && pipefd_tab[1][0] < 0)
	{
dprintf(2, "1eme if\n");
		close(pipefd_tab[0][1]);
		dup2(pipefd_tab[0][0], 0);
	}
	if (pipefd_tab && pipefd_tab[0][0] >= 0 && pipefd_tab[1][0] >= 0)
	{
dprintf(2, "2eme if\n");
		close(pipefd_tab[0][1]);
		dup2(pipefd_tab[0][0], 0);
		close(pipefd_tab[1][0]);
		dup2(pipefd_tab[1][1], 1);
	}
	if (pipefd_tab && pipefd_tab[0][0] < 0 && pipefd_tab[1][0] >= 0)
	{
dprintf(2, "3eme if\n");
		close(pipefd_tab[1][0]);
		dup2(pipefd_tab[1][1], 1);
	}
dprintf(2, "end handler case 0 :(%d)(%d)\n", pipefd_tab[0][0], pipefd_tab[0][1]);
dprintf(2, "end handler case 1 :(%d)(%d)\n", pipefd_tab[1][0], pipefd_tab[1][1]);
	return (TRUE);
}

int					son(int pipefd_tab[2][2], char **cmd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- SON ------", 2);

	//	ft_putendl_fd("WHY????????\n");
	//	if (check_builtin(tree, lstfd, cmd) == TRUE)
	//		return (TRUE);
	check_signal(2);
	dprintf(2, "cmd : (%s)\n", cmd[0]);
	pfd_handler(pipefd_tab);

//	if (fd_in != -1 && fd_out != -1)
//	{
//	ft_putendl_fd("fd_in", 2);
//	ft_putnbr_fd(fd_in, 2);
//	ft_putendl_fd("", 2);
//	ft_putendl_fd("fd_out", 2);
//	ft_putnbr_fd(fd_out, 2);
//	ft_putendl_fd("", 2);
//
//		if (fd_out != STDOUT_FILENO)
//		{
//			ft_putendl_fd("TITI2", 2);
//			if (dup2(fd_out, STDOUT_FILENO) == ERROR)
//				ft_putendl_fd("ERROR2", 2);
//	//		perror(strerror(errno));
//			close(fd_out);
//		}
//		if (fd_in != STDIN_FILENO)
//		{
//			ft_putendl_fd("TITI1", 2);
//			if (dup2(fd_in, STDIN_FILENO) == ERROR)
//				ft_putendl_fd("ERROR1", 2);
//			close(fd_in);
//		}
//	}
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
//	int					pfd[2];

	fpid = -1;
	cmd = NULL;
	ret = 0;
	if ((cmd = format_cmd(tree)) == NULL)
		return (ERROR);
	if (tree->left != NULL)
	{
		if (redirect(tree->left, lstfd) == ERROR)
		{
			//			close_fd_red(&lstfd, saved_std);
			return (ERROR);
		}
	}
	if ((ret = check_builtin(cmd, pipefd_tab)) == TRUE) // + verif si il n'y a pas de pipe
		return (TRUE);
	if (ret == ERROR)
		return (ERROR);
	if ((fpid = fork()) < 0)
		return (ERROR);
	reset_term();
	if (fpid == 0)
	{
//		close(*pipefd_tab[1][0]);
		son(pipefd_tab, cmd);
//		dup2(*pipefd_tab[1][1], 1);
//		close(*pipefd_tab[1][1]);
	}
	else
	{
//		close(*pipefd_tab[1][1]);
//		dup2(*pipefd_tab[1][0], 0);
		father(pipefd_tab);
//		close(*pipefd_tab[1][0]);
//		reset_stdin(*pipefd_tab[1][1]);
	}
	init_term();
	return (TRUE);
}
/*
int					handle_fork(int **pipefd_tab[2], t_node *tree, t_lst_fd **lstfd)
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
	if (tree->left != NULL)
	{
		if (redirect(tree->left, lstfd) == ERROR)
		{
			//			close_fd_red(&lstfd, saved_std);
			return (ERROR);
		}
	}
	if ((ret = check_builtin(cmd, *pipefd_tab)) == TRUE) // + verif si il n'y a pas de pipe
		return (TRUE);
	if (ret == ERROR)
		return (ERROR);
	if ((fpid = fork()) < 0)
		return (ERROR);
	reset_term();
	if (fpid == 0)
	{

		son(*pipefd_tab, cmd);
	}
	else
		father(*pipefd_tab);
	init_term();
	return (TRUE);
}
*/
