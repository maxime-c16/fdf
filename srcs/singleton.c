/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:48:10 by macauchy          #+#    #+#             */
/*   Updated: 2025/03/22 19:46:26 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

t_fdf	*_fdf(void)
{
	static t_fdf	fdf;
	static int		init = 0;

	if (!init)
	{
		ft_bzero(&fdf, sizeof(t_fdf));
		fdf.gl = ft_gl_init();
		fdf.mlx = mlx_init();
		if (!fdf.mlx)
		{
			ft_putstr_fd("Error: mlx_init() failed\n", 2);
			exit(1);
		}
		fdf.win = mlx_new_window(fdf.mlx, WIDTH, HEIGHT, "fdf");
		if (!fdf.win)
		{
			ft_putstr_fd("Error: mlx_new_window() failed\n", 2);
			exit(1);
		}
		fdf.camera.zoom = (WIDTH + HEIGHT) / 40;
		fdf.camera.x_offset = WIDTH / 2;
		fdf.camera.y_offset = HEIGHT / 2;
		init = 1;
	}
	return (&fdf);
}
