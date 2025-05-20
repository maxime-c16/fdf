/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection_point.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:17:11 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/20 15:20:22 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

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
