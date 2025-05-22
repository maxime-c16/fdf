/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection_point.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:17:11 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/22 15:16:15 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

t_point	project_point(int i, int j)
{
	t_fdf		*fdf;
	t_point		point;
	t_proj_vars	vars;

	fdf = _fdf();
	vars.x = (j - fdf->width / 2.0) * fdf->camera.zoom;
	vars.y = (i - fdf->height / 2.0) * fdf->camera.zoom;
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

static double	get_min_scale(t_fdf *fdf, t_gyro *gyro)
{
	double	scale_x;
	double	scale_y;
	double	scale_z;
	double	scale;

	if (fdf->width > 1)
		scale_x = gyro->rad / (fdf->width - 1);
	else
		scale_x = gyro->rad;
	if (fdf->height > 1)
		scale_y = gyro->rad / (fdf->height - 1);
	else
		scale_y = gyro->rad;
	if ((fdf->max_altitude - fdf->min_altitude) > 0)
		scale_z = gyro->rad / (fdf->max_altitude - fdf->min_altitude);
	else
		scale_z = gyro->rad;
	scale = scale_x;
	if (scale_y < scale)
		scale = scale_y;
	if (scale_z < scale)
		scale = scale_z;
	if (scale > MAX_SCALE)
		scale = MAX_SCALE;
	return (scale);
}

static void	fill_scale_ctx(t_scale_ctx *ctx, t_fdf *fdf, int i, int j)
{
	t_gyro	*gyro;

	gyro = &fdf->gyro;
	ctx->x = (j - fdf->width / 2.0) * ctx->scale;
	ctx->y = (i - fdf->height / 2.0) * ctx->scale;
	gyro->height_factor = ctx->scale / 4.0;
	ctx->cz = (fdf->max_altitude - fdf->min_altitude) / 2.0;
	ctx->z = (fdf->map[i][j] - ctx->cz) * gyro->height_factor;
}

static t_point	apply_rot_proj_ctx(t_scale_ctx *ctx, t_fdf *fdf, int i, int j)
{
	t_point	point;
	t_gyro	*gyro;

	gyro = &fdf->gyro;
	apply_y_rotation(&ctx->x, &ctx->z, *(gyro->ry));
	apply_x_rotation(&ctx->y, &ctx->z, *(gyro->rx));
	apply_z_rotation(&ctx->x, &ctx->y, *(gyro->rz));
	apply_proj(&point, ctx->x, ctx->y, ctx->z);
	point.x += gyro->cx;
	point.y += gyro->cy;
	point.z = fdf->map[i][j];
	return (point);
}

t_point	project_point_scaled(int i, int j)
{
	t_gyro		gyro;
	t_fdf		*fdf;
	t_scale_ctx	ctx;
	t_point		point;

	fdf = _fdf();
	gyro = fdf->gyro;
	ctx.scale = get_min_scale(fdf, &gyro);
	fill_scale_ctx(&ctx, fdf, i, j);
	point = apply_rot_proj_ctx(&ctx, fdf, i, j);
	return (point);
}
