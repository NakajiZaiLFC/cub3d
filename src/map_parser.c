#include "../include/cub3d.h"

static int	count_map_lines(int fd)
{
	char	*line;
	char	*trimmed;
	int		count;
	long	current_pos;

	current_pos = lseek(fd, 0, SEEK_CUR);
	count = 0;
	while (read_line(fd, &line) > 0)
	{
		if (line)
		{
			trimmed = ft_strtrim(line);
			if (trimmed && trimmed[0] != '\0')
				count++;
			if (trimmed)
				free(trimmed);
			free(line);
		}
	}
	lseek(fd, current_pos, SEEK_SET);
	return (count);
}

static int	get_map_width(char **map_lines, int height)
{
	int	i;
	int	width;
	int	line_len;

	width = 0;
	i = 0;
	while (i < height)
	{
		if (map_lines[i])
		{
			line_len = strlen(map_lines[i]);
			if (line_len > width)
				width = line_len;
		}
		i++;
	}
	return (width);
}

static char	**read_map_lines(int fd, int *height)
{
	char	**map_lines;
	char	*line;
	char	*trimmed;
	int		i;

	*height = count_map_lines(fd);
	if (*height <= 0)
		return (NULL);
	map_lines = malloc(sizeof(char *) * (*height + 1));
	if (!map_lines)
		return (NULL);
	i = 0;
	while (i < *height && read_line(fd, &line) > 0)
	{
		if (line)
		{
			trimmed = ft_strtrim(line);
			if (trimmed && trimmed[0] != '\0')
			{
				map_lines[i] = trimmed;
				i++;
			}
			else if (trimmed)
			{
				free(trimmed);
			}
			free(line);
		}
	}
	// Adjust height to actual lines read
	*height = i;
	map_lines[i] = NULL;
	return (map_lines);
}

static void	normalize_map_lines(char **map_lines, int height, int width)
{
	int		i;
	int		j;
	int		current_len;
	char	*new_line;

	i = 0;
	while (i < height)
	{
		if (map_lines[i])
		{
			current_len = strlen(map_lines[i]);
			if (current_len < width)
			{
				new_line = malloc(width + 1);
				if (new_line)
				{
					j = 0;
					while (j < current_len)
					{
						new_line[j] = map_lines[i][j];
						j++;
					}
					while (j < width)
					{
						new_line[j] = ' ';
						j++;
					}
					new_line[width] = '\0';
					free(map_lines[i]);
					map_lines[i] = new_line;
				}
			}
		}
		i++;
	}
}

int	parse_map_data_with_first_line(int fd, t_game *game, char *first_line)
{
	char	**map_lines;
	int		height;
	int		width;
	int		i;
	char	*line;
	char	*trimmed;

	// Count remaining lines
	height = count_map_lines(fd) + 1; // +1 for first_line
	map_lines = malloc(sizeof(char *) * (height + 1));
	if (!map_lines)
		return (-1);
	// Add first line
	trimmed = ft_strtrim(first_line);
	if (!trimmed)
	{
		free(map_lines);
		return (-1);
	}
	map_lines[0] = trimmed;
	// Read remaining lines
	i = 1;
	while (i < height && read_line(fd, &line) > 0)
	{
		if (line)
		{
			trimmed = ft_strtrim(line);
			if (trimmed && trimmed[0] != '\0')
			{
				map_lines[i] = trimmed;
				i++;
			}
			else if (trimmed)
			{
				free(trimmed);
			}
			free(line);
		}
	}
	// Adjust height to actual lines read
	height = i;
	map_lines[i] = NULL;
	width = get_map_width(map_lines, height);
	normalize_map_lines(map_lines, height, width);
	game->map.grid = map_lines;
	game->map.height = height;
	game->map.width = width;
	// Validate the parsed map
	if (validate_map(game) != 0)
		return (-1);
	return (0);
}

int	parse_map_data(int fd, t_game *game)
{
	char **map_lines;
	int height;
	int width;

	map_lines = read_map_lines(fd, &height);
	if (!map_lines || height == 0)
		return (-1);

	width = get_map_width(map_lines, height);
	normalize_map_lines(map_lines, height, width);

	game->map.grid = map_lines;
	game->map.height = height;
	game->map.width = width;

	// Validate the parsed map
	if (validate_map(game) != 0)
		return (-1);

	return (0);
}