/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gyro_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:29:48 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/22 15:31:32 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static void	init_bresenham(t_point a, t_point b, t_bresenham *bsh)
{
	bsh->dx = abs(b.x - a.x);
	bsh->dy = abs(b.y - a.y);
	if (a.x < b.x)
		bsh->sx = 1;
	else
		bsh->sx = -1;
	if (a.y < b.y)
		bsh->sy = 1;
	else
		bsh->sy = -1;
	if (bsh->dx > bsh->dy)
		bsh->err = bsh->dx / 2;
	else
		bsh->err = -bsh->dy / 2;
}

static void	step_pixel(t_bresenham *bsh, t_point *a)
{
	int	e2;

	e2 = bsh->err;
	if (e2 > -bsh->dx)
	{
		bsh->err -= bsh->dy;
		a->x += bsh->sx;
	}
	if (e2 < bsh->dy)
	{
		bsh->err += bsh->dx;
		a->y += bsh->sy;
	}
}

static void	compute_line_params(t_line_params *lp)
{
	double	curr_dist;
	double	dx;
	double	dy;
	double	sx;
	double	sy;

	dx = lp->b.x - lp->a.x;
	dy = lp->b.y - lp->a.y;
	sx = lp->a.x - lp->start.x;
	sy = lp->a.y - lp->start.y;
	curr_dist = sqrt(sx * sx + sy * sy);
	if (sqrt(dx * dx + dy * dy) == 0)
		lp->t_param = 0;
	else
		lp->t_param = curr_dist / sqrt(dx * dx + dy * dy);
	lp->current_z = lerp(lp->a.z, lp->b.z, lp->t_param);
}

void	draw_line_c(t_point a, t_point b, int color)
{
	t_fdf			*fdf;
	t_point			start;
	t_bresenham		bsh;
	t_line_params	lp;

	fdf = _fdf();
	start = a;
	init_bresenham(a, b, &bsh);
	lp.a = a;
	lp.start = start;
	lp.b = b;
	fdf->flag = 1;
	fdf->color = color;
	while (1)
	{
		compute_line_params(&lp);
		set_pixel(fdf->img_data, lp.a.x, lp.a.y, lp.current_z);
		if (lp.a.x == lp.b.x && lp.a.y == lp.b.y)
			break ;
		step_pixel(&bsh, &lp.a);
	}
	fdf->flag = 0;
	fdf->color = 0;
}
