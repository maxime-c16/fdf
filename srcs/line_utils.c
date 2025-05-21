/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:51:29 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/19 16:52:39 by macauchy         ###   ########.fr       */
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
	double	dx_total;
	double	dy_total;
	double	dx_curr;
	double	dy_curr;

	dx_total = lp->b.x - lp->a.x;
	dy_total = lp->b.y - lp->a.y;
	dx_curr = lp->a.x - lp->start.x;
	dy_curr = lp->a.y - lp->start.y;
	lp->total_dist = sqrt(pow(dx_total, 2) + pow(dy_total, 2));
	curr_dist = sqrt(pow(dx_curr, 2) + pow(dy_curr, 2));
	if (lp->total_dist == 0)
		lp->t_param = 0;
	else
		lp->t_param = curr_dist / lp->total_dist;
	lp->current_z = lerp(lp->b.z, lp->a.z, lp->t_param);
}

void	draw_line(t_point a, t_point b, char *img_data)
{
	t_bresenham		bsh;
	t_line_params	lp;

	lp.a = a;
	lp.start = a;
	lp.b = b;
	init_bresenham(a, b, &bsh);
	while (1)
	{
		compute_line_params(&lp);
		set_pixel(img_data, (int)lp.a.x, (int)lp.a.y, lp.current_z);
		if (lp.a.x == lp.b.x && lp.a.y == lp.b.y)
			break ;
		step_pixel(&bsh, &lp.a);
	}
}
