#include "../include/cub3d.h"

static int	create_rgb_int(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

long	get_time_usec(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

void	limit_framerate(t_game *game)
{
	long	current_time;
	long	elapsed_time;
	long	sleep_time;

	current_time = get_time_usec();
	elapsed_time = current_time - game->last_frame_time;
	if (elapsed_time < FRAME_TIME_USEC)
	{
		sleep_time = FRAME_TIME_USEC - elapsed_time;
		usleep(sleep_time);
	}
	game->last_frame_time = get_time_usec();
}

static void	render_walls(t_game *game)
{
	t_ray	ray;
	int		x;
	int		y;
	int		tex_y;
	double	step;
	double	tex_pos;
	int		color;
	int		pixel_pos;

	x = 0;
	while (x < SCREEN_WIDTH)
	{
		// Cast ray for current column
		init_ray(&ray, game, x);
		perform_dda(&ray, game);
		calculate_wall_distance(&ray, game);
		calculate_wall_height(&ray, game);
		// Calculate texture step and starting position
		step = 1.0 * game->map.textures[ray.tex_num].height / ray.line_height;
		tex_pos = (ray.draw_start - SCREEN_HEIGHT / 2 + ray.line_height / 2)
			* step;
		// Draw the textured column
		y = ray.draw_start;
		while (y <= ray.draw_end)
		{
			tex_y = (int)tex_pos & (game->map.textures[ray.tex_num].height - 1);
			tex_pos += step;
			color = get_texture_pixel(&game->map.textures[ray.tex_num],
					ray.tex_x, tex_y);
			if (y >= 0 && y < SCREEN_HEIGHT && x >= 0 && x < SCREEN_WIDTH)
			{
				pixel_pos = y * game->mlx.line_length + x
					* (game->mlx.bits_per_pixel / 8);
				*(int *)(game->mlx.img_data + pixel_pos) = color;
			}
			y++;
		}
		x++;
	}
}

static void	render_background(t_game *game)
{
	int	x;
	int	y;
	int	pixel_pos;
	int	ceiling_color;
	int	floor_color;

	ceiling_color = create_rgb_int(game->map.ceil_color.r,
			game->map.ceil_color.g, game->map.ceil_color.b);
	floor_color = create_rgb_int(game->map.floor_color.r,
			game->map.floor_color.g, game->map.floor_color.b);
	y = 0;
	while (y < SCREEN_HEIGHT)
	{
		x = 0;
		while (x < SCREEN_WIDTH)
		{
			pixel_pos = y * game->mlx.line_length + x
				* (game->mlx.bits_per_pixel / 8);
			if (y < SCREEN_HEIGHT / 2)
				*(int *)(game->mlx.img_data + pixel_pos) = ceiling_color;
			else
				*(int *)(game->mlx.img_data + pixel_pos) = floor_color;
			x++;
		}
		y++;
	}
}

int	render_frame(t_game *game)
{
	t_error_context context;

	context.file_name = NULL;
	context.line_number = 0;
	context.function_name = "render_frame";
	context.additional_info = NULL;

	if (!game)
	{
		context.additional_info = "Null game pointer provided";
		error_report(ERROR_MEMORY, &context);
		return (-1);
	}

	// Limit framerate
	limit_framerate(game);

	// Create new image for this frame
	if (game->mlx.img_ptr)
		mlx_destroy_image(game->mlx.mlx_ptr, game->mlx.img_ptr);

	game->mlx.img_ptr = mlx_new_image(game->mlx.mlx_ptr, SCREEN_WIDTH,
			SCREEN_HEIGHT);
	if (!game->mlx.img_ptr)
	{
		context.additional_info = "Failed to create frame image";
		error_report(ERROR_MLX_INIT, &context);
		return (-1);
	}

	game->mlx.img_data = mlx_get_data_addr(game->mlx.img_ptr,
			&game->mlx.bits_per_pixel, &game->mlx.line_length,
			&game->mlx.endian);
	if (!game->mlx.img_data)
	{
		context.additional_info = "Failed to get image data address";
		error_report(ERROR_MLX_INIT, &context);
		return (-1);
	}

	// Render background (ceiling and floor)
	render_background(game);

	// Render walls using raycasting
	render_walls(game);

	// Draw minimap
	draw_minimap(game);

	// Display the image
	mlx_put_image_to_window(game->mlx.mlx_ptr, game->mlx.win_ptr,
		game->mlx.img_ptr, 0, 0);

	return (0);
}