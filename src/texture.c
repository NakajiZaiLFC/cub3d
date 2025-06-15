#include "../include/cub3d.h"

int	create_texture_image(t_game *game, int tex_num, int color_base)
{
	int				pixel_pos;
	int				color;
	t_error_context	context;

	int x, y;
	context.file_name = NULL;
	context.line_number = 0;
	context.function_name = "create_texture_image";
	context.additional_info = NULL;
	if (!game || tex_num < 0 || tex_num >= 4)
	{
		context.additional_info = "Invalid parameters provided";
		error_report(ERROR_MEMORY, &context);
		return (-1);
	}
	game->map.textures[tex_num].width = 64;
	game->map.textures[tex_num].height = 64;
	game->map.textures[tex_num].img_ptr = mlx_new_image(game->mlx.mlx_ptr, 64,
			64);
	if (!game->map.textures[tex_num].img_ptr)
	{
		context.additional_info = "Failed to create MLX image for texture";
		error_report(ERROR_TEXTURE_LOAD, &context);
		return (-1);
	}
	game->map.textures[tex_num].img_data = mlx_get_data_addr(game->map.textures[tex_num].img_ptr,
			&game->map.textures[tex_num].bits_per_pixel,
			&game->map.textures[tex_num].line_length,
			&game->map.textures[tex_num].endian);
	if (!game->map.textures[tex_num].img_data)
	{
		mlx_destroy_image(game->mlx.mlx_ptr,
			game->map.textures[tex_num].img_ptr);
		game->map.textures[tex_num].img_ptr = NULL;
		context.additional_info = "Failed to get image data address";
		error_report(ERROR_TEXTURE_LOAD, &context);
		return (-1);
	}
	// Create striped pattern texture
	y = 0;
	while (y < 64)
	{
		x = 0;
		while (x < 64)
		{
			// Create alternating pattern
			if ((x / 8 + y / 8) % 2 == 0)
				color = color_base;
			else
				color = color_base / 2; // Darker shade
			pixel_pos = y * game->map.textures[tex_num].line_length + x
				* (game->map.textures[tex_num].bits_per_pixel / 8);
			*(int *)(game->map.textures[tex_num].img_data + pixel_pos) = color;
			x++;
		}
		y++;
	}
	return (0);
}

int	load_textures(t_game *game)
{
	t_error_context	context;

	context.file_name = NULL;
	context.line_number = 0;
	context.function_name = "load_textures";
	context.additional_info = NULL;
	if (!game)
	{
		context.additional_info = "Null game pointer provided";
		error_report(ERROR_MEMORY, &context);
		return (-1);
	}
	// Create north texture (red)
	if (create_texture_image(game, 0, 0xFF0000) != 0)
	{
		context.additional_info = "Failed to create north texture";
		error_report(ERROR_TEXTURE_LOAD, &context);
		return (-1);
	}
	// Create south texture (blue)
	if (create_texture_image(game, 1, 0x0000FF) != 0)
	{
		context.additional_info = "Failed to create south texture";
		error_report(ERROR_TEXTURE_LOAD, &context);
		return (-1);
	}
	// Create west texture (green)
	if (create_texture_image(game, 2, 0x00FF00) != 0)
	{
		context.additional_info = "Failed to create west texture";
		error_report(ERROR_TEXTURE_LOAD, &context);
		return (-1);
	}
	// Create east texture (yellow)
	if (create_texture_image(game, 3, 0xFFFF00) != 0)
	{
		context.additional_info = "Failed to create east texture";
		error_report(ERROR_TEXTURE_LOAD, &context);
		return (-1);
	}
	return (0);
}

void	free_textures(t_game *game)
{
	int	i;

	if (!game || !game->mlx.mlx_ptr)
		return ;
	i = 0;
	while (i < 4)
	{
		if (game->map.textures[i].img_ptr)
		{
			mlx_destroy_image(game->mlx.mlx_ptr, game->map.textures[i].img_ptr);
			game->map.textures[i].img_ptr = NULL;
			game->map.textures[i].img_data = NULL;
		}
		i++;
	}
}

void	calculate_texture_coords(t_ray *ray, t_game *game)
{
	// Calculate wall_x (exact position where wall was hit)
	if (ray->side == 0)
		ray->wall_x = game->player.pos.y + ray->perp_wall_dist * ray->dir.y;
	else
		ray->wall_x = game->player.pos.x + ray->perp_wall_dist * ray->dir.x;
	ray->wall_x -= floor(ray->wall_x);
	// Calculate texture number based on wall direction
	if (ray->side == 0)
	{
		if (ray->dir.x > 0)
			ray->tex_num = 3; // East wall
		else
			ray->tex_num = 2; // West wall
	}
	else
	{
		if (ray->dir.y > 0)
			ray->tex_num = 1; // South wall
		else
			ray->tex_num = 0; // North wall
	}
	// Calculate x coordinate on texture
	ray->tex_x = (int)(ray->wall_x * game->map.textures[ray->tex_num].width);
	if ((ray->side == 0 && ray->dir.x > 0) || (ray->side == 1
			&& ray->dir.y < 0))
		ray->tex_x = game->map.textures[ray->tex_num].width - ray->tex_x - 1;
}

int	get_texture_pixel(t_texture *texture, int x, int y)
{
	int pixel_pos;

	if (x < 0 || y < 0 || x >= texture->width || y >= texture->height)
		return (0);
	pixel_pos = y * texture->line_length + x * (texture->bits_per_pixel / 8);
	return (*(int *)(texture->img_data + pixel_pos));
}