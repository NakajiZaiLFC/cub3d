#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <stdint.h>

/* RGB color structure */
typedef struct s_rgb
{
	int	r;
	int	g;
	int	b;
}		t_rgb;

/* Texture structure */
typedef struct s_texture
{
	void	*img_ptr;
	char	*img_data;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}		t_texture;

/* Vector structure */
typedef struct s_vector
{
	double	x;
	double	y;
}		t_vector;

/* Player structure */
typedef struct s_player
{
	t_vector	pos;
	t_vector	dir;
	t_vector	plane;
}		t_player;

/* Map structure */
typedef struct s_map
{
	char		**grid;
	int			width;
	int			height;
	char		*no_texture;
	char		*so_texture;
	char		*we_texture;
	char		*ea_texture;
	t_texture	textures[4];
	t_rgb		floor_color;
	t_rgb		ceil_color;
}		t_map;

/* Ray structure */
typedef struct s_ray
{
	t_vector	dir;
	t_vector	delta_dist;
	t_vector	side_dist;
	int			map_x;
	int			map_y;
	int			step_x;
	int			step_y;
	int			hit;
	int			side;
	double		perp_wall_dist;
	int			line_height;
	int			draw_start;
	int			draw_end;
	double		wall_x;
	int			tex_x;
	int			tex_num;
}		t_ray;

/* MLX data structure */
typedef struct s_mlx_data
{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img_ptr;
	char	*img_data;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}		t_mlx_data;

/* Main game structure */
typedef struct s_game
{
	t_map		map;
	t_player	player;
	t_mlx_data	mlx;
	int			win_width;
	int			win_height;
	long		last_frame_time;
}		t_game;

#endif 