

# include	"minilibx-linux/mlx.h"
# include	<stddef.h> // for NULL
# include	<stdlib.h> // for free, 
# include	<stdio.h> //for perror
# include	<math.h>

# define ESC 0xff1b
# define BACKGROUND_COLOR 0x20400f
# define PLAYER_COLOR 0xFF0000
# define RAY_COLOR 0x00ff00
# define WINDOW_W 1920
# define WINDOW_H 1080
// # define MAP_W 1080
// # define MAP_H 1080


# define PI180 3.14159265359 //180
# define PI360 6.283185307
# define DEGREE 0.017453293
# define FOV 60 //1.047197551


# define UNIT_W 64
# define UNIT_H 64
# define MPOS_S 5

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

typedef struct s_player
{
	t_cp	pos;
	float	angle;
}	t_player;


typedef struct s_pm
{
	void	*mlx;
	void	*win;
	t_img	*images;
	t_img	img_scene;
	t_img	img_map;
	t_img	img_map;
	t_img	img_rays;
	t_player	player;
	// env?  H et W du window
}	t_pm;

void	my_mlx_pixel_put(t_img **img, int x, int y, int color)
{
	char	*dst;

	dst = (*img)->addr + (y * (*img)->line_len + x * ((*img)->bpp / 8));
	*(unsigned int *)dst = color;
}

int	cross(t_pm *pm)
{
	mlx_destroy_image(pm->mlx, pm->img_scene.img);
	mlx_destroy_image(pm->mlx, pm->img_map.img);
	mlx_destroy_window(pm->mlx, pm->win);
	mlx_destroy_display(pm->mlx);
	free(pm->mlx);
	exit(0);
	return (0);
}

int	esc(int keycode, t_pm *pm)
{
	if (keycode == ESC)
	{
		mlx_destroy_image(pm->mlx, pm->img_scene.img);
		mlx_destroy_image(pm->mlx, pm->img_map.img);
		mlx_destroy_window(pm->mlx, pm->win);
		mlx_destroy_display(pm->mlx);
		free(pm->mlx);
		exit(0);
	}
	return (0);
}

int	draw_scene(t_pm pm, t_img *img)
{
	t_cp		pixel;

	pixel.x = 0;
	pixel.y = 0;
	while (pixel.x < WINDOW_W)
	{
		pixel.y = 0;
		while (pixel.y < WINDOW_H)
		{
			my_mlx_pixel_put(&img, pixel.x, pixel.y, BACKGROUND_COLOR);
			pixel.y++;
		}
		pixel.x++;
	}
	mlx_put_image_to_window(pm.mlx, pm.win, img->img, 0, 0);
	return (0);
}

int	draw_player(t_pm pm, t_img *img)
{
	t_cp	t;

	t = pm.player.pos;


	t = width;
	while (length > 0)
	{
		width = tw;
		while (width > 0)
		{
			my_mlx_pixel_put(&img, width, length, color);
			width--;
		}
		length--;
	}
	return (0);
}


int	draw_map(t_pm pm)
{
	// draw cadre 2D
	// t_cp	begin;
	// int	map_w;
	// int	map_h;

	// map_w = WINDOW_W ; //* 0.5
	// map_h = WINDOW_H;
	// begin.x = WINDOW_W / 2 - map_w / 2;
	// begin.y = WINDOW_H - map_h;


	draw_player(pm, &(pm.img_map)); // en 3D


}

int	draw_ray(t_pm *pm)
{
	t_cp rv; 
	t_cp rh;
	float	dv;
	float	dh;


	rv.y = ((int)(pm->player.pos.y) >> 6 << 6)- 0.0001;
	rv.x = rv.y * atan(pm->player.angle);
	dv = rv.y / sin(pm->player.angle);

	rh.x = ((int)(pm->player.pos.x) >> 6 << 6) - 0.0001;
	rh.y = rh.x * tan(pm->player.angle);
	dh = rh.x / sin(pm->player.angle);

	draw_square(pm->img_rays.img, MPOS_S, MPOS_S, RAY_COLOR);
	if (dv > dh)
		mlx_put_image_to_window(pm->mlx, pm->win, pm->img_rays.img, (int)(rh.x), (int)(rh.y));
	else
		mlx_put_image_to_window(pm->mlx, pm->win, pm->img_rays.img, (int)(rv.x), (int)(rv.y));
	return (0);
}

// int	draw_map(t_pm *pm)
// {
// 	t_cp rv; 
// 	t_cp rh;
// 	float	dv;
// 	float	dh;


// 	rv.y = ((int)(pm->player.pos.y) >> 6 << 6)- 0.0001;
// 	rv.x = rv.y * atan(pm->player.angle);
// 	dv = rv.y / sin(pm->player.angle);

