#include "parsing.h"
#include "raycasting.h"

//temp
#define WALL 49

bool	is_valid_map_position(int x, int y, char **map)
{
	int	map_len;

	if (x < 0 || y < 0)
		return (false);
	map_len = ft_strslen(map);
	if (y >= map_len)
		return (false);
	if (x >= ft_strlen(map[y]))
		return (false);
	return (true);
}

static bool	is_obstacle(t_cp pos, char **map)
{
    int    x;
    int    y;

    x = pos.x / UNIT_W;
    y = pos.y / UNIT_H;
	if (!is_valid_map_position(x, y, map))
		return (true);
	printf("check in obst x : %d, y : %d \n", x, y);
	printf("map : %d\n", map[y][x] );
    if (map[y][x] == WALL)
        return (true);
    return (false);
}

float	get_dist(t_c3d *c3d, int *color, float angle)
{
	t_cp rh; 
	t_cp rw;
	t_cp rho;
	t_cp rwo;
	float	dw;
	float	dh;
	int	iter_h;
	int iter_w;
	float	dist;
	//int	color;

	dh = 0;
	dw = 0;
	iter_h = c3d->map.map_size.h;
	iter_w = c3d->map.map_size.w;
	printf("check angle : %f\n", angle);
	if (!angle || angle == PI) //rh useless
	{
		rh = c3d->player.pos;
		//printf("PI rh : %f, %f\n", rh.x, rh.y);
		rw.y = c3d->player.pos.y;
		
		if (!angle)
		{
			rw.x = ((int)(c3d->player.pos.x) >> 6 << 6) + UNIT_W;
			while (!is_obstacle(rw, c3d->map.map) && iter_w > 0)
			{
				rw.x += UNIT_W;
				iter_w--;
			}
		}
		else
		{
			rw.x = ((int)(c3d->player.pos.x) >> 6 << 6) - 0.0001;
			while (!is_obstacle(rw, c3d->map.map) && iter_w > 0)
			{
				rw.x -= UNIT_W;
				iter_w--;
			}
		}
		dw = fabs(c3d->player.pos.x - rw.x);
		*color = W_COLOR;
		return (dw);
	}
	else if (angle == PI90 || angle == PI270)
	{
		rw = c3d->player.pos;
		rh.x = c3d->player.pos.x;
		if (angle == 270)
		{
			rh.y = ((int)(c3d->player.pos.y) >> 6 << 6) + UNIT_H;
			while (!is_obstacle(rh, c3d->map.map) && iter_h > 0) //&& is_obstacle(rw, c3d->map.map) == false
			{
				rh.y += UNIT_H;
				iter_h--;
			}
			//rho.y = rh.y - c3d->player.pos.y; //necessary?
		}
		else
		{
			rh.y = ((int)(c3d->player.pos.y) >> 6 << 6) - 0.0001;
			while (!is_obstacle(rh, c3d->map.map)  && iter_h > 0) //&& is_obstacle(rw, c3d->map.map) == false
			{
				rh.y -= UNIT_H;
				iter_h--;
			}
			//rho.y = c3d->player.pos.y - rh.y; //necessary?
		}
		dh = fabs(c3d->player.pos.y - rh.y);
		*color = H_COLOR;
		return (dh);
	}


	if (angle < PI)
	{
		rh.y = ((int)(c3d->player.pos.y) >> 6 << 6) - 0.0001; // necessary?
		rho.y = rh.y - c3d->player.pos.y;
		rh.x = c3d->player.pos.x + (rho.y * atan(angle));

		if (angle < PI90)
		{
			rw.x = ((int)(c3d->player.pos.x) >> 6 << 6) + UNIT_W;
			rwo.x = rw.x - c3d->player.pos.x;
			rw.y = c3d->player.pos.y - (rwo.x * tan(angle));
		}
		else
		{
			rw.x = ((int)(c3d->player.pos.x) >> 6 << 6) - 0.0001;
			rwo.x = c3d->player.pos.x - rw.x;
			rw.y = c3d->player.pos.y - (rwo.x * tan(angle));
		}

		while (!is_obstacle(rh, c3d->map.map) && !is_obstacle(rw, c3d->map.map) && iter_h > 0 && iter_w > 0) //&& is_obstacle(rw, c3d->map.map) == false
		{
			rh.y -= UNIT_H;
			if (rh.y <= 0)
			{
				rh.y = 0;
				iter_h = 0;
			}
			rho.y = rh.y - c3d->player.pos.y;
			rh.x = c3d->player.pos.x + (rho.y * atan(angle));
			iter_h--;
			if (angle < PI90)
			{
				rw.x += UNIT_W;
				rwo.x = rw.x - c3d->player.pos.x;
				rw.y = c3d->player.pos.y - (rwo.x * tan(angle));
				
			}
			else
			{
				rw.x -= UNIT_W;
				if (rw.x <= 0)
				{
					rw.x = 0;
					iter_w = 0;
				}
				rwo.x = c3d->player.pos.x - rw.x;
				rw.y = c3d->player.pos.y - (rwo.x * tan(angle));
			}
			iter_w--;		
			
		}
		rho.x = fabs(c3d->player.pos.x - rh.x);
		dh = sqrt(rho.x * rho.x + rho.y * rho.y); //av
		rwo.y = fabs(c3d->player.pos.y - rw.y);
		dw = sqrt(rwo.x * rwo.x + rwo.y * rwo.y);
	}
	else
	{
		rh.y = ((int)(c3d->player.pos.y) >> 6 << 6) + UNIT_H;
		rho.y = rh.y - c3d->player.pos.y;
		rh.x = c3d->player.pos.x - (rho.y * atan(angle));


		if (angle > PI270)
		{
			rw.x = ((int)(c3d->player.pos.x) >> 6 << 6) + UNIT_W;
			rwo.x = rw.x - c3d->player.pos.x;
			rw.y = c3d->player.pos.y - (rwo.x * tan(angle));
		}
		else
		{
			rw.x = ((int)(c3d->player.pos.x) >> 6 << 6) - 0.0001;
			rwo.x = c3d->player.pos.x - rw.x;
			rw.y = c3d->player.pos.y - (rwo.x * tan(angle));
		}

		while (!is_obstacle(rh, c3d->map.map) && iter_h > 0) //&& is_obstacle(rw, c3d->map.map) == false
		{
			rh.y += UNIT_H;
			rho.y = rh.y - c3d->player.pos.y;
			rh.x = c3d->player.pos.x - (rho.y * atan(angle));
			iter_h--;

			if (angle > PI270)
			{
				rw.x += UNIT_W;
				rwo.x = rw.x - c3d->player.pos.x;
				rw.y = c3d->player.pos.y - (rwo.x * tan(angle));
				
			}
			else
			{
				rw.x -= UNIT_W;
				rwo.x = c3d->player.pos.x - rw.x;
				rw.y = c3d->player.pos.y - (rwo.x * tan(angle));
			}
			iter_w--;	
		}
		rho.x = fabs(c3d->player.pos.x - rh.x);
		dh = sqrt(rho.x * rho.x + rho.y * rho.y);
		rwo.y = fabs(c3d->player.pos.y - rw.y);
		dw = sqrt(rwo.x * rwo.x + rwo.y * rwo.y);
	}

	*color = W_COLOR;
	if (dh < dw)
	{	
		if (is_obstacle(rh, c3d->map.map))
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
	printf("angle: %f pos: %f, %f rh : %f, %f rw: %f, %f Dist %f \n", angle / DEGREE, c3d->player.pos.x, c3d->player.pos.y, rh.x, rh.y, rw.x, rw.y, dist);
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

void update_line(t_c3d *c3d, float *h_line, float *start, int *fov, int *color)
{
	//*h_line = get_dist(c3d, color, *start) * cos(fabs(*start - c3d->player.angle)) * WINDOW_H / (WINDOW_W / 4);
	*h_line = 1.5 * WINDOW_H * CDIST / (get_dist(c3d, color, *start) * cos(fabs(*start - c3d->player.angle)));
	*start -= DEGREE;
	(*fov)++;
	correct_angle(start);
}


void	draw_view(t_c3d *c3d)
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
	start = FOV / 2 * DEGREE + c3d->player.angle;
	fov = 0;
	w_line = WINDOW_W / FOV;
	correct_angle(&start);

	while (fov < FOV)
	{
		update_line(c3d, &pre_h_line, &start, &fov, &pre_color);
		update_line(c3d, &h_line, &start, &fov, &color);
		// count = 1;
		// while (fov && pre_h_line == h_line && color == pre_color)
		// {
		// 	count++;
		// 	update_line(c3d, &h_line, &start, &fov, &color);
		// }
		draw_line(&(c3d->img_scene), pre_h_line, count * w_line, fov, color);
		draw_line(&(c3d->img_scene), h_line, w_line, fov, color);
	}
}
