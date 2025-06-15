#ifndef CONSTANTS_H
# define CONSTANTS_H

/* Window settings */
# define WIN_WIDTH 1024
# define WIN_HEIGHT 768
# define WIN_TITLE "cub3D"

/* Screen settings for raycasting */
# define SCREEN_WIDTH 1024
# define SCREEN_HEIGHT 768

/* Key codes (Linux) */
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_LEFT 65361
# define KEY_RIGHT 65363

/* Map settings */
# define TEX_SIZE 64

/* Game constants */
# define PI 3.14159265359
# define FOV 1.0472  // 60 degrees in radians

/* Movement speed */
# define MOVE_SPEED 0.1
# define ROT_SPEED 0.1

/* Minimap settings */
# define MINIMAP_WIDTH 200
# define MINIMAP_HEIGHT 200
# define MINIMAP_SCALE 25
# define MINIMAP_OFFSET_X 10
# define MINIMAP_OFFSET_Y 10
# define MINIMAP_PLAYER_SIZE 5

/* Performance settings */
# define TARGET_FPS 60
# define FRAME_TIME_USEC (1000000 / TARGET_FPS)

#endif 