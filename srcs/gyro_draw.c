/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gyro_draw.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:57:54 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/22 15:41:51 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static void	draw_colored_line_helper(t_point a, t_point b, char axis)
{
	if (axis == 'x')
		draw_line_c(a, b, 0xFF0000);
	else if (axis == 'y')
		draw_line_c(a, b, 0x00FF00);
	else
		draw_line_c(a, b, 0x0000FF);
}

void	draw_colored_line(int *curr, int *v, char axis)
{
	t_point	a;
	t_point	b;

	a.x = curr[0];
	a.y = curr[1];
	a.z = 0;
	b.x = v[0];
	b.y = v[1];
	b.z = 0;
	draw_colored_line_helper(a, b, axis);
}

static void	draw_circle_step(t_fdf *fdf, int i, int *curr, int *v)
{
	fdf->gyro.angle = (2.0 * M_PI * i) / SEG;
	get_gyro_point(&fdf->gyro, &curr[0], &curr[1]);
	if (i > 0)
		draw_colored_line(curr, v, fdf->gyro.axis);
	v[0] = curr[0];
	v[1] = curr[1];
}

static void	draw_circle(t_fdf *fdf, char axis)
{
	int		v[2];
	int		curr[2];
	int		i;

	i = 0;
	fdf->gyro.axis = axis;
	while (i <= SEG)
	{
		draw_circle_step(fdf, i, curr, v);
		i++;
	}
}

void	draw_gyroscope_sphere(t_fdf *fdf)
{
	draw_circle(fdf, 'x');
	draw_circle(fdf, 'y');
	draw_circle(fdf, 'z');
}
