#include "../include/cub3d.h"

void	error_exit(const char *message)
{
	if (message)
	{
		write(STDERR_FILENO, "Error\n", 6);
		write(STDERR_FILENO, message, strlen(message));
		write(STDERR_FILENO, "\n", 1);
	}
	exit(EXIT_FAILURE);
}

const char	*get_error_message(t_error_type error_type)
{
	static const char	*error_messages[] = {"No error",
			"Memory allocation failed", "File not found", "Invalid file format",
			"Failed to load texture", "Invalid texture format", "Invalid map",
			"Map is not properly enclosed by walls",
			"No player starting position found in map",
			"Multiple player starting positions found in map",
			"Invalid character found in map", "Invalid configuration file",
			"Missing required configuration element",
			"Duplicate configuration element", "Invalid RGB color values",
			"Failed to initialize MLX", "Invalid command line arguments"};

	if (error_type >= 0 && error_type < sizeof(error_messages)
		/ sizeof(error_messages[0]))
		return (error_messages[error_type]);
	return ("Unknown error");
}

int	get_exit_code(t_error_type error_type)
{
	switch (error_type)
	{
	case ERROR_ARGS_INVALID:
		return (EXIT_ERROR_ARGS);
	case ERROR_FILE_NOT_FOUND:
	case ERROR_FILE_INVALID:
	case ERROR_TEXTURE_LOAD:
	case ERROR_TEXTURE_FORMAT:
		return (EXIT_ERROR_FILE);
	case ERROR_CONFIG_INVALID:
	case ERROR_CONFIG_MISSING_ELEMENT:
	case ERROR_CONFIG_DUPLICATE:
	case ERROR_RGB_INVALID:
	case ERROR_MAP_INVALID:
	case ERROR_MAP_NOT_CLOSED:
	case ERROR_MAP_PLAYER_MISSING:
	case ERROR_MAP_PLAYER_MULTIPLE:
	case ERROR_MAP_INVALID_CHAR:
		return (EXIT_ERROR_CONFIG);
	case ERROR_MEMORY:
		return (EXIT_ERROR_MEMORY);
	case ERROR_MLX_INIT:
		return (EXIT_ERROR_MLX);
	default:
		return (EXIT_FAILURE);
	}
}

void	log_error(t_error_type error_type, t_error_context *context)
{
	char	line_str[32];

	write(STDERR_FILENO, "Error\n", 6);
	write(STDERR_FILENO, get_error_message(error_type),
		strlen(get_error_message(error_type)));
	if (context)
	{
		if (context->file_name)
		{
			write(STDERR_FILENO, " in file: ", 10);
			write(STDERR_FILENO, context->file_name,
				strlen(context->file_name));
		}
		if (context->line_number > 0)
		{
			sprintf(line_str, " at line %d", context->line_number);
			write(STDERR_FILENO, line_str, strlen(line_str));
		}
		if (context->function_name)
		{
			write(STDERR_FILENO, " in function: ", 14);
			write(STDERR_FILENO, context->function_name,
				strlen(context->function_name));
		}
		if (context->additional_info)
		{
			write(STDERR_FILENO, " - ", 3);
			write(STDERR_FILENO, context->additional_info,
				strlen(context->additional_info));
		}
	}
	write(STDERR_FILENO, "\n", 1);
}

int	error_report(t_error_type error_type, t_error_context *context)
{
	log_error(error_type, context);
	return (get_exit_code(error_type));
}

int	error_report_simple(t_error_type error_type, const char *message)
{
	t_error_context context;

	context.file_name = NULL;
	context.line_number = 0;
	context.function_name = NULL;
	context.additional_info = message;

	return (error_report(error_type, &context));
}