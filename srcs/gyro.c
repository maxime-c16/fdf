/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gyro.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 11:12:13 by macauchy          #+#    #+#             */
/*   Updated: 2025/03/23 11:12:34 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

// Helper function to draw a simple 2D line using Bresenham’s algorithm.
void draw_simple_line(void *mlx, void *win, int x0, int y0, int x1, int y1, int color)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;
	int e2;

	while (1)
	{
		mlx_pixel_put(mlx, win, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = err * 2;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

// Helper function to draw a circle using the midpoint circle algorithm.
void draw_circle(void *mlx, void *win, int cx, int cy, int radius, int color)
{
	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y)
	{
		mlx_pixel_put(mlx, win, cx + x, cy + y, color);
		mlx_pixel_put(mlx, win, cx + y, cy + x, color);
		mlx_pixel_put(mlx, win, cx - y, cy + x, color);
		mlx_pixel_put(mlx, win, cx - x, cy + y, color);
		mlx_pixel_put(mlx, win, cx - x, cy - y, color);
		mlx_pixel_put(mlx, win, cx - y, cy - x, color);
		mlx_pixel_put(mlx, win, cx + y, cy - x, color);
		mlx_pixel_put(mlx, win, cx + x, cy - y, color);
		y++;
		if (err <= 0)
		{
			err += 2 * y + 1;
		}
		if (err > 0)
		{
			x--;
			err -= 2 * x + 1;
		}
	}
}

// Draws a gyroscope in the top-right corner showing the three rotation axes.
void draw_gyroscope(t_fdf *fdf)
{
	// Define the gyroscope's position and radius.
	int cx = WIDTH - 70;  // Center X in MLX window (adjust as needed)
	int cy = 70;          // Center Y in MLX window (adjust as needed)
	int radius = 50;      // Outer circle radius

	// Draw the outer circle (white)
	draw_circle(fdf->mlx, fdf->win, cx, cy, radius, 0xFFFFFF);

	// Compute endpoints for each axis line.
	// For simplicity, use the camera rotation angles directly.
	// You may add an offset to spread them apart if desired.
	int x_end, y_end;
	double r = (double)radius * 0.8; // length of axis line

	// X-axis (red)
	x_end = cx + (int)(r * cos(fdf->camera.rotation_x));
	y_end = cy + (int)(r * sin(fdf->camera.rotation_x));
	draw_simple_line(fdf->mlx, fdf->win, cx, cy, x_end, y_end, 0xFF0000);

	// Y-axis (green)
	x_end = cx + (int)(r * cos(fdf->camera.rotation_y));
	y_end = cy + (int)(r * sin(fdf->camera.rotation_y));
	draw_simple_line(fdf->mlx, fdf->win, cx, cy, x_end, y_end, 0x00FF00);

	// Z-axis (blue)
	x_end = cx + (int)(r * cos(fdf->camera.rotation_z));
	y_end = cy + (int)(r * sin(fdf->camera.rotation_z));
	draw_simple_line(fdf->mlx, fdf->win, cx, cy, x_end, y_end, 0x0000FF);
}

