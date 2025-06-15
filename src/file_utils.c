#include "../include/cub3d.h"

int	read_line(int fd, char **line)
{
	char buffer[1024];
	int bytes_read;
	int i;
	char c;

	i = 0;
	while (i < 1023)
	{
		bytes_read = read(fd, &c, 1);
		if (bytes_read <= 0)
			break ;
		if (c == '\n')
			break ;
		buffer[i] = c;
		i++;
	}

	if (i == 0 && bytes_read <= 0)
		return (0);

	buffer[i] = '\0';
	*line = ft_strdup(buffer);
	return (i > 0 || bytes_read > 0 ? 1 : 0);
}