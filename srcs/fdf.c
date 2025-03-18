/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:54:29 by mcauchy           #+#    #+#             */
/*   Updated: 2025/03/18 14:58:36 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	draw_line(t_point a, t_point b)
{
	t_fdf	*fdf;
	int		dx;
	int		dy;
	int		sx;
	int		sy;

	fdf = _fdf();
	dx = abs(b.x - a.x);
	dy = abs(b.y - a.y);
	sx = a.x < b.x ? 1 : -1;
	sy = a.y < b.y ? 1 : -1;
	if (dx > dy)
	{
		int d = (dy << 1) - dx;
		int d1 = dy << 1;
		int d2 = (dy - dx) << 1;
		mlx_pixel_put(fdf->mlx, fdf->win, a.x, a.y, 0xFFFFFF);
		for (int x = a.x + sx, y = a.y, i = 1; i <= dx; i++, x += sx)
		{
			if (d > 0)
			{
				d += d2;
				y += sy;
			}
			else
				d += d1;
			mlx_pixel_put(fdf->mlx, fdf->win, x, y, 0xFFFFFF);
		}
	}
	else
	{
		int d = (dx << 1) - dy;
		int d1 = dx << 1;
		int d2 = (dx - dy) << 1;
		mlx_pixel_put(fdf->mlx, fdf->win, a.x, a.y, 0xFFFFFF);
		for (int x = a.x, y = a.y + sy, i = 1; i <= dy; i++, y += sy)
		{
			if (d > 0)
			{
				d += d2;
				x += sx;
			}
			else
				d += d1;
			mlx_pixel_put(fdf->mlx, fdf->win, x, y, 0xFFFFFF);
		}
	}
}

void	draw_triangle(t_point a, t_point b, t_point c)
{
	draw_line(a, b);
	draw_line(b, c);
	draw_line(c, a);
}

int	on_close(void)
{
	exit(0);
}

int	main(int ac, char **av)
{
	t_fdf	*fdf;

	fdf = _fdf();
	draw_triangle((t_point){100, 100}, (t_point){200, 100}, (t_point){150, 200});
	draw_triangle((t_point){300, 100}, (t_point){400, 100}, (t_point){350, 200});
	draw_triangle((t_point){500, 100}, (t_point){600, 100}, (t_point){550, 200});
	mlx_hook(fdf->win, 17, 0, on_close, 0);
	mlx_loop(fdf->mlx);
	return (0);
}
