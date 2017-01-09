#include <stdlib.h>
#include <string.h>
#include "libft.h"

char				*ft_strdup_ignchar(const char *s1, char c)
{
	int					i;
	int					j;
	char				*rlt;
	int					str_len;

	i = 0;
	j = 0;
	if (s1 == NULL)
		return (NULL);
	str_len = ft_strlen(s1);
	rlt = (char*)malloc((str_len + 1) - ft_strncount((char *)s1, c));
	if (!rlt)
		return (NULL);
	while (i < str_len)
	{
		if (s1[i] == c)
			i++;
		rlt[j] = s1[i];
		i++;
		j++;
	}
	rlt[i] = '\0';
	return (rlt);
}
