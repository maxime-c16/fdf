/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:15:31 by macauchy          #+#    #+#             */
/*   Updated: 2025/03/23 20:28:31 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	apply_y_rotation(double *x, double *z, double theta)
{
	double x_new;
	double z_new;

	x_new = *x * cos(theta) + *z * sin(theta);
	z_new = -*x * sin(theta) + *z * cos(theta);
	*x = x_new;
	*z = z_new;
}

void	apply_x_rotation(double *y, double *z, double phi)
{
	double y_new;
	double z_new;

	y_new = *y * cos(phi) - *z * sin(phi);
	z_new = *y * sin(phi) + *z * cos(phi);
	*y = y_new;
	*z = z_new;
}

void	apply_z_rotation(double *x, double *y, double psi)
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

static void	apply_perspective_projection(t_point *point, double x, double y, double z)
{
	point->x = x / (1 - z / 1000);
	point->y = y / (1 - z / 1000);
}

static void	apply_oblique_projection(t_point *point, double x, double y, double z)
{
	point->x = x + 0.5 * z * cos(M_PI / 6);
	point->y = y + 0.5 * z * sin(M_PI / 6);
}

static void	apply_cabinet_projection(t_point *point, double x, double y, double z)
{
	point->x = 0.5 * x + z * cos(M_PI / 6);
	point->y = 0.5 * y + z * sin(M_PI / 6);
}

static void	apply_conic_projection(t_point *point, double x, double y, double z)
{
	point->x = x * z;
	point->y = y * z;
}

void	apply_proj(t_point *point, double x, double y, double z)
{
	t_fdf	*fdf;

	fdf = _fdf();
	if (fdf->proj_style == PROJ_ISOMETRIC)
		apply_isometric_projection(point, x, y, z);
	else if (fdf->proj_style == PROJ_PERSPECTIVE)
		apply_perspective_projection(point, x, y, z);
	else if (fdf->proj_style == PROJ_OBLIQUE)
		apply_oblique_projection(point, x, y, z);
	else if (fdf->proj_style == PROJ_CABINET)
		apply_cabinet_projection(point, x, y, z);
	else if (fdf->proj_style == PROJ_CONIC)
		apply_conic_projection(point, x, y, z);
	else
	{
		point->x = x;
		point->y = y;
	}
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
	apply_y_rotation(&x, &z, fdf->camera.rotation_y);
	apply_x_rotation(&y, &z, fdf->camera.rotation_x);
	apply_z_rotation(&x, &y, fdf->camera.rotation_z);
	apply_proj(&point, x, y, z);
	point.x += fdf->camera.x_offset;
	point.y += fdf->camera.y_offset;
	point.z = fdf->map[i][j];
	return (point);
}

t_point	project_point_scaled(int i, int j)
{
	t_gyro	gyro;
	t_point	point;
	double	x;
	double	y;
	double	z;
	double	scale;
	double	cz;

	gyro = _fdf()->gyro;
	scale = (gyro.rad) / max(1, _fdf()->max_altitude - _fdf()->min_altitude);
	x = (j - _fdf()->width / 2.0) * scale;
	y = (i - _fdf()->height / 2.0) * scale;
	gyro.height_factor = scale / 4.0;
	cz = (_fdf()->max_altitude - _fdf()->min_altitude) / 2.0;
	z = (_fdf()->map[i][j] - cz) * gyro.height_factor;
	apply_y_rotation(&x, &z, *(gyro).ry);
	apply_x_rotation(&y, &z, *(gyro).rx);
	apply_z_rotation(&x, &y, *(gyro).rz);
	apply_proj(&point, x, y, z);
	point.x += gyro.cx;
	point.y += gyro.cy;
	point.z = _fdf()->map[i][j];
	return (point);
}
