#ifndef RAYCASTING_H
# define RAYCASTING_H

# include	"mlx.h"
# include	<stddef.h> // for NULL
# include	<stdlib.h> // for free, 
# include	<stdio.h> //for perror
# include	<math.h>

//new
# define CEILING_C 0xe6e6ff
# define FLOOR_C 0x000033

# define ESC 0xff1b
# define BACKGROUND_COLOR 0x20400f
# define PLAYER_COLOR 0xFF0000
# define DIRECT_COLOR 0x0000FF
# define H_COLOR 0x0000FF
# define W_COLOR 0x00ffFF
# define RAY_COLOR 0x00FF00
# define WINDOW_W 600 //1920
# define WINDOW_H 400 // 1080
# define MAX_VIEW 900
// # define MAP_W 1080
// # define MAP_H 1080

# define PI 3.14159f
# define PI90 1.57079f
# define PI270 4.71238f
# define PI360 6.28318f
# define DEGREE 0.01745f
# define FOV 150 //new
# define CDIST 64
# define TURN_ANGLE 30

# define UNIT_W 64
# define UNIT_H 64
# define SQUARE_S 5
# define DELTA 8
# define MPOS_S 5

typedef int		t_rgb;

enum e_keycodes
{
	A = 97,
	S = 115,
	W = 119,
	D = 100,
	LEFT = 65361,
	RIGHT = 65363
};

typedef struct s_cp{
	float	x;
	float	y;
}	t_cp;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_img;

typedef struct s_player //from parsing
{
	t_cp	pos_map; //from parsing
	t_cp	pos; // transform a pixel
	t_cp	dir;
	float	angle; //from parsing
}	t_player;

typedef struct s_vec
{
	int	h;
	int	w;
}		t_vec;

typedef struct s_textures
{
	char		*no_wall;
	char		*so_wall;
	char		*we_wall;
	char		*ea_wall;
	t_rgb		*floor;
	t_rgb		*ceiling;
}				t_textures;

typedef struct s_map
{
	char		**map;
	t_vec		map_size;
	t_textures	textures;
}				t_map;

typedef struct s_pm
{
	void		*mlx;
	void		*win;
	t_img		img_scene;
	t_img		img_map;
	t_player	player;
	t_map		map;
	// env?  H et W du window
}				t_pm;


float	get_dist(t_pm *pm, int *color, float angle);
void	draw_line(t_img *img, float	h_line, int w_line, int fov, int color);
void correct_angle(float *angle);
void update_line(t_pm *pm, float *h_line, float *start, int *fov, int *color);

void	ray_casting(t_pm *pm);
void	draw_view(t_pm *pm);
void	my_mlx_pixel_put(t_img **img, int x, int y, int color);
int		cross(t_pm *pm);
int		esc(int keycode, t_pm *pm);
void	draw_ground(t_img *img);
void	draw_point(t_img *img, t_cp pos, int color);
int		move(int keycode, t_pm *pm);
void	set_pm(t_pm *pm);

#endif