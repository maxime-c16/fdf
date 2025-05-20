/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection_point.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:17:11 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/20 15:47:51 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

t_point	project_point(int i, int j)
{
	t_fdf		*fdf;
	t_point		point;
	t_proj_vars	vars;

	fdf = _fdf();
	vars.x = j * fdf->camera.zoom - fdf->center_x;
	vars.y = i * fdf->camera.zoom - fdf->center_y;
	vars.z = fdf->map[i][j] * fdf->camera.height_factor;
	apply_y_rotation(&vars.x, &vars.z, fdf->camera.rotation_y);
	apply_x_rotation(&vars.y, &vars.z, fdf->camera.rotation_x);
	apply_z_rotation(&vars.x, &vars.y, fdf->camera.rotation_z);
	apply_proj(&point, vars.x, vars.y, vars.z);
	point.x += fdf->camera.x_offset;
	point.y += fdf->camera.y_offset;
	point.z = fdf->map[i][j];
	return (point);
}

t_point	project_point_scaled(int i, int j)
{
	t_gyro		gyro;
	t_point		point;
	t_proj_vars	vars;

	gyro = _fdf()->gyro;
	vars.scale = (gyro.rad) / max(1, _fdf()->max_altitude - \
		_fdf()->min_altitude);
	vars.x = (j - _fdf()->width / 2.0) * vars.scale;
	vars.y = (i - _fdf()->height / 2.0) * vars.scale;
	gyro.height_factor = vars.scale / 4.0;
	vars.cz = (_fdf()->max_altitude - _fdf()->min_altitude) / 2.0;
	vars.z = (_fdf()->map[i][j] - vars.cz) * gyro.height_factor;
	apply_y_rotation(&vars.x, &vars.z, *(gyro.ry));
	apply_x_rotation(&vars.y, &vars.z, *(gyro.rx));
	apply_z_rotation(&vars.x, &vars.y, *(gyro.rz));
	apply_proj(&point, vars.x, vars.y, vars.z);
	point.x += gyro.cx;
	point.y += gyro.cy;
	point.z = _fdf()->map[i][j];
	return (point);
}
