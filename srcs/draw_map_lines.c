/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map_lines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:46:02 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/23 12:00:07 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

int	is_visible(t_point a, t_point b)
{
	if ((a.x < 0 && b.x < 0) || (a.x >= WIDTH && b.x >= WIDTH))
		return (0);
	if ((a.y < 0 && b.y < 0) || (a.y >= HEIGHT && b.y >= HEIGHT))
		return (0);
	return (1);
}

void	draw_map_lines(t_drawmap_ctx *ctx, int i, int j)
{
	t_point	a;
	t_point	b;
	t_point	gyro_a;
	t_point	gyro_b;

	a = project_point(i, j);
	gyro_a = project_point_scaled(i, j);
	if (j + 1 < ctx->fdf->width)
	{
		b = project_point(i, j + 1);
		gyro_b = project_point_scaled(i, j + 1);
		if (is_visible(a, b))
			draw_line(a, b, ctx->fdf->img_data);
		draw_line(gyro_a, gyro_b, ctx->fdf->img_data);
	}
	if (i + 1 < ctx->fdf->height)
	{
		b = project_point(i + 1, j);
		gyro_b = project_point_scaled(i + 1, j);
		if (is_visible(a, b))
			draw_line(a, b, ctx->fdf->img_data);
		draw_line(gyro_a, gyro_b, ctx->fdf->img_data);
	}
}

void	draw_map_rows(t_drawmap_ctx *ctx)
{
	int	j;
	int	j_start;
	int	j_step;

	if (ctx->fdf->camera.rotation_y > 0)
	{
		j_start = ctx->fdf->width - 1;
		j_step = -1;
	}
	else
	{
		j_start = 0;
		j_step = 1;
	}
	j = j_start;
	while (j < ctx->fdf->width && j >= 0)
	{
		draw_map_lines(ctx, ctx->i, j);
		j += j_step;
	}
}
