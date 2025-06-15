#include "../include/cub3d.h"

int	validate_map(t_game *game)
{
	if (!game || !game->map.grid)
		return (-1);
	if (validate_map_characters(game) != 0)
		return (-1);
	if (validate_player_position(game) != 0)
		return (-1);
	if (validate_map_borders(game) != 0)
		return (-1);
	return (0);
}

int	validate_map_characters(t_game *game)
{
	int		x;
	int		y;
	char	c;

	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width && game->map.grid[y][x])
		{
			c = game->map.grid[y][x];
			if (c != '0' && c != '1' && c != 'N' && c != 'S' && c != 'E'
				&& c != 'W' && c != ' ')
			{
				error_exit("Invalid character in map");
				return (-1);
			}
			x++;
		}
		y++;
	}
	return (0);
}

int	validate_player_position(t_game *game)
{
	int		x;
	int		y;
	int		player_count;
	char	c;

	player_count = 0;
	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width && game->map.grid[y][x])
		{
			c = game->map.grid[y][x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				player_count++;
				if (player_count == 1)
					init_player_from_map(game, x, y, c);
			}
			x++;
		}
		y++;
	}
	if (player_count == 0)
	{
		error_exit("No player starting position found");
		return (-1);
	}
	if (player_count > 1)
	{
		error_exit("Multiple player starting positions found");
		return (-1);
	}
	return (0);
}

void	init_player_from_map(t_game *game, int x, int y, char direction)
{
	game->player.pos.x = (double)x + 0.5;
	game->player.pos.y = (double)y + 0.5;
	if (direction == 'N')
	{
		game->player.dir.x = 0.0;
		game->player.dir.y = -1.0;
	}
	else if (direction == 'S')
	{
		game->player.dir.x = 0.0;
		game->player.dir.y = 1.0;
	}
	else if (direction == 'E')
	{
		game->player.dir.x = 1.0;
		game->player.dir.y = 0.0;
	}
	else if (direction == 'W')
	{
		game->player.dir.x = -1.0;
		game->player.dir.y = 0.0;
	}
	// Set camera plane perpendicular to direction
	game->player.plane.x = -game->player.dir.y * 0.66;
	game->player.plane.y = game->player.dir.x * 0.66;
}

static int	is_wall_or_space(char c)
{
	return (c == '1' || c == ' ');
}

int	validate_map_borders(t_game *game)
{
	int	x;
	int	y;

	// Check all border cells
	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			// Check if this is a border cell
			if (y == 0 || y == game->map.height - 1 || x == 0
				|| x == game->map.width - 1)
			{
				if (!is_wall_or_space(game->map.grid[y][x]))
				{
					error_exit("Map must be surrounded by walls");
					return (-1);
				}
			}
			x++;
		}
		y++;
	}
	// Use flood fill to check internal connectivity
	return (flood_fill_check(game, (int)game->player.pos.x,
			(int)game->player.pos.y));
}

int	flood_fill_check(t_game *game, int start_x, int start_y)
{
	char **visited;
	int stack[10000][2];
	int stack_size;
	int i;
	int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
	int d;

	int x, y, new_x, new_y;
	// Initialize visited array
	visited = malloc(sizeof(char *) * game->map.height);
	if (!visited)
		return (-1);
	i = 0;
	while (i < game->map.height)
	{
		visited[i] = malloc(sizeof(char) * game->map.width);
		if (!visited[i])
		{
			while (--i >= 0)
				free(visited[i]);
			free(visited);
			return (-1);
		}
		memset(visited[i], 0, game->map.width);
		i++;
	}
	// Flood fill using stack
	stack_size = 0;
	stack[stack_size][0] = start_x;
	stack[stack_size][1] = start_y;
	stack_size++;
	while (stack_size > 0)
	{
		stack_size--;
		x = stack[stack_size][0];
		y = stack[stack_size][1];
		if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
		{
			// Reached border through walkable space - map not closed
			i = 0;
			while (i < game->map.height)
				free(visited[i++]);
			free(visited);
			error_exit("Map is not properly enclosed by walls");
			return (-1);
		}
		if (visited[y][x] || game->map.grid[y][x] == '1')
			continue ;
		if (game->map.grid[y][x] == ' ')
		{
			// Check if space is at the edge - if so, it's valid
			if (x == 0 || x == game->map.width - 1 || y == 0
				|| y == game->map.height - 1)
				continue ;
			// Hit a space that should be unreachable in the middle
			i = 0;
			while (i < game->map.height)
				free(visited[i++]);
			free(visited);
			error_exit("Map contains unreachable spaces");
			return (-1);
		}
		visited[y][x] = 1;
		// Add adjacent cells to stack
		d = 0;
		while (d < 4)
		{
			new_x = x + directions[d][0];
			new_y = y + directions[d][1];
			if (new_x >= 0 && new_x < game->map.width && new_y >= 0
				&& new_y < game->map.height && !visited[new_y][new_x]
				&& game->map.grid[new_y][new_x] != '1' && stack_size < 9999)
			{
				stack[stack_size][0] = new_x;
				stack[stack_size][1] = new_y;
				stack_size++;
			}
			d++;
		}
	}
	// Cleanup
	i = 0;
	while (i < game->map.height)
		free(visited[i++]);
	free(visited);
	return (0);
}