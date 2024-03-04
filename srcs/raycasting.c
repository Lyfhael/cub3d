#include "parsing.h"
#include "raycasting.h"

void	my_mlx_pixel_put(t_img **img, int x, int y, int color)
{
	char	*dst;

	dst = (*img)->addr + (y * (*img)->line_len + x * ((*img)->bpp / 8));
	*(unsigned int *)dst = color;
}

int	move_key_pressed(int keycode, t_c3d *c3d)
{
	(void)c3d;
	//printf("key: %d \n",keycode);
	if (keycode == W)
	{
		;
	}
	else if (keycode == S)
	{
		;
	}
	else if (keycode == A)
	{
		;
	}	
	else if (keycode == D)
	{
		;
	}
	else if (keycode == LEFT)
	{
		;
	}
	else if (keycode == RIGHT)
	{
		;
	}
	else
		return (0);
	return (0);
}

int	esc(int keycode, t_c3d *c3d)
{
	(void)c3d;
	(void)keycode;
	exit(1);
}

void	ray_casting(t_c3d *c3d)
{
	//init window
	c3d->mlx = mlx_init();	
	c3d->win = mlx_new_window(c3d->mlx, WINDOW_W, WINDOW_H, "CUB3D");

	//mlx_mouse_hook(c3d->win, &mouse_zoom, &c3d);
	mlx_hook(c3d->win, 17, 0, &esc, c3d);
	mlx_hook(c3d->win, 2, 1L << 0, &esc, c3d);
	mlx_hook(c3d->win, 3, 1L << 1, &move_key_pressed, c3d);
	mlx_loop(c3d->mlx);
}
