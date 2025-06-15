#include "../include/cub3d.h"

static int	check_file_extension(const char *filename)
{
	int			len;
	const char	*extension;

	if (!filename)
		return (0);
	len = strlen(filename);
	if (len < 5)
		return (0);
	extension = filename + len - 4;
	return (ft_strncmp(extension, ".cub", 4) == 0);
}

static int	check_file_access(const char *filename, t_error_context *context)
{
	int	fd;

	if (!check_file_extension(filename))
	{
		context->additional_info = "File must have .cub extension";
		error_report(ERROR_FILE_INVALID, context);
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		context->file_name = filename;
		context->additional_info = "Cannot open file";
		error_report(ERROR_FILE_NOT_FOUND, context);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	parse_element_line(char *line, t_game *game,
		t_error_context *context)
{
	char	*trimmed;

	trimmed = ft_strtrim(line);
	if (!trimmed)
	{
		context->additional_info = "Memory allocation failed during line parsing";
		error_report(ERROR_MEMORY, context);
		return (-1);
	}
	if (*trimmed == '\0')
	{
		free(trimmed);
		return (0);
	}
	if (parse_texture_line(trimmed, game) == 0)
	{
		free(trimmed);
		return (0);
	}
	if (parse_color_line(trimmed, game) == 0)
	{
		free(trimmed);
		return (0);
	}
	free(trimmed);
	context->additional_info = "Unrecognized configuration element";
	error_report(ERROR_CONFIG_INVALID, context);
	return (-1);
}

static int	parse_elements(int fd, t_game *game, t_error_context *context)
{
	char	*line;
	int		line_number;

	line_number = 0;
	while (read_line(fd, &line) > 0)
	{
		line_number++;
		context->line_number = line_number;
		if (!line || *line == '\0')
		{
			free(line);
			continue ;
		}
		if (is_map_line(line))
		{
			if (parse_map_data_with_first_line(fd, game, line) != 0)
			{
				free(line);
				context->additional_info = "Failed to parse map starting from this line";
				error_report(ERROR_MAP_INVALID, context);
				return (-1);
			}
			free(line);
			return (0);
		}
		if (parse_element_line(line, game, context) != 0)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	context->additional_info = "No map found in configuration file";
	error_report(ERROR_CONFIG_MISSING_ELEMENT, context);
	return (-1);
}

int	parse_config_file(const char *filename, t_game *game)
{
	int				fd;
	t_error_context	context;

	context.file_name = filename;
	context.line_number = 0;
	context.function_name = "parse_config_file";
	context.additional_info = NULL;
	if (check_file_access(filename, &context) != 0)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		context.additional_info = "Failed to open file for reading";
		error_report(ERROR_FILE_NOT_FOUND, &context);
		return (-1);
	}
	if (parse_elements(fd, game, &context) != 0)
	{
		close(fd);
		return (-1);
	}
	// parse_elements already handles map parsing when it encounters map lines
	// so we don't need to call parse_map_data again
	close(fd);
	if (validate_map(game) != 0)
	{
		context.additional_info = "Map validation failed";
		error_report(ERROR_MAP_INVALID, &context);
		return (-1);
	}
	return (0);
}

int	parse_texture_line(char *line, t_game *game)
{
	char	**split;

	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1])
	{
		free_split(split);
		return (-1);
	}
	if (ft_strncmp(split[0], "NO", 2) == 0 && !game->map.no_texture)
		game->map.no_texture = ft_strdup(split[1]);
	else if (ft_strncmp(split[0], "SO", 2) == 0 && !game->map.so_texture)
		game->map.so_texture = ft_strdup(split[1]);
	else if (ft_strncmp(split[0], "WE", 2) == 0 && !game->map.we_texture)
		game->map.we_texture = ft_strdup(split[1]);
	else if (ft_strncmp(split[0], "EA", 2) == 0 && !game->map.ea_texture)
		game->map.ea_texture = ft_strdup(split[1]);
	else
	{
		free_split(split);
		return (-1);
	}
	free_split(split);
	return (0);
}

int	parse_color_line(char *line, t_game *game)
{
	char	**split;

	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1])
	{
		free_split(split);
		return (-1);
	}
	if (ft_strncmp(split[0], "F", 1) == 0)
	{
		if (parse_rgb_values(split[1], &game->map.floor_color) != 0)
		{
			free_split(split);
			return (-1);
		}
	}
	else if (ft_strncmp(split[0], "C", 1) == 0)
	{
		if (parse_rgb_values(split[1], &game->map.ceil_color) != 0)
		{
			free_split(split);
			return (-1);
		}
	}
	else
	{
		free_split(split);
		return (-1);
	}
	free_split(split);
	return (0);
}

int	parse_rgb_values(char *rgb_str, t_rgb *color)
{
	char	**rgb_split;

	int r, g, b;
	rgb_split = ft_split(rgb_str, ',');
	if (!rgb_split || !rgb_split[0] || !rgb_split[1] || !rgb_split[2])
	{
		free_split(rgb_split);
		return (-1);
	}
	r = atoi(rgb_split[0]);
	g = atoi(rgb_split[1]);
	b = atoi(rgb_split[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
		free_split(rgb_split);
		return (-1);
	}
	color->r = r;
	color->g = g;
	color->b = b;
	free_split(rgb_split);
	return (0);
}

int	is_map_line(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '1' || line[i] == '0' || line[i] == 'N' || line[i] == 'S'
			|| line[i] == 'E' || line[i] == 'W' || line[i] == ' ')
			i++;
		else
			return (0);
	}
	return (i > 0);
}