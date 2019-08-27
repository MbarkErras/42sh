#include "mshell.h"

char *file_reader(int fd)
{
	char b[2];
	char *content;

	while (read(fd, b, 1))
	{
		b[1] = '\0';
		content = ft_strjoin_free(a, b, 1);
	}
	return (content);
}


