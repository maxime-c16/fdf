#include "../includes/fdf.h"
#include <math.h>

static double	calc_total_dist(t_point a, t_point b)
{
	return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
}

static double	calc_t_param(t_point a, t_point start, double total_dist)
{
	double	curr_dist;

	curr_dist = sqrt(pow(a.x - start.x, 2) + pow(a.y - start.y, 2));
	if (total_dist == 0)
		return (0);
	return (curr_dist / total_dist);
}

static void	draw_line_step(t_point *a, int *err, int dx, int dy, int sx, int sy)
{
	int e2;

	e2 = *err;
	if (e2 > -dx)
	{
		*err -= dy;
		a->x += sx;
	}
	if (e2 < dy)
	{
		*err += dx;
		a->y += sy;
	}
}

static int	get_sign(int a, int b)
{
	if (a < b)
		return (1);
	return (-1);
}

static int	get_err(int dx, int dy)
{
	if (dx > dy)
		return (dx / 2);
	return (-dy / 2);
}

void	draw_line(t_point a, t_point b, char *img_data)
{
	t_line	line;
	double	t_param;
	double	current_z;
	int		dx;
	int		dy;

	line.a = a;
	line.b = b;
	line.start = a;
	line.sx = get_sign(a.x, b.x);
	line.sy = get_sign(a.y, b.y);
	dx = abs(b.x - a.x);
	dy = abs(b.y - a.y);
	line.err = get_err(dx, dy);
	line.total_dist = calc_total_dist(a, b);
	while (1)
	{
		t_param = calc_t_param(line.a, line.start, line.total_dist);
		current_z = lerp(line.b.z, line.a.z, t_param);
		set_pixel(img_data, (int)line.a.x, (int)line.a.y, current_z);
		if (line.a.x == line.b.x && line.a.y == line.b.y)
			break;
		draw_line_step(&line.a, &line.err, dx, dy, line.sx, line.sy);
	}
}
