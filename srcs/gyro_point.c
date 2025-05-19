/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gyro_point.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:55:21 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/19 16:29:23 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static void	get_axis_coords(
	t_gyro *g, double *x, double *y, double *z)
{
	if (g->axis == 'x')
	{
		*x = 0;
		*y = g->rad * cos(g->angle);
		*z = g->rad * sin(g->angle);
	}
	else if (g->axis == 'y')
	{
		*x = g->rad * cos(g->angle);
		*y = 0;
		*z = g->rad * sin(g->angle);
	}
	else
	{
		*x = g->rad * cos(g->angle);
		*y = g->rad * sin(g->angle);
		*z = 0;
	}
}

static void	apply_rot_proj(t_gyro *g, double *coords, int *px, int *py)
{
	t_point	proj;
	double	x;
	double	y;
	double	z;

	y = coords[1];
	x = coords[0];
	z = coords[2];
	apply_y_rotation(&x, &z, *g->ry);
	apply_x_rotation(&y, &z, *g->rx);
	apply_z_rotation(&x, &y, *g->rz);
	apply_proj(&proj, x, y, z);
	*px = proj.x + g->cx;
	*py = proj.y + g->cy;
}

void	get_gyro_point(t_gyro *g, int *px, int *py)
{
	double	x;
	double	y;
	double	z;
	double	coords[3];

	get_axis_coords(g, &x, &y, &z);
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
	apply_rot_proj(g, coords, px, py);
}