// 	rh.x = ((int)(pm->player.pos.x) >> 6 << 6) - 0.0001;
// 	rh.y = rh.x * tan(pm->player.angle);
// 	dh = rh.x / sin(pm->player.angle);

// 	draw_square(pm->img_rays.img, MPOS_S, MPOS_S, RAY_COLOR);
// 	if (dv > dh)
// 		mlx_put_image_to_window(pm->mlx, pm->win, pm->img_rays.img, (int)(rh.x), (int)(rh.y));
// 	else
// 		mlx_put_image_to_window(pm->mlx, pm->win, pm->img_rays.img, (int)(rv.x), (int)(rv.y));
// 	return (0);
// }



void	update_draw(t_pm *pm)
{
	mlx_clear_window(pm->mlx, pm->win);
	mlx_put_image_to_window(pm->mlx, pm->win, pm->img_scene.img, 0, 0);
	printf("%f - %f\n", pm->player.pos.x, pm->player.pos.y);
	mlx_put_image_to_window(pm->mlx, pm->win, pm->img_map.img, (int)(pm->player.pos.x), (int)(pm->player.pos.y));

	// t_cp r; 

	// r.y = (((int)(pm->player.pos.y) >> 6) << 6) - 0.0001;
	// r.x = r.y * atan(pm->player.angle);
	// mlx_put_image_to_window(pm->mlx, pm->win, pm->img_rays.img, (int)(r.x), (int)(r.y));

	draw_ray(pm);
}


int	move(int keycode, t_pm *pm)
{
	if (keycode == W)
	{
		pm->player.pos.y -= MPOS_S;
		if (pm->player.pos.y < 0)
			pm->player.pos.y = 0;
	}
	else if (keycode == S)
	{
		pm->player.pos.y += MPOS_S;
		if (pm->player.pos.y > WINDOW_H) //deborder pour linstant
			pm->player.pos.y = WINDOW_H;
	}
	else if (keycode == A)
	{
		pm->player.pos.x -= MPOS_S;
		if (pm->player.pos.x < 0)
			pm->player.pos.x = 0;
	}
	else if (keycode == D)
	{
		pm->player.pos.x += MPOS_S;
		if (pm->player.pos.x > WINDOW_W)
			pm->player.pos.x = WINDOW_W;
	}
	else if (keycode == LEFT)
	{
		pm->player.angle += DEGREE;
		if (pm->player.angle > PI360)
			pm->player.angle -= PI360;
	}
	else if (keycode == RIGHT)
	{
		pm->player.angle -= DEGREE;
		if (pm->player.angle < 0)
			pm->player.angle += PI360;
	}
	//printf("%f - %f \n", pm->player.pos.x, pm->player.pos.y);
	update_draw(pm);
	return (0);
}


// void	drawing(t_pm pm, t_img *img)
// {
// 	//t_cp	t;

// 	draw_scene(pm, &(pm.img_scene));
// 	draw_square(pm, &(pm.img));
// 	//draw_map(pm, &(pm.img));
// 	//draw_rays(pm, &(pm.img));
// }




int	main(void)
{
	t_pm	pm;

	//parsing(ac, ags, &pm); // A R
	pm.mlx = mlx_init();
	if (pm.mlx == NULL)
	{
		perror("mlx_init() failed");
		exit(1);
	}		
	
	
	// temp - vient du parsing - le map
	pm.player.pos.x = 300;
	pm.player.pos.y = 300;
	pm.player.angle = DEGREE * 179;

	pm.win = mlx_new_window(pm.mlx, WINDOW_W, WINDOW_H, "CUB3D");
	pm.img_scene.img = mlx_new_image(pm.mlx, WINDOW_W, WINDOW_H);
	pm.img_scene.addr = mlx_get_data_addr(pm.img_scene.img, &pm.img_scene.bpp, &pm.img_scene.line_len,
			&pm.img_scene.endian);

	// pm.img_map.img = mlx_new_image(pm.mlx, 800, 800); //AM
	// pm.img_map.addr = mlx_get_data_addr(pm.img_map.img, &pm.img_map.bpp, &pm.img_map.line_len,
	// 		&pm.img_map.endian);


	draw_scene(pm, &(pm.img_scene));
	// draw_map(pm); // pos player sera donne par parsing
	update_draw(&pm);
	//mlx_mouse_hook(pm.win, &mouse_zoom, &pm);
	mlx_hook(pm.win, 17, 0, &cross, &pm);
	mlx_hook(pm.win, 2, 1L << 0, &esc, &pm);
	mlx_hook(pm.win, 3, 1L << 1, &move, &pm);
	mlx_loop(pm.mlx);
}
