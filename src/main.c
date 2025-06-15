#include "../include/cub3d.h"
#include <signal.h>

static t_game	*g_game_instance = NULL;

void	signal_handler(int sig)
{
	printf("\nSignal %d received - performing emergency cleanup\n", sig);
	if (g_game_instance)
		emergency_exit(g_game_instance, EXIT_FAILURE);
	else
		exit(EXIT_FAILURE);
}

void	setup_signal_handlers(void)
{
	signal(SIGINT, signal_handler);  // Ctrl+C
	signal(SIGTERM, signal_handler); // Termination request
	signal(SIGSEGV, signal_handler); // Segmentation fault
	signal(SIGABRT, signal_handler); // Abort signal
}

#ifdef DEBUG
static int		g_alloc_count = 0;

void	*debug_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr)
	{
		g_alloc_count++;
		printf("DEBUG: malloc(%zu) = %p [count: %d]\n", size, ptr,
			g_alloc_count);
	}
	return (ptr);
}

void	debug_free(void *ptr)
{
	if (ptr)
	{
		g_alloc_count--;
		printf("DEBUG: free(%p) [count: %d]\n", ptr, g_alloc_count);
		free(ptr);
	}
}

void	check_memory_leaks(void)
{
	if (g_alloc_count > 0)
		printf("MEMORY LEAK DETECTED: %d allocations not freed\n",
			g_alloc_count);
	else
		printf("No memory leaks detected\n");
}
#endif

int	main(int argc, char **argv)
{
	t_game game;
	t_error_context context;
	int exit_code;

	context.file_name = NULL;
	context.line_number = 0;
	context.function_name = "main";
	context.additional_info = NULL;

	printf("Starting cub3D...\n");

	// Set up signal handlers first
	setup_signal_handlers();

	if (argc != 2)
	{
		context.additional_info = "Usage: ./cub3D <map.cub>";
		exit_code = error_report(ERROR_ARGS_INVALID, &context);
		return (exit_code);
	}

	if (init_game(&game) != 0)
	{
		context.additional_info = "Failed to initialize game";
		exit_code = error_report(ERROR_MEMORY, &context);
		return (exit_code);
	}

	// Set global game instance for signal handler AFTER successful initialization
	g_game_instance = &game;

	if (parse_config_file(argv[1], &game) != 0)
	{
		g_game_instance = NULL;
		clean_exit(&game, EXIT_ERROR_CONFIG);
	}

	if (init_mlx(&game) != 0)
	{
		context.additional_info = "Failed to initialize MLX";
		exit_code = error_report(ERROR_MLX_INIT, &context);
		g_game_instance = NULL;
		clean_exit(&game, exit_code);
	}

	if (load_textures(&game) != 0)
	{
		context.additional_info = "Failed to load textures";
		exit_code = error_report(ERROR_TEXTURE_LOAD, &context);
		g_game_instance = NULL;
		clean_exit(&game, exit_code);
	}

	printf("Game initialized successfully. Starting main loop...\n");

	mlx_hook(game.mlx.win_ptr, 2, 1L << 0, handle_keypress, &game);
	mlx_hook(game.mlx.win_ptr, 17, 1L << 17, handle_close, &game);
	mlx_loop_hook(game.mlx.mlx_ptr, render_frame, &game);

	mlx_loop(game.mlx.mlx_ptr);

	// This point should never be reached due to clean_exit in event handlers
	g_game_instance = NULL;
	clean_exit(&game, EXIT_SUCCESS);

#ifdef DEBUG
	check_memory_leaks();
#endif

	return (0);
}