#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <sys/time.h>
# include "../minilibx/mlx.h"
# include "structures.h"
# include "constants.h"
# include "error_types.h"

/* Function prototypes */

/* Initialization */
int		init_game(t_game *game);
int		init_mlx(t_game *game);

/* Configuration file parsing */
int		parse_config_file(const char *filename, t_game *game);
int		parse_texture_line(char *line, t_game *game);
int		parse_color_line(char *line, t_game *game);
int		parse_rgb_values(char *rgb_str, t_rgb *color);
int		is_map_line(char *line);
int		parse_map_data(int fd, t_game *game);
int		parse_map_data_with_first_line(int fd, t_game *game, char *first_line);
int		read_line(int fd, char **line);

/* Map validation */
int		validate_map(t_game *game);
int		validate_map_characters(t_game *game);
int		validate_player_position(t_game *game);
int		validate_map_borders(t_game *game);
int		flood_fill_check(t_game *game, int start_x, int start_y);
void	init_player_from_map(t_game *game, int x, int y, char direction);

/* Raycasting */
void	init_ray(t_ray *ray, t_game *game, int x);
void	calculate_ray_direction(t_ray *ray, t_game *game, int x);
void	perform_dda(t_ray *ray, t_game *game);
void	calculate_wall_distance(t_ray *ray, t_game *game);
void	calculate_wall_height(t_ray *ray, t_game *game);
void	cast_rays(t_game *game);

/* Utility functions */
char	*ft_strdup(const char *s);
char	*ft_strtrim(char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_split(char const *s, char c);
void	free_split(char **split);

/* Event handling */
int		handle_keypress(int keycode, t_game *game);
int		handle_close(t_game *game);

/* Rendering */
int		render_frame(t_game *game);

/* Cleanup */
void	cleanup_game(t_game *game);
int		clean_exit(t_game *game, int exit_code);
int		emergency_exit(t_game *game, int exit_code);

/* Error handling */
void	error_exit(const char *message);
int		error_report(t_error_type error_type, t_error_context *context);
int		error_report_simple(t_error_type error_type, const char *message);
const char	*get_error_message(t_error_type error_type);
int		get_exit_code(t_error_type error_type);
void	log_error(t_error_type error_type, t_error_context *context);

/* Player movement */
void	move_forward(t_game *game);
void	move_backward(t_game *game);
void	move_left(t_game *game);
void	move_right(t_game *game);
void	rotate_left(t_game *game);
void	rotate_right(t_game *game);

/* Texture handling */
int		create_texture_image(t_game *game, int tex_num, int color_base);
int		load_textures(t_game *game);
void	free_textures(t_game *game);
void	calculate_texture_coords(t_ray *ray, t_game *game);
int		get_texture_pixel(t_texture *texture, int x, int y);

/* Minimap */
void	draw_minimap(t_game *game);
void	draw_minimap_background(t_game *game);
void	draw_minimap_walls(t_game *game);
void	draw_minimap_player(t_game *game);
void	draw_minimap_pixel(t_game *game, int x, int y, int color);

/* Performance */
long	get_time_usec(void);
void	limit_framerate(t_game *game);

#endif 