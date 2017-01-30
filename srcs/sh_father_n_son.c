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

	//dprintf(2, "pfd 00 : (%d)\tpfd 01 : (%d)\tpfd 10 : (%d)\tpfd 11 : (%d)\n", pipefd_tab[0][0], pipefd_tab[0][1], pipefd_tab[1][0], pipefd_tab[1][1]);
	if (pipefd_tab && pipefd_tab[0][0] < 0 && pipefd_tab[1][0] >= 0)
	{
		close(pipefd_tab[1][0]);
		if (dup2(pipefd_tab[1][1], STDOUT_FILENO) == ERROR)
			/* RET: error EXIT: false MSG: "dup2 fail" */
			return (ERROR);
	}
	if (pipefd_tab && pipefd_tab[0][0] >= 0 && pipefd_tab[1][0] >= 0)
	{
		close(pipefd_tab[0][1]);
		if(dup2(pipefd_tab[0][0], STDIN_FILENO) == ERROR)
			/* RET: error EXIT: false MSG: "dup2 fail" */
			return (ERROR);
		close(pipefd_tab[1][0]);
		if (dup2(pipefd_tab[1][1], STDOUT_FILENO) == ERROR)
			/* RET: error EXIT: false MSG: "dup2 fail" */
			return (ERROR);
	}
	if (pipefd_tab && pipefd_tab[0][0] >= 0 && pipefd_tab[1][0] < 0)
	{
		close(pipefd_tab[0][1]);
		if(dup2(pipefd_tab[0][0], STDIN_FILENO) == ERROR)
			/* RET: error EXIT: false MSG: "dup2 fail" */
			return (ERROR);
	}
	return (TRUE);
}

int					check_builtin(char **cmd, int pipefd_tab[2][2],
					t_lst_fd **lstfd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- CHECK BUILTIN ------", 2);

	int					ret;

	(void)*pipefd_tab;
	ret = -1;
	if (is_builtin(cmd) != -1)
	{
		if ((ret = handle_builtin(cmd)) == ERROR)
		{
			// useless return
			close_lstfd(lstfd);
			return (ERROR);
		}
		return (TRUE);
	}
	return (FALSE);
}

int 				pfd_close(int pipefd_tab[2][2])
{
	if (pipefd_tab && pipefd_tab[0][0] >= 0)
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

int					son(char **cmd, int pipefd_tab[2][2], t_node *tree,
					t_global_fd **globalfd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- SON ------", 2);

	// dprintf(2, "trololo \n");
	pfd_handler(pipefd_tab);
	if (globalfd && *globalfd)
	//dprintf(2, "tttttttttttttttttttttttttttttt (%p)\n", (*globalfd)->lstfd);
	if ((pipefd_tab[0][0] >= 0 || pipefd_tab[1][0] >= 0) && tree && tree->left
	&& globalfd && *globalfd && redirect(tree->left, (*globalfd)->lstfd) == ERROR)
		/* RET: error EXIT: false MSG: "i don't know" */
		return (ERROR);
	if (check_builtin(cmd, pipefd_tab, NULL) == TRUE)
	{
		exit(EXIT_SUCCESS);
		return (TRUE);
	}
	check_signal(2);
	check_fct(cmd);
	/* RET: error EXIT: true MSG: "command not found" */

	// appeler la fonction d'erreur
	ft_putstr_fd("21sh: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putendl_fd(": command not found", 2);
	exit(EXIT_FAILURE);
	return (FALSE);
}

int					handle_fork(int pipefd_tab[2][2], t_node *tree,
					t_global_fd **globalfd)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- HANDLE FORK ------", 2);

	pid_t				fpid;
	char				**cmd;
	int					ret;

	fpid = -1;
	cmd = NULL;
	ret = -1;
	if ((cmd = format_cmd(tree)) == NULL)
		return (ERROR);
	if (pipefd_tab[0][0] < 0 && pipefd_tab[1][0] < 0)
	{
		//dprintf(2, "tree : ((%s))\tlstfd : ((%s))\n", tree->left->data, (*globalfd)->lstfd->filename);
		// interieur du if a mettre dans une fonction
		if (tree && tree->left && *globalfd && redirect(tree->left, (*globalfd)->lstfd) == ERROR)
		{
			//			close_fd_red(&lstfd, saved_std);
			/* RET: error EXIT: false MSG: "i don't know" */
			return (ERROR);
		}
		else if (tree && tree->left && tree->left->type == DLRED && redirect(tree->left, NULL) == ERROR)
			return (ERROR);
		else if (tree->left && *globalfd)
			*globalfd = (*globalfd)->next;
		savior_tree(tree, TRUE); // TRES IMPORTANT SAVIOR TREE TRUE ICI !!!!
		if ((ret = check_builtin(cmd, pipefd_tab, NULL)) == TRUE)
			return (TRUE);
		if (ret == ERROR)
			// useless return
			return (ERROR);
	}
	if ((fpid = fork()) < 0)
		/* RET: error EXIT: true MSG: "fork fail" */
		return (ERROR);
	reset_term();
	if (fpid == 0)
		son(cmd, pipefd_tab, tree, globalfd);
	else
		father(pipefd_tab);
	init_term();
	free_tab(&cmd); // FREE_MALLOC_OK
	return (TRUE);
}
