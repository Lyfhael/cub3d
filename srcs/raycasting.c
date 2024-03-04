#include "parsing.h"
#include "raycasting.h"

void	my_mlx_pixel_put(t_img **img, int x, int y, int color)
{
	char	*dst;

	dst = (*img)->addr + (y * (*img)->line_len + x * ((*img)->bpp / 8));
	*(unsigned int *)dst = color;
}

int	move(int keycode, t_pm *pm) // to add 2D; to stop by walls; 
{
	(void)pm;
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

int	esc(int keycode, t_pm *pm)
{
	(void)pm;
	(void)keycode;
	exit(1);
}

void	ray_casting(t_pm *pm)
{
	//init window
	pm->mlx = mlx_init();	
	pm->win = mlx_new_window(pm->mlx, WINDOW_W, WINDOW_H, "CUB3D");

	//mlx_mouse_hook(pm->win, &mouse_zoom, &pm);
	mlx_hook(pm->win, 17, 0, &esc, pm);
	mlx_hook(pm->win, 2, 1L << 0, &esc, pm);
	mlx_hook(pm->win, 3, 1L << 1, &move, pm);
	mlx_loop(pm->mlx);
}
