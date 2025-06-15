#include "../include/cub3d.h"

int	is_walkable(t_game *game, double x, double y)
{
	int	map_x;
	int	map_y;

	if (!game || !game->map.grid)
		return (0);
	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_x >= game->map.width || map_y < 0
		|| map_y >= game->map.height)
		return (0);
	return (game->map.grid[map_y][map_x] != '1');
}

void	move_forward(t_game *game)
{
	double	nx;
	double	ny;

	if (!game)
		return ;
	nx = game->player.pos.x + game->player.dir.x * MOVE_SPEED;
	ny = game->player.pos.y + game->player.dir.y * MOVE_SPEED;
	if (is_walkable(game, nx, game->player.pos.y))
		game->player.pos.x = nx;
	if (is_walkable(game, game->player.pos.x, ny))
		game->player.pos.y = ny;
}

void	move_backward(t_game *game)
{
	double	nx;
	double	ny;

	if (!game)
		return ;
	nx = game->player.pos.x - game->player.dir.x * MOVE_SPEED;
	ny = game->player.pos.y - game->player.dir.y * MOVE_SPEED;
	if (is_walkable(game, nx, game->player.pos.y))
		game->player.pos.x = nx;
	if (is_walkable(game, game->player.pos.x, ny))
		game->player.pos.y = ny;
}

void	move_left(t_game *game)
{
	double	nx;
	double	ny;

	if (!game)
		return ;
	nx = game->player.pos.x - game->player.plane.x * MOVE_SPEED;
	ny = game->player.pos.y - game->player.plane.y * MOVE_SPEED;
	if (is_walkable(game, nx, game->player.pos.y))
		game->player.pos.x = nx;
	if (is_walkable(game, game->player.pos.x, ny))
		game->player.pos.y = ny;
}

void	move_right(t_game *game)
{
	double	nx;
	double	ny;

	if (!game)
		return ;
	nx = game->player.pos.x + game->player.plane.x * MOVE_SPEED;
	ny = game->player.pos.y + game->player.plane.y * MOVE_SPEED;
	if (is_walkable(game, nx, game->player.pos.y))
		game->player.pos.x = nx;
	if (is_walkable(game, game->player.pos.x, ny))
		game->player.pos.y = ny;
}

void	rotate_left(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;

	if (!game)
		return ;
	old_dir_x = game->player.dir.x;
	game->player.dir.x = game->player.dir.x * cos(-ROT_SPEED)
		- game->player.dir.y * sin(-ROT_SPEED);
	game->player.dir.y = old_dir_x * sin(-ROT_SPEED) + game->player.dir.y
		* cos(-ROT_SPEED);
	old_plane_x = game->player.plane.x;
	game->player.plane.x = game->player.plane.x * cos(-ROT_SPEED)
		- game->player.plane.y * sin(-ROT_SPEED);
	game->player.plane.y = old_plane_x * sin(-ROT_SPEED) + game->player.plane.y
		* cos(-ROT_SPEED);
}

void	rotate_right(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;

	if (!game)
		return ;
	old_dir_x = game->player.dir.x;
	game->player.dir.x = game->player.dir.x * cos(ROT_SPEED)
		- game->player.dir.y * sin(ROT_SPEED);
	game->player.dir.y = old_dir_x * sin(ROT_SPEED) + game->player.dir.y
		* cos(ROT_SPEED);
	old_plane_x = game->player.plane.x;
	game->player.plane.x = game->player.plane.x * cos(ROT_SPEED)
		- game->player.plane.y * sin(ROT_SPEED);
	game->player.plane.y = old_plane_x * sin(ROT_SPEED) + game->player.plane.y
		* cos(ROT_SPEED);
}

int	handle_keypress(int keycode, t_game *game)
{
	if (!game)
		return (-1);
	printf("Key pressed: %d\n", keycode);
	if (keycode == KEY_ESC)
	{
		printf("ESC key pressed - initiating clean exit\n");
		clean_exit(game, EXIT_SUCCESS);
	}
	else if (keycode == KEY_W)
		move_forward(game);
	else if (keycode == KEY_S)
		move_backward(game);
	else if (keycode == KEY_A)
		move_left(game);
	else if (keycode == KEY_D)
		move_right(game);
	else if (keycode == KEY_LEFT)
		rotate_left(game);
	else if (keycode == KEY_RIGHT)
		rotate_right(game);
	return (0);
}

int	handle_close(t_game *game)
{
	if (!game)
		return (-1);

	printf("Window close button clicked - initiating clean exit\n");
	clean_exit(game, EXIT_SUCCESS);
	return (0);
}