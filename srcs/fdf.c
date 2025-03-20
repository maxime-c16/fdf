/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:54:29 by mcauchy           #+#    #+#             */
/*   Updated: 2025/03/20 22:36:37 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static double clamp(double t)
{
	if (t < 0.0)
		t = 0.0;
	if (t > 1.0)
		t = 1.0;
	return (t);
}

static double lerp(double a, double b, double t)
{
	return (a + t * (b - a));
}

static int interpolate_color(int col1, int col2, double t)
{
	int r1 = (col1 >> 16) & 0xFF;
	int g1 = (col1 >> 8) & 0xFF;
	int b1 = col1 & 0xFF;
	int r2 = (col2 >> 16) & 0xFF;
	int g2 = (col2 >> 8) & 0xFF;
	int b2 = col2 & 0xFF;
	t = clamp(t);
	int r = (int)lerp(r1, r2, t);
	int g = (int)lerp(g1, g2, t);
	int b = (int)lerp(b1, b2, t);
	return (r << 16) | (g << 8) | b;
}

int linear_altitude_color(int altitude, int min_alt, int max_alt)
{
	double t;
	int start_color;
	int end_color;

	t = (max_alt == min_alt) ? 0.0 : ((double)(altitude - min_alt)) /\
		 (max_alt - min_alt);
	start_color = 0xF0AAFF;
	end_color = 0x0000FF;
	return (interpolate_color(start_color, end_color, t));
}

void	draw_line(t_point a, t_point b)
{
	t_fdf	*fdf;
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	int		e2;
	t_point	start;
	double	total_dist;
	double	curr_dist;
	double	t_param;
	double	current_z;

	fdf = _fdf();
	start = a;
	dx = abs(b.x - a.x);
	dy = abs(b.y - a.y);
	sx = a.x < b.x ? 1 : -1;
	sy = a.y < b.y ? 1 : -1;
	err = (dx > dy ? dx : -dy) / 2;
	total_dist = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
	while (1)
	{
		curr_dist = sqrt(pow(a.x - start.x, 2) + pow(a.y - start.y, 2));
		if (total_dist == 0)
			t_param = 0;
		else
			t_param = curr_dist / total_dist;
		current_z = lerp(a.z, b.z, t_param);
		mlx_pixel_put(fdf->mlx, fdf->win, a.x, a.y, linear_altitude_color(current_z, fdf->min_altitude, fdf->max_altitude));
		if (a.x == b.x && a.y == b.y)
			break ;
		e2 = err;
		if (e2 > -dx)
		{
			err -= dy;
			a.x += sx;
		}
		if (e2 < dy)
		{
			err += dx;
			a.y += sy;
		}
	}
}

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

void compute_height_factor(t_fdf *fdf)
{
	int min_alt;
	int max_alt;
	int i;
	int	j;

	i = 0;
	fdf->min_altitude = INT_MAX;
	fdf->max_altitude = INT_MIN;
	while (i < fdf->height)
	{
		j = 0;
		while (j < fdf->width)
		{
			if (fdf->map[i][j] < fdf->min_altitude)
				fdf->min_altitude = fdf->map[i][j];
			if (fdf->map[i][j] > fdf->max_altitude)
				fdf->max_altitude = fdf->map[i][j];
			j++;
		}
		i++;
	}
	fdf->camera.height_factor = (fdf->camera.zoom\
			/ max(1, fdf->max_altitude - min_alt)) * 5.0;
}

void	draw_map(void)
{
	t_fdf	*fdf;
	int		i;
	int		j;
	t_point	a;
	t_point	b;

	i = 0;
	fdf = _fdf();
	while (i < fdf->height)
	{
		j = 0;
		while (j < fdf->width)
		{
			a = project_point(i, j);
			if (j + 1 < fdf->width)
			{
				b = project_point(i, j + 1);
				draw_line(a, b);
			}
			if (i + 1 < fdf->height)
			{
				b = project_point(i + 1, j);
				draw_line(a, b);
			}
			j++;
		}
		i++;
	}
}

int	on_close(void)
{
	exit(0);
}

int	key_hook(int keycode)
{
	if (keycode == 53)
		exit(0);
	if (keycode == LEFT_ARROW)
		_fdf()->camera.x_offset -= 10;
	if (keycode == RIGHT_ARROW)
		_fdf()->camera.x_offset += 10;
	if (keycode == DOWN_ARROW)
		_fdf()->camera.y_offset += 10;
	if (keycode == UP_ARROW)
		_fdf()->camera.y_offset -= 10;
	if (keycode == ADD_X_ROTATE)
		_fdf()->camera.rotation_x += 0.1;
	if (keycode == SUB_X_ROTATE)
		_fdf()->camera.rotation_x -= 0.1;
	if (keycode == ADD_Y_ROTATE)
		_fdf()->camera.rotation_y += 0.1;
	if (keycode == SUB_Y_ROTATE)
		_fdf()->camera.rotation_y -= 0.1;
	if (keycode == ADD_Z_ROTATE)
		_fdf()->camera.rotation_z += 0.1;
	if (keycode == SUB_Z_ROTATE)
		_fdf()->camera.rotation_z -= 0.1;
	mlx_clear_window(_fdf()->mlx, _fdf()->win);
	draw_map();
	return (0);
}

int	main(int ac, char **av)
{
	t_fdf	*fdf;

	if (ac != 2)
	{
		ft_putstr_fd("Usage: ./fdf <filename>\n", 2);
		return (1);
	}
	parsing(av[1]);
	fdf = _fdf();
	compute_height_factor(fdf);
	draw_map();
	mlx_hook(fdf->win, 17, 0, on_close, 0);
	mlx_hook(fdf->win, 2, 0, key_hook, 0);
	mlx_loop(fdf->mlx);
	return (0);
}
