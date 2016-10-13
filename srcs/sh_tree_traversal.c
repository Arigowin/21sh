#include "shell.h"
#include "libft.h"


void pipe_function(t_node *tree)
{
    int     pfd[2];

    pipe(pfd);
    (void)tree;

    // exec pipe
    // qui va open and close fd and execut cmd
    // voir fichier pipe.c
    // finir par close tout les fd open (red and pipe)
}

int				tree_traversal(t_node *tree, t_duo **env)
{
    (void)env;

    if (tree->type == PIPE)
    {
        // pipe_function(tree->left);
        // va socuper de tout les pipes
    }
    if (tree->type == SEMI)
    {
        // this(tree->left);
        // this(tree->right);
    }
    if (tree->type == CMD)
    {
		manage_cmd(tree, env);
    }

    return (TRUE);
}
