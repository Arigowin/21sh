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

int				tree_traversal(t_node *tree, char **env)
{
    (void)env;
    (void)tree;

    if (PIPE)
    {
        // pipe_function(tree->left);
        // va socuper de tout les pipes
    }
    if (SEMI)
    {
        // this(tree->left);
        // this(tree->right);
    }
    if (CMD)
    {
        // pas de pipe, semi, etc si on arrive ici
        // donc exec cmd
        // cmd();
        // father_n_son ????? car la fonction a deja tout
        // se qu'il faut pour ici sauf si ces un builtin donc
        // trouver la fonction qui fait le choix
    }

    return (TRUE);
}
