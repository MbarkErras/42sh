#include "mshell.h"

void	setter(char *set, char *elements)
{
	int i;

	i = ft_strlen(set) - 1;
	while (*elements)
	{
		if (!ft_strchr(set, *elements))
			set[i++] = *elements;
		elements++;
	}
}
