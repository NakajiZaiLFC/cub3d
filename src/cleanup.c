#include "../include/cub3d.h"

static void	cleanup_map_data(t_game *game)
{
	int	i;

	if (!game)
		return ;
	printf("DEBUG: Starting map data cleanup\n");
	// Free map grid
	if (game->map.grid)
	{
		printf("DEBUG: Freeing map grid (height: %d)\n", game->map.height);
		i = 0;
		while (i < game->map.height && game->map.grid[i])
		{
			printf("DEBUG: Freeing grid[%d]: %p\n", i, game->map.grid[i]);
			free(game->map.grid[i]);
			game->map.grid[i] = NULL;
			i++;
		}
		printf("DEBUG: Freeing grid array: %p\n", game->map.grid);
		free(game->map.grid);
		game->map.grid = NULL;
	}
	// Free texture paths
	if (game->map.no_texture)
	{
		printf("DEBUG: Freeing no_texture: %p\n", game->map.no_texture);
		free(game->map.no_texture);
		game->map.no_texture = NULL;
	}
	if (game->map.so_texture)
	{
		printf("DEBUG: Freeing so_texture: %p\n", game->map.so_texture);
		free(game->map.so_texture);
		game->map.so_texture = NULL;
	}
	if (game->map.we_texture)
	{
		printf("DEBUG: Freeing we_texture: %p\n", game->map.we_texture);
		free(game->map.we_texture);
		game->map.we_texture = NULL;
	}
	if (game->map.ea_texture)
	{
		printf("DEBUG: Freeing ea_texture: %p\n", game->map.ea_texture);
		free(game->map.ea_texture);
		game->map.ea_texture = NULL;
	}
	printf("DEBUG: Map data cleanup completed\n");
}

static void	cleanup_mlx_resources(t_game *game)
{
	if (!game)
		return ;
	printf("DEBUG: Starting MLX resources cleanup\n");
	// Free textures first (they depend on mlx_ptr)
	if (game->mlx.mlx_ptr)
	{
		printf("DEBUG: Calling free_textures\n");
		free_textures(game);
	}
	// Free current frame image
	if (game->mlx.img_ptr && game->mlx.mlx_ptr)
	{
		printf("DEBUG: Destroying frame image: %p\n", game->mlx.img_ptr);
		mlx_destroy_image(game->mlx.mlx_ptr, game->mlx.img_ptr);
		game->mlx.img_ptr = NULL;
		game->mlx.img_data = NULL;
	}
	// Free window
	if (game->mlx.win_ptr && game->mlx.mlx_ptr)
	{
		printf("DEBUG: Destroying window: %p\n", game->mlx.win_ptr);
		mlx_destroy_window(game->mlx.mlx_ptr, game->mlx.win_ptr);
		game->mlx.win_ptr = NULL;
	}
	// Free MLX display and connection
	if (game->mlx.mlx_ptr)
	{
		printf("DEBUG: Destroying MLX display and connection: %p\n",
			game->mlx.mlx_ptr);
		mlx_destroy_display(game->mlx.mlx_ptr);
		free(game->mlx.mlx_ptr);
		game->mlx.mlx_ptr = NULL;
	}
	printf("DEBUG: MLX resources cleanup completed\n");
}

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	printf("Cleaning up game resources...\n");
	// Clean up map data and texture paths
	cleanup_map_data(game);
	// Clean up MLX resources
	cleanup_mlx_resources(game);
	printf("Game cleanup completed.\n");
}

int	clean_exit(t_game *game, int exit_code)
{
	printf("Initiating clean exit with code %d...\n", exit_code);
	if (game)
		cleanup_game(game);
	printf("Exiting program.\n");
	exit(exit_code);
}

int	emergency_exit(t_game *game, int exit_code)
{
	printf("EMERGENCY EXIT: code %d\n", exit_code);
	// Emergency exit with minimal cleanup to avoid further errors
	if (game && game->mlx.mlx_ptr)
	{
		if (game->mlx.win_ptr)
		{
			printf("EMERGENCY: Destroying window\n");
			mlx_destroy_window(game->mlx.mlx_ptr, game->mlx.win_ptr);
		}
		printf("EMERGENCY: Destroying display\n");
		mlx_destroy_display(game->mlx.mlx_ptr);
		printf("EMERGENCY: Freeing MLX ptr\n");
		free(game->mlx.mlx_ptr);
	}
	printf("EMERGENCY EXIT completed\n");
	exit(exit_code);
}