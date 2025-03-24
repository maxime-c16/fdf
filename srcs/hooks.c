/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:54:55 by macauchy          #+#    #+#             */
/*   Updated: 2025/03/24 11:29:14 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static double	snap(double angle, double step, double threshold)
{
	double	nearest;

	nearest = round(angle / step) * step;
	if (fabs(angle - nearest) < threshold)
		return (nearest);
	return (angle);
}

int	mouse_press(int button, int x, int y)
{
	t_fdf	*fdf;

	fdf = _fdf();
	if (button == 1)
	{
		fdf->mouse_pressed = 1;
		fdf->mouse_x = x;
		fdf->mouse_y = y;
	}
	return (0);
}

int	mouse_release(int x, int y)
{
	t_fdf	*fdf;

	(void)x;
	(void)y;
	fdf = _fdf();
	fdf->mouse_pressed = 0;
	return (0);
}

int	mouse_move(int x, int y)
{
	t_fdf	*fdf;
	int		dx;
	int		dy;

	fdf = _fdf();
	if (fdf->mouse_pressed)
	{
		dx = x - fdf->mouse_x;
		dy = y - fdf->mouse_y;
		fdf->camera.rotation_x += (double)dy * SENS_MOUSE;
		fdf->camera.rotation_y += (double)dx * SENS_MOUSE;
		fdf->mouse_x = x;
		fdf->mouse_y = y;
		fdf->camera.rotation_x = snap(fdf->camera.rotation_x, M_PI / 2.0, 0.04);
		fdf->camera.rotation_y = snap(fdf->camera.rotation_y, M_PI / 2.0, 0.04);
		mlx_clear_window(fdf->mlx, fdf->win);
		draw_map();
	}
	return (0);
}
