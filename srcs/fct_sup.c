
#include <stdio.h>
int					tree_traversal_verif(t_node *tree)
{
	printf("(((%p)))\n", tree);
	if (tree != NULL)
	{
		printf("tree : %s - %d\n", tree->data, tree->type);
		if (tree && tree->left != NULL)
		{
			printf("{{%s left %s}}\n", tree->data, tree->left->data);
			tree_traversal_verif(tree->left);
		}
		if (tree && tree->right != NULL)
		{
			printf("{{%s right %s}}\n", tree->data, tree->right->data);
			tree_traversal_verif(tree->right);
		}
	}
	else
	printf("\nend tree\n");
	return (0);
}

