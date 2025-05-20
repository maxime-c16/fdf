/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:48:24 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/20 15:06:19 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	init_draw_map_ctx(t_drawmap_ctx *ctx, int *i_start, int *i_step)
{
	ctx->fdf = _fdf();
	ctx->fdf->img_data = mlx_get_data_addr(ctx->fdf->img, &ctx->fdf->bpp,
			&ctx->fdf->size_line, &ctx->fdf->endian);
	ft_bzero(ctx->fdf->img_data, ctx->fdf->size_line * HEIGHT);
	if (ctx->fdf->camera.rotation_x > 0)
	{
		*i_start = ctx->fdf->height - 1;
		*i_step = -1;
	}
	else
	{
		*i_start = 0;
		*i_step = 1;
	}
}

void	draw_map_loop(t_drawmap_ctx *ctx, int i_start, int i_step)
{
	ctx->i = i_start;
	while (ctx->i < ctx->fdf->height && ctx->i >= 0)
	{
		draw_map_rows(ctx);
		ctx->i += i_step;
	}
}
