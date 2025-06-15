#ifndef ERROR_TYPES_H
# define ERROR_TYPES_H

/* Error types */
typedef enum e_error_type
{
	ERROR_NONE = 0,
	ERROR_MEMORY,
	ERROR_FILE_NOT_FOUND,
	ERROR_FILE_INVALID,
	ERROR_TEXTURE_LOAD,
	ERROR_TEXTURE_FORMAT,
	ERROR_MAP_INVALID,
	ERROR_MAP_NOT_CLOSED,
	ERROR_MAP_PLAYER_MISSING,
	ERROR_MAP_PLAYER_MULTIPLE,
	ERROR_MAP_INVALID_CHAR,
	ERROR_CONFIG_INVALID,
	ERROR_CONFIG_MISSING_ELEMENT,
	ERROR_CONFIG_DUPLICATE,
	ERROR_RGB_INVALID,
	ERROR_MLX_INIT,
	ERROR_ARGS_INVALID
}	t_error_type;

/* Error context structure */
typedef struct s_error_context
{
	const char	*file_name;
	int			line_number;
	const char	*function_name;
	const char	*additional_info;
}	t_error_context;

/* Error exit codes */
# define EXIT_SUCCESS 0
# define EXIT_ERROR_ARGS 1
# define EXIT_ERROR_FILE 2
# define EXIT_ERROR_CONFIG 3
# define EXIT_ERROR_MEMORY 4
# define EXIT_ERROR_MLX 5

#endif 