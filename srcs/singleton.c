/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:48:10 by macauchy          #+#    #+#             */
/*   Updated: 2025/03/25 14:03:11 by macauchy         ###   ########.fr       */
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
		fdf.img = mlx_new_image(fdf.mlx, WIDTH, HEIGHT);
		fdf.camera.zoom = (WIDTH + HEIGHT) / 40;
		fdf.camera.x_offset = WIDTH / 2;
		fdf.camera.y_offset = HEIGHT / 2;
		fdf.gyro.rad = (double)WIDTH / 20.0;
		fdf.gyro.rx = &fdf.camera.rotation_x;
		fdf.gyro.ry = &fdf.camera.rotation_y;
		fdf.gyro.rz = &fdf.camera.rotation_z;
		fdf.gyro.cx = WIDTH - (int)fdf.gyro.rad - 10;
		fdf.gyro.cy = (int)fdf.gyro.rad + 10;
		fdf.audio_map = NULL;
		init = 1;
	}
	return (&fdf);
}
