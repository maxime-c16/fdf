/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:15:31 by macauchy          #+#    #+#             */
/*   Updated: 2025/03/20 15:46:56 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static void	apply_y_rotation(double *x, double *z, double theta)
{
	double x_new;
	double z_new;

	x_new = *x * cos(theta) + *z * sin(theta);
	z_new = -*x * sin(theta) + *z * cos(theta);
	*x = x_new;
	*z = z_new;
}

static void	apply_x_rotation(double *y, double *z, double phi)
{
	double y_new;
	double z_new;

	y_new = *y * cos(phi) - *z * sin(phi);
	z_new = *y * sin(phi) + *z * cos(phi);
	*y = y_new;
	*z = z_new;
}

static void	apply_z_rotation(double *x, double *y, double psi)
{
	double x_new;
	double y_new;

	x_new = *x * cos(psi) - *y * sin(psi);
	y_new = *x * sin(psi) + *y * cos(psi);
	*x = x_new;
	*y = y_new;
}

static void	apply_isometric_projection(t_point *point, double x, double y, double z)
{
	point->x = (x - y) * cos(M_PI / 6);
	point->y = (x + y) * sin(M_PI / 6) - z;
}

t_point	project_point(int i, int j)
{
	t_fdf	*fdf;
	t_point	point;
	double	x;
	double	y;
	double	z;

	fdf = _fdf();
	x = j * fdf->camera.zoom - fdf->center_x;
	y = i * fdf->camera.zoom - fdf->center_y;
	z = fdf->map[i][j] * fdf->camera.height_factor;
	// point.color_altitude = (int)z;
	apply_y_rotation(&x, &z, fdf->camera.rotation_y);
	apply_x_rotation(&y, &z, fdf->camera.rotation_x);
	apply_z_rotation(&x, &y, fdf->camera.rotation_z);
	apply_isometric_projection(&point, x, y, z);
	point.x += fdf->camera.x_offset;
	point.y += fdf->camera.y_offset;
	point.z = fdf->map[i][j];
	return (point);
}
