/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gyro.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 11:12:13 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/16 15:02:50 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static void	draw_line_c(t_point a, t_point b, int color)
{
	t_fdf	*fdf;
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	int		e2;
	t_point	start;
	t_point	a_gl;
	double	total_dist;
	double	curr_dist;
	double	t_param;
	double	current_z;

	fdf = _fdf();
	start = a;
	dx = abs(b.x - a.x);
	dy = abs(b.y - a.y);
	sx = a.x < b.x ? 1 : -1;
	sy = a.y < b.y ? 1 : -1;
	err = (dx > dy ? dx : -dy) / 2;
	total_dist = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
	while (1)
	{
		curr_dist = sqrt(pow(a.x - start.x, 2) + pow(a.y - start.y, 2));
		if (total_dist == 0)
			t_param = 0;
		else
			t_param = curr_dist / total_dist;
		current_z = lerp(a.z, b.z, t_param);
		set_pixel(fdf->img_data, a.x, a.y, current_z);
		if (a.x == b.x && a.y == b.y)
			break ;
		e2 = err;
		if (e2 > -dx)
		{
			err -= dy;
			a.x += sx;
		}
		if (e2 < dy)
		{
			err += dx;
			a.y += sy;
		}
	}
}

static void	get_gyro_point(t_gyro *g, int *px, int *py)
{
	double	x;
	double	y;
	double	z;
	double	res1;
	double	res2;
	t_point	proj;

	if (g->axis == 'x')
	{
		x = 0;
		y = g->rad * cos(g->angle);
		z = g->rad * sin(g->angle);
	}
	else if (g->axis == 'y')
	{
		x = g->rad * cos(g->angle);
		y = 0;
		z = g->rad * sin(g->angle);
	}
	else
	{
		x = g->rad * cos(g->angle);
		y = g->rad * sin(g->angle);
		z = 0;
	}
	apply_y_rotation(&x, &z, *g->ry);
	apply_x_rotation(&y, &z, *g->rx);
	apply_z_rotation(&x, &y, *g->rz);
	apply_proj(&proj, x, y, z);
	*px = proj.x + g->cx;
	*py = proj.y + g->cy;
}

static void	draw_colored_line(t_fdf *fdf, int *curr, int *v, char axis)
{
	t_point	a;
	t_point	b;

	a.x = curr[0];
	a.y = curr[1];
	a.z = 0;
	b.x = v[0];
	b.y = v[1];
	b.z = 0;
	if (axis == 'x')
		draw_line_c(a, b, 0xFF0000);
	else if (axis == 'y')
		draw_line_c(a, b, 0x00FF00);
	else
		draw_line_c(a, b, 0x0000FF);
}

static void	draw_circle(t_fdf *fdf, char axis)
{
	int		v[2];
	int		curr[2];
	int		i;

	i = 0;
	fdf->gyro.axis = axis;
	while (i <= SEG)
	{
		fdf->gyro.angle = (2.0 * M_PI * i) / SEG;
		get_gyro_point(&fdf->gyro, &curr[0], &curr[1]);
		if (i > 0)
			draw_colored_line(fdf, curr, v, fdf->gyro.axis);
		v[0] = curr[0];
		v[1] = curr[1];
		i++;
	}
}
void	draw_gyroscope_sphere(t_fdf *fdf)
{
	draw_circle(fdf, 'x');
	draw_circle(fdf, 'y');
	draw_circle(fdf, 'z');
}
