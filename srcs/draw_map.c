/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:52:49 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/20 12:49:32 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	draw_map(void)
{
	t_drawmap_ctx	ctx;
	int				i_start;
	int				i_step;

	init_draw_map_ctx(&ctx, &i_start, &i_step);
	draw_map_loop(&ctx, i_start, i_step);
	draw_gyroscope_sphere(ctx.fdf);
	mlx_put_image_to_window(ctx.fdf->mlx, ctx.fdf->win, ctx.fdf->img, 0, 0);
}
