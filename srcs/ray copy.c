#include "parsing.h"
#include "raycasting.h"

//temp
#define WALL 49
static bool	is_obstacle(t_cp pos, char **map)
{
    int    x;
    int    y;

    x = pos.x / UNIT_W;
    y = pos.y / UNIT_H;
	printf("check in obst x : %d, y : %d \n", x, y);
	printf("map : %d\n", map[y][x] );
    if (map[y][x] == WALL)
        return (true);
    return (false);
}

float	get_dist(t_pm *pm, int *color, float angle)
{
	t_cp rh; 
	t_cp rw;
	t_cp rho;
	t_cp rwo;
	float	dw;
	float	dh;
	int	iter;
	float	dist;
	//int	color;

	dh = 0;
	dw = 0;
	iter = pm->map.map_size.h;
	printf("check angle : %f\n", angle);
	if (!angle || angle == PI) //rh useless
	{
		rh = pm->player.pos;
		//printf("PI rh : %f, %f\n", rh.x, rh.y);
	}
		
	else if (angle == PI90 || angle == PI270)
	{
		rh.x = pm->player.pos.x;
		if (angle == 270)
		{
			rh.y = ((int)(pm->player.pos.y) >> 6 << 6) + UNIT_H;
			while (!is_obstacle(rh, pm->map.map) && iter > 0) //&& is_obstacle(rw, pm->map.map) == false
			{
				rh.y += UNIT_H;
				iter--;
			}
			//rho.y = rh.y - pm->player.pos.y; //necessary?
		}
		else
		{
			rh.y = ((int)(pm->player.pos.y) >> 6 << 6) - 0.0001;
			while (!is_obstacle(rh, pm->map.map)  && iter > 0) //&& is_obstacle(rw, pm->map.map) == false
			{
				rh.y -= UNIT_H;
				iter--;
			}
			//rho.y = pm->player.pos.y - rh.y; //necessary?
		}
		dh = fabs(pm->player.pos.y - rh.y);
		*color = H_COLOR;
		return (dh);
	}
	else if (angle < PI)
	{
		rh.y = ((int)(pm->player.pos.y) >> 6 << 6) - 0.0001; // necessary?
		rho.y = rh.y - pm->player.pos.y;
		rh.x = pm->player.pos.x + (rho.y * atan(angle));
		printf("rh : %f, %f\n", rh.x, rh.y);
		while (!is_obstacle(rh, pm->map.map)  && iter > 0) //&& is_obstacle(rw, pm->map.map) == false
		{
			rh.y -= UNIT_H;
			if (rh.y <= 0)
			{
				rh.y = 0;
				iter = 0;
			}
			rho.y = rh.y - pm->player.pos.y;
			rh.x = pm->player.pos.x + (rho.y * atan(angle));
			iter--;
			printf("rh in loop : %f, %f\n", rh.x, rh.y);
		}
		rho.x = fabs(pm->player.pos.x - rh.x);
		dh = sqrt(rho.x * rho.x + rho.y * rho.y); //av
	}
	else
	{
		rh.y = ((int)(pm->player.pos.y) >> 6 << 6) + UNIT_H;
		rho.y = rh.y - pm->player.pos.y;
		rh.x = pm->player.pos.x - (rho.y * atan(angle));
		while (!is_obstacle(rh, pm->map.map) && iter > 0) //&& is_obstacle(rw, pm->map.map) == false
		{
			rh.y += UNIT_H;
			rho.y = rh.y - pm->player.pos.y;
			rh.x = pm->player.pos.x - (rho.y * atan(angle));
			iter--;
		}
		rho.x = fabs(pm->player.pos.x - rh.x);
		dh = sqrt(rho.x * rho.x + rho.y * rho.y);
	}

	iter = pm->map.map_size.w; //av
	if (angle == PI90 || angle == PI270) //necessary?
		rw = pm->player.pos;
	else if (!angle || angle == PI)
	{
		rw.y = pm->player.pos.y;
		
		if (!angle)
		{
			rw.x = ((int)(pm->player.pos.x) >> 6 << 6) + UNIT_W;
			while (!is_obstacle(rw, pm->map.map) && iter > 0)
			{
				rw.x += UNIT_W;
				iter--;
			}
		}
		else
		{
			rw.x = ((int)(pm->player.pos.x) >> 6 << 6) - 0.0001;
			while (!is_obstacle(rw, pm->map.map) && iter > 0)
			{
				rw.x -= UNIT_W;
				iter--;
			}
		}
		dw = fabs(pm->player.pos.x - rw.x);
		*color = W_COLOR;
		return (dw);
	}
	else if (angle < PI90 || angle > PI270)
	{
		rw.x = ((int)(pm->player.pos.x) >> 6 << 6) + UNIT_W;
		rwo.x = rw.x - pm->player.pos.x;
		rw.y = pm->player.pos.y - (rwo.x * tan(angle));
		while (!is_obstacle(rw, pm->map.map) && iter > 0)
		{
			rw.x += UNIT_W;
			rwo.x = rw.x - pm->player.pos.x;
			rw.y = pm->player.pos.y - (rwo.x * tan(angle));
			iter--;
		}
		rwo.y = fabs(pm->player.pos.y - rw.y);
		dw = sqrt(rwo.x * rwo.x + rwo.y * rwo.y);
	}
	else
	{
		rw.x = ((int)(pm->player.pos.x) >> 6 << 6) - 0.0001;
		rwo.x = pm->player.pos.x - rw.x;
		rw.y = pm->player.pos.y - (rwo.x * tan(angle));
		while (!is_obstacle(rw, pm->map.map) && iter > 0)
		{
			rw.x -= UNIT_W;
			rwo.x = pm->player.pos.x - rw.x;
			rw.y = pm->player.pos.y - (rwo.x * tan(angle));
			iter--;
		}
		rwo.y = fabs(pm->player.pos.y - rw.y);
		dw = sqrt(rwo.x * rwo.x + rwo.y * rwo.y);
	}

	*color = W_COLOR;
	if (dh < dw)
	{	
		if (is_obstacle(rh, pm->map.map))
		{
			dist = dh;
			*color = H_COLOR;
		}	
		else
			dist = dw;
	}
	else
		dist = dw;

	// if (dist > WINDOW_H)
	// 	dist = WINDOW_H;
	// if (dist < 2)
	// 	dist = 2;
	
	//for test		
	//draw_point(img, rh); 
	//
	printf("angle: %f pos: %f, %f rh : %f, %f rw: %f, %f Dist %f \n", angle / DEGREE, pm->player.pos.x, pm->player.pos.y, rh.x, rh.y, rw.x, rw.y, dist);
	return (dist);
}


