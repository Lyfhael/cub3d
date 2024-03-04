#include "parsing.h"
#include "raycasting.h"

void	my_mlx_pixel_put(t_img **img, int x, int y, int color)
{
	char	*dst;

	dst = (*img)->addr + (y * (*img)->line_len + x * ((*img)->bpp / 8));
	*(unsigned int *)dst = color;
}

int	cross(t_pm *pm)
{
	mlx_destroy_image(pm->mlx, pm->img_scene.img);
	//mlx_destroy_image(pm->mlx, pm->img_map.img);
	mlx_destroy_window(pm->mlx, pm->win);
	mlx_destroy_display(pm->mlx);
	free(pm->mlx);
	_c3d_free_t_pm(pm);
	exit(0);
	return (0);
}

int	esc(int keycode, t_pm *pm)
{
	if (keycode == ESC)
	{
		mlx_destroy_image(pm->mlx, pm->img_scene.img);
		//mlx_destroy_image(pm->mlx, pm->img_map.img);
		mlx_destroy_window(pm->mlx, pm->win);
		mlx_destroy_display(pm->mlx);
		free(pm->mlx);
		_c3d_free_t_pm(pm);
		exit(0);
	}
	return (0);
}

void	draw_ground(t_img *img) // for scene now
{
	t_cp		pixel;

	pixel.x = 0;
	pixel.y = 0;
	while (pixel.y < WINDOW_H)
	{
		pixel.x = 0;
		while (pixel.x < WINDOW_W)
		{
			my_mlx_pixel_put(&img, pixel.x, pixel.y, BACKGROUND_COLOR);
			pixel.x++;
		}
		pixel.y++;
	}
}

void	draw_point(t_img *img, t_cp pos, int color)
{
	t_cp	i;

	i.y = 0;
	while (i.y < SQUARE_S && pos.y + i.y < WINDOW_H)
	{
		i.x = 0;
		while (i.x < SQUARE_S && pos.x + i.x < WINDOW_W)
		{
			my_mlx_pixel_put(&img, pos.x + i.x, pos.y + i.y, color);
			i.x++;
		}
		i.y++;
	}
}

void	do_drawing(t_pm *pm)
{
	// a voir combiner les trois fcts
	//draw_ground(&(pm->img_scene)); // later for 2D only
	//draw_point(&(pm->img_scene), pm->player.pos, PLAYER_COLOR);
	//draw_point(&(pm->img_scene), pm->player.dir, DIRECT_COLOR);
	draw_view(pm);
	mlx_put_image_to_window(pm->mlx, pm->win, pm->img_scene.img, 0, 0);

	//do for 2D // draw_map(pm); // pos player sera donne par parsing
}

//int	draw_map(t_pm pm)
// {
// 	int	map_w;
// 	int	map_h;
// 	t_cp begin;

// 	map_w = WINDOW_W * 0.2;
// 	map_h = WINDOW_H * 0.2;
// 	begin.x = WINDOW_W / 2 - map_w / 2;
// 	begin.y = WINDOW_H - map_h;

// 	//draw_map_ground(); 
// 	//draw_map_player();
// }

int	move(int keycode, t_pm *pm) // to add 2D; to stop by walls; 
{
	//printf("key: %d \n",keycode);
	if (keycode == W)
	{
		pm->player.pos.x += cos(pm->player.angle) * DELTA;
		pm->player.pos.y -= sin(pm->player.angle) * DELTA;
		pm->player.dir.x = pm->player.pos.x + cos(pm->player.angle) * DELTA;
		pm->player.dir.y = pm->player.pos.y - sin(pm->player.angle) * DELTA;
	}
	else if (keycode == S)
	{
		pm->player.pos.x -= cos(pm->player.angle) * DELTA;
		pm->player.pos.y += sin(pm->player.angle) * DELTA;
		pm->player.dir.x = pm->player.pos.x + cos(pm->player.angle) * DELTA;
		pm->player.dir.y = pm->player.pos.y - sin(pm->player.angle) * DELTA;
	}
	else if (keycode == A)
	{
		pm->player.pos.x -= sin(pm->player.angle) * DELTA;
		pm->player.pos.y -= cos(pm->player.angle) * DELTA;
		pm->player.dir.x = pm->player.pos.x + cos(pm->player.angle) * DELTA;
		pm->player.dir.y = pm->player.pos.y - sin(pm->player.angle) * DELTA;
	}	
	else if (keycode == D)
	{
		pm->player.pos.x += sin(pm->player.angle) * DELTA;
		pm->player.pos.y += cos(pm->player.angle) * DELTA;
		pm->player.dir.x = pm->player.pos.x + cos(pm->player.angle) * DELTA;
		pm->player.dir.y = pm->player.pos.y - sin(pm->player.angle) * DELTA;
	}
	else if (keycode == LEFT)
	{
		pm->player.angle += DEGREE * TURN_ANGLE;
		correct_angle(&pm->player.angle);
		pm->player.dir.x = pm->player.pos.x + cos(pm->player.angle) * DELTA;
		pm->player.dir.y = pm->player.pos.y - sin(pm->player.angle) * DELTA;
	}
	else if (keycode == RIGHT)
	{
		pm->player.angle -= DEGREE * TURN_ANGLE;
		correct_angle(&pm->player.angle);
		pm->player.dir.x = pm->player.pos.x + cos(pm->player.angle) * DELTA;
		pm->player.dir.y = pm->player.pos.y - sin(pm->player.angle) * DELTA;
	}
	else
		return (0);

	do_drawing(pm);
	return (0);
}

void set_pm(t_pm *pm)
{
   //temp
	// pm->player.pos_map.x = 3;
	// pm->player.pos_map.y = 3;
	// pm->player.angle = 0; // NORD

	pm->player.pos.x = pm->player.pos_map.x * UNIT_W;
	pm->player.pos.y = pm->player.pos_map.y * UNIT_H;
	pm->player.dir.x = pm->player.pos.x + cos(pm->player.angle) * DELTA;
	pm->player.dir.y = pm->player.pos.y - sin(pm->player.angle) * DELTA;
	
	// init img scene
	pm->img_scene.img = mlx_new_image(pm->mlx, WINDOW_W, WINDOW_H);
	pm->img_scene.addr = mlx_get_data_addr(pm->img_scene.img, &(pm->img_scene.bpp), &(pm->img_scene.line_len),
			&(pm->img_scene.endian));

	//// init img map
	// pm.img_map.img = mlx_new_image(pm.mlx, 800, 800); //AM
	// pm.img_map.addr = mlx_get_data_addr(pm.img_map.img, &pm.img_map.bpp, &pm.img_map.line_len,
	// 		&pm.img_map.endian);

}

void	ray_casting(t_pm *pm)
{
	//init window
	pm->mlx = mlx_init();
	if (pm->mlx == NULL)
	{
		perror("mlx_init() failed");
		exit(1);
	}		
	pm->win = mlx_new_window(pm->mlx, WINDOW_W, WINDOW_H, "CUB3D");

	set_pm(pm);
	do_drawing(pm);

	//mlx_mouse_hook(pm->win, &mouse_zoom, &pm);
	mlx_hook(pm->win, 17, 0, &cross, pm);
	mlx_hook(pm->win, 2, 1L << 0, &esc, pm);
	mlx_hook(pm->win, 3, 1L << 1, &move, pm);
	mlx_loop(pm->mlx);
}
