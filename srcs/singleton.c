/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:48:10 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/20 15:59:55 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static void	init_fdf_window(t_fdf *fdf)
{
	fdf->win = mlx_new_window(fdf->mlx, WIDTH, HEIGHT, "fdf");
	if (!fdf->win)
	{
		mlx_destroy_display(fdf->mlx);
		ft_putstr_fd("Error: mlx_new_window() failed\n", 2);
		exit(1);
	}
}

static void	init_fdf_image(t_fdf *fdf)
{
	fdf->img = mlx_new_image(fdf->mlx, WIDTH, HEIGHT);
	if (!fdf->img)
	{
		ft_putstr_fd("Error: mlx_new_image() failed\n", 2);
		mlx_destroy_window(fdf->mlx, fdf->win);
		mlx_destroy_display(fdf->mlx);
		exit(1);
	}
}

static void	init_fdf_camera_gyro(t_fdf *fdf)
{
	fdf->camera.zoom = (WIDTH + HEIGHT) / 40;
	fdf->camera.x_offset = WIDTH / 2;
	fdf->camera.y_offset = HEIGHT / 2;
	fdf->gyro.rad = (double)WIDTH / 20.0;
	fdf->gyro.rx = &fdf->camera.rotation_x;
	fdf->gyro.ry = &fdf->camera.rotation_y;
	fdf->gyro.rz = &fdf->camera.rotation_z;
	fdf->gyro.cx = WIDTH - (int)fdf->gyro.rad - 10;
	fdf->gyro.cy = (int)fdf->gyro.rad + 10;
}

t_fdf	*_fdf(void)
{
	static t_fdf	fdf;
	static int		init = 0;

	if (!init)
	{
		ft_bzero(&fdf, sizeof(t_fdf));
		fdf.mlx = mlx_init();
		if (!fdf.mlx)
		{
			ft_putstr_fd("Error: mlx_init() failed\n", 2);
			exit(1);
		}
		fdf.win = mlx_new_window(fdf.mlx, WIDTH, HEIGHT, "fdf");
		if (!fdf.win)
		{
			mlx_destroy_display(fdf.mlx);
			ft_putstr_fd("Error: mlx_new_window() failed\n", 2);
			exit(1);
		}
		fdf.img = mlx_new_image(fdf.mlx, WIDTH, HEIGHT);
		if (!fdf.img)
		{
			ft_putstr_fd("Error: mlx_new_image() failed\n", 2);
			mlx_destroy_window(fdf.mlx, fdf.win);
			mlx_destroy_display(fdf.mlx);
			exit(1);
		}
		fdf.camera.zoom = (WIDTH + HEIGHT) / 40;
		fdf.camera.x_offset = WIDTH / 2;
		fdf.camera.y_offset = HEIGHT / 2;
		fdf.gyro.rad = (double)WIDTH / 20.0;
		fdf.gyro.rx = &fdf.camera.rotation_x;
		fdf.gyro.ry = &fdf.camera.rotation_y;
		fdf.gyro.rz = &fdf.camera.rotation_z;
		fdf.gyro.cx = WIDTH - (int)fdf.gyro.rad - 10;
		fdf.gyro.cy = (int)fdf.gyro.rad + 10;
		init = 1;
	}
	return (&fdf);
}

// t_fdf	*_fdf(void)
// {
// 	static t_fdf	fdf;
// 	static int		init = 0;

// 	if (!init)
// 	{
// 		ft_bzero(&fdf, sizeof(t_fdf));
// 		fdf.mlx = mlx_init();
// 		if (!fdf.mlx)
// 		{
// 			ft_putstr_fd("Error: mlx_init() failed\n", 2);
// 			exit(1);
// 		}
// 		init_fdf_window(&fdf);
// 		init_fdf_image(&fdf);
// 		init_fdf_camera_gyro(&fdf);
// 		init = 1;
// 	}
// 	return (&fdf);
// }