void	draw_line(t_img *img, float	h_line, int w_line, int fov, int color)
{
	float	start;
	float	end;
	static int px = 0;
	int	py;
	int	t;

	py = 0;

	start = (WINDOW_H - h_line) * 0.5;
	end = start + h_line;
	if (h_line > 2 * WINDOW_H)
		h_line = 2 * WINDOW_H;
	if ( h_line > WINDOW_H)
	{
		printf("here\n");
		start = 0;
		end = WINDOW_H;
	}
	t = w_line;
	printf("line h: %f w: %d start %f end %f fov %d \n", h_line, w_line, start, end, fov);
	while (py < WINDOW_H)
	{
		t = w_line;
		while (t)
		{
			if (py < start)
				my_mlx_pixel_put(&img, px, py, CEILING_C); //color a MAJ avec parsing
			else if (py > end)
				my_mlx_pixel_put(&img, px, py, FLOOR_C);
			else
				my_mlx_pixel_put(&img, px, py, color);
			px+=1;
			t--;
		}
		px -= w_line;
		py+=1;
	}
	px += w_line;
}

void correct_angle(float *angle)
{
	if (*angle < 0)
		*angle += PI360;
	if (*angle > PI360)
		*angle -= PI360;
}

void update_line(t_pm *pm, float *h_line, float *start, int *fov, int *color)
{
	//*h_line = get_dist(pm, color, *start) * cos(fabs(*start - pm->player.angle)) * WINDOW_H / (WINDOW_W / 4);
	*h_line = 1.5 * WINDOW_H * CDIST / (get_dist(pm, color, *start) * cos(fabs(*start - pm->player.angle)));
	*start -= DEGREE;
	(*fov)++;
	correct_angle(start);
}


void	draw_view(t_pm *pm)
{
	float	start;
	int	fov;
	int	pre_color;
	int	count;
	int w_line;

	float	pre_h_line;
	float	h_line;// create a structure?
	int	color;

	h_line = 0;
	pre_color = 0;
	color = 1;
	count = 1;
	start = FOV / 2 * DEGREE + pm->player.angle;
	fov = 0;
	w_line = WINDOW_W / FOV;
	correct_angle(&start);

	while (fov < FOV)
	{
		update_line(pm, &pre_h_line, &start, &fov, &pre_color);
		update_line(pm, &h_line, &start, &fov, &color);
		// count = 1;
		// while (fov && pre_h_line == h_line && color == pre_color)
		// {
		// 	count++;
		// 	update_line(pm, &h_line, &start, &fov, &color);
		// }
		draw_line(&(pm->img_scene), pre_h_line, count * w_line, fov, color);
		draw_line(&(pm->img_scene), h_line, w_line, fov, color);
	}
}
