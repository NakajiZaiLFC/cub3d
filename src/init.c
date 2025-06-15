#include "../include/cub3d.h"

int	init_game(t_game *game)
{
	int				i;
	t_error_context	context;

	context.file_name = NULL;
	context.line_number = 0;
	context.function_name = "init_game";
	context.additional_info = NULL;
	if (!game)
	{
		context.additional_info = "Null game pointer provided";
		error_report(ERROR_MEMORY, &context);
		return (-1);
	}
	// Initialize game structure
	game->map.grid = NULL;
	game->map.width = 0;
	game->map.height = 0;
	game->map.no_texture = NULL;
	game->map.so_texture = NULL;
	game->map.we_texture = NULL;
	game->map.ea_texture = NULL;
	game->map.floor_color = (t_rgb){0, 0, 0};
	game->map.ceil_color = (t_rgb){0, 0, 0};
	// Initialize textures
	i = 0;
	while (i < 4)
	{
		game->map.textures[i].img_ptr = NULL;
		game->map.textures[i].img_data = NULL;
		game->map.textures[i].width = 0;
		game->map.textures[i].height = 0;
		game->map.textures[i].bits_per_pixel = 0;
		game->map.textures[i].line_length = 0;
		game->map.textures[i].endian = 0;
		i++;
	}
	// Initialize player
	game->player.pos.x = 0.0;
	game->player.pos.y = 0.0;
	game->player.dir.x = 0.0;
	game->player.dir.y = 0.0;
	game->player.plane.x = 0.0;
	game->player.plane.y = 0.0;
	// Initialize MLX pointers
	game->mlx.mlx_ptr = NULL;
	game->mlx.win_ptr = NULL;
	game->mlx.img_ptr = NULL;
	game->mlx.img_data = NULL;
	game->mlx.bits_per_pixel = 0;
	game->mlx.line_length = 0;
	game->mlx.endian = 0;
	// Initialize performance
	game->last_frame_time = 0;
	return (0);
}

int	init_mlx(t_game *game)
{
	t_error_context context;

	context.file_name = NULL;
	context.line_number = 0;
	context.function_name = "init_mlx";
	context.additional_info = NULL;

	if (!game)
	{
		context.additional_info = "Null game pointer provided";
		error_report(ERROR_MEMORY, &context);
		return (-1);
	}

	// Initialize MLX
	game->mlx.mlx_ptr = mlx_init();
	if (!game->mlx.mlx_ptr)
	{
		context.additional_info = "Failed to initialize MLX connection";
		error_report(ERROR_MLX_INIT, &context);
		return (-1);
	}

	// Create window
	game->mlx.win_ptr = mlx_new_window(game->mlx.mlx_ptr, SCREEN_WIDTH,
			SCREEN_HEIGHT, "cub3D");
	if (!game->mlx.win_ptr)
	{
		context.additional_info = "Failed to create MLX window";
		error_report(ERROR_MLX_INIT, &context);
		return (-1);
	}

	// Note: Image is created per frame in render_frame()
	// This allows for dynamic image updates

	return (0);
}