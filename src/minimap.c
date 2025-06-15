#include "../include/cub3d.h"

void	draw_minimap_pixel(t_game *game, int x, int y, int color)
{
	int	pixel_pos;

	if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return ;
	pixel_pos = y * game->mlx.line_length + x * (game->mlx.bits_per_pixel / 8);
	*(int *)(game->mlx.img_data + pixel_pos) = color;
}

void	draw_minimap_background(t_game *game)
{
	int x, y;
	// Draw minimap border (dark gray)
	y = MINIMAP_OFFSET_Y - 2;
	while (y < MINIMAP_OFFSET_Y + MINIMAP_HEIGHT + 2)
	{
		x = MINIMAP_OFFSET_X - 2;
		while (x < MINIMAP_OFFSET_X + MINIMAP_WIDTH + 2)
		{
			if (x == MINIMAP_OFFSET_X - 2 || x == MINIMAP_OFFSET_X
				+ MINIMAP_WIDTH + 1 || y == MINIMAP_OFFSET_Y - 2
				|| y == MINIMAP_OFFSET_Y + MINIMAP_HEIGHT + 1)
				draw_minimap_pixel(game, x, y, 0x444444);
			x++;
		}
		y++;
	}
	// Fill minimap background (light gray)
	y = MINIMAP_OFFSET_Y;
	while (y < MINIMAP_OFFSET_Y + MINIMAP_HEIGHT)
	{
		x = MINIMAP_OFFSET_X;
		while (x < MINIMAP_OFFSET_X + MINIMAP_WIDTH)
		{
			draw_minimap_pixel(game, x, y, 0xCCCCCC);
			x++;
		}
		y++;
	}
}

void	draw_minimap_walls(t_game *game)
{
	int map_x, map_y;
	int pixel_x, pixel_y;
	int x, y;
	map_y = 0;
	while (map_y < game->map.height)
	{
		map_x = 0;
		while (map_x < game->map.width)
		{
			if (game->map.grid[map_y][map_x] == '1')
			{
				// Calculate pixel position in minimap
				pixel_x = MINIMAP_OFFSET_X + (map_x * MINIMAP_SCALE);
				pixel_y = MINIMAP_OFFSET_Y + (map_y * MINIMAP_SCALE);
				// Draw wall block
				y = 0;
				while (y < MINIMAP_SCALE && pixel_y + y < MINIMAP_OFFSET_Y
					+ MINIMAP_HEIGHT)
				{
					x = 0;
					while (x < MINIMAP_SCALE && pixel_x + x < MINIMAP_OFFSET_X
						+ MINIMAP_WIDTH)
					{
						draw_minimap_pixel(game, pixel_x + x, pixel_y + y,
							0x000000);
						x++;
					}
					y++;
				}
			}
			map_x++;
		}
		map_y++;
	}
}

void	draw_minimap_player(t_game *game)
{
	int	i;

	int player_x, player_y;
	int x, y;
	int dir_x, dir_y;
	// Calculate player position in minimap
	player_x = MINIMAP_OFFSET_X + (int)(game->player.pos.x * MINIMAP_SCALE);
	player_y = MINIMAP_OFFSET_Y + (int)(game->player.pos.y * MINIMAP_SCALE);
	// Draw player as a red circle
	y = -MINIMAP_PLAYER_SIZE;
	while (y <= MINIMAP_PLAYER_SIZE)
	{
		x = -MINIMAP_PLAYER_SIZE;
		while (x <= MINIMAP_PLAYER_SIZE)
		{
			if (x * x + y * y <= MINIMAP_PLAYER_SIZE * MINIMAP_PLAYER_SIZE)
				draw_minimap_pixel(game, player_x + x, player_y + y, 0xFF0000);
			x++;
		}
		y++;
	}
	// Draw direction line
	i = 0;
	while (i < 15)
	{
		dir_x = player_x + (int)(game->player.dir.x * i);
		dir_y = player_y + (int)(game->player.dir.y * i);
		draw_minimap_pixel(game, dir_x, dir_y, 0xFF0000);
		i++;
	}
}

void	draw_minimap(t_game *game)
{
	draw_minimap_background(game);
	draw_minimap_walls(game);
	draw_minimap_player(game);
}