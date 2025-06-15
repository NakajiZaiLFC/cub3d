#include "../include/cub3d.h"

void	init_ray(t_ray *ray, t_game *game, int x)
{
	calculate_ray_direction(ray, game, x);
	// Calculate delta distances
	if (ray->dir.x == 0)
		ray->delta_dist.x = 1e30;
	else
		ray->delta_dist.x = fabs(1.0 / ray->dir.x);
	if (ray->dir.y == 0)
		ray->delta_dist.y = 1e30;
	else
		ray->delta_dist.y = fabs(1.0 / ray->dir.y);
	// Current map position
	ray->map_x = (int)game->player.pos.x;
	ray->map_y = (int)game->player.pos.y;
	// Calculate step and initial side_dist
	if (ray->dir.x < 0)
	{
		ray->step_x = -1;
		ray->side_dist.x = (game->player.pos.x - ray->map_x)
			* ray->delta_dist.x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist.x = (ray->map_x + 1.0 - game->player.pos.x)
			* ray->delta_dist.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step_y = -1;
		ray->side_dist.y = (game->player.pos.y - ray->map_y)
			* ray->delta_dist.y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist.y = (ray->map_y + 1.0 - game->player.pos.y)
			* ray->delta_dist.y;
	}
	ray->hit = 0;
	ray->side = 0;
}

void	calculate_ray_direction(t_ray *ray, t_game *game, int x)
{
	double	camera_x;

	// Camera x coordinate (from -1 to 1, left to right)
	camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
	// Ray direction vector
	ray->dir.x = game->player.dir.x + game->player.plane.x * camera_x;
	ray->dir.y = game->player.dir.y + game->player.plane.y * camera_x;
}

void	perform_dda(t_ray *ray, t_game *game)
{
	// Perform DDA algorithm
	while (ray->hit == 0)
	{
		// Jump to next map square, either in x-direction, or in y-direction
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->map_x += ray->step_x;
			ray->side = 0; // X-side hit
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->map_y += ray->step_y;
			ray->side = 1; // Y-side hit
		}
		// Check if ray has hit a wall
		if (ray->map_x >= 0 && ray->map_x < game->map.width && ray->map_y >= 0
			&& ray->map_y < game->map.height
			&& game->map.grid[ray->map_y][ray->map_x] == '1')
		{
			ray->hit = 1;
		}
		// Safety check to prevent infinite loops
		if (ray->map_x < 0 || ray->map_x >= game->map.width || ray->map_y < 0
			|| ray->map_y >= game->map.height)
		{
			ray->hit = 1;
		}
	}
}

void	calculate_wall_distance(t_ray *ray, t_game *game)
{
	// Calculate perpendicular distance to avoid fisheye effect
	if (ray->side == 0)
	{
		ray->perp_wall_dist = (ray->map_x - game->player.pos.x + (1
					- ray->step_x) / 2) / ray->dir.x;
	}
	else
	{
		ray->perp_wall_dist = (ray->map_y - game->player.pos.y + (1
					- ray->step_y) / 2) / ray->dir.y;
	}
}

void	calculate_wall_height(t_ray *ray, t_game *game)
{
	(void)game; // Suppress unused parameter warning
	// Calculate height of line to draw on screen
	if (ray->perp_wall_dist == 0)
		ray->line_height = SCREEN_HEIGHT;
	else
		ray->line_height = (int)(SCREEN_HEIGHT / ray->perp_wall_dist);
	// Calculate lowest and highest pixel to fill in current stripe
	ray->draw_start = -ray->line_height / 2 + SCREEN_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + SCREEN_HEIGHT / 2;
	if (ray->draw_end >= SCREEN_HEIGHT)
		ray->draw_end = SCREEN_HEIGHT - 1;
	// Calculate texture coordinates
	calculate_texture_coords(ray, game);
}

void	cast_rays(t_game *game)
{
	t_ray ray;
	int x;

	x = 0;
	while (x < SCREEN_WIDTH)
	{
		// Initialize and cast ray for current column
		init_ray(&ray, game, x);
		perform_dda(&ray, game);
		calculate_wall_distance(&ray, game);
		calculate_wall_height(&ray, game);

		// Placeholder for basic wall rendering (will be expanded in next tasks)

		x++;
	}
}