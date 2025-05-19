/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:54:29 by mcauchy           #+#    #+#             */
/*   Updated: 2025/05/19 13:14:42 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static double	clamp(double t)
{
	if (t < 0.0)
		t = 0.0;
	if (t > 1.0)
		t = 1.0;
	return (t);
}

double	lerp(double a, double b, double t)
{
	return (a * t + b * (1.0 - t));
}

static int	interpolate_color(int col1, int col2, double t)
{
	int	r;
	int	g;
	int	b;

	t = clamp(t);
	r = (int)lerp((col1 >> 16) & 0xFF, (col2 >> 16) & 0xFF, t);
	g = (int)lerp((col1 >> 8) & 0xFF, (col2 >> 8) & 0xFF, t);
	b = (int)lerp(col1 & 0xFF, col2 & 0xFF, t);
	return ((r << 16) | (g << 8) | b);
}

int	linear_altitude_color(int altitude, int min_alt, int max_alt)
{
	double	t;
	int		start_color;
	int		end_color;

	if (max_alt == min_alt)
		t = 0.0;
	else
		t = ((double)(altitude - min_alt)) / (max_alt - min_alt);
	start_color = 0xF0AAFF;
	end_color = 0x0000FF;
	return (interpolate_color(start_color, end_color, t));
}

void	set_pixel(char *data, int x, int y, int current_z)
{
	t_fdf	*fdf;
	int		*buf;
	int		offset;
	int		color;

	fdf = _fdf();
	buf = (int *)data;
	offset = y * WIDTH + x;
	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		color = linear_altitude_color(current_z, fdf->min_altitude,
				fdf->max_altitude);
		buf[offset] = color;
	}
}

static double	calc_total_dist(t_point a, t_point b)
{
	return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
}

static double	calc_t_param(t_point a, t_point start, double total_dist)
{
	double	curr_dist;

	curr_dist = sqrt(pow(a.x - start.x, 2) + pow(a.y - start.y, 2));
	if (total_dist == 0)
		return (0);
	return (curr_dist / total_dist);
}

static void	draw_line_step(t_point *a, int *err, int dx, int dy, int sx, int sy)
{
	int e2;

	e2 = *err;
	if (e2 > -dx)
	{
		*err -= dy;
		a->x += sx;
	}
	if (e2 < dy)
	{
		*err += dx;
		a->y += sy;
	}
}

static int	get_sign(int a, int b)
{
	if (a < b)
		return (1);
	return (-1);
}

static int	get_err(int dx, int dy)
{
	if (dx > dy)
		return (dx / 2);
	return (-dy / 2);
}

void	draw_line(t_point a, t_point b, char *img_data)
{
	t_line	line;
	double	t_param;
	double	current_z;
	int		dx;
	int		dy;

	line.a = a;
	line.b = b;
	line.start = a;
	line.sx = get_sign(a.x, b.x);
	line.sy = get_sign(a.y, b.y);
	dx = abs(b.x - a.x);
	dy = abs(b.y - a.y);
	line.err = get_err(dx, dy);
	line.total_dist = calc_total_dist(a, b);
	while (1)
	{
		t_param = calc_t_param(line.a, line.start, line.total_dist);
		current_z = lerp(line.b.z, line.a.z, t_param);
		set_pixel(img_data, (int)line.a.x, (int)line.a.y, current_z);
		if (line.a.x == line.b.x && line.a.y == line.b.y)
			break;
		draw_line_step(&line.a, &line.err, dx, dy, line.sx, line.sy);
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
	fdf->camera.height_factor = ((double)fdf->camera.zoom
	/ max(1, fdf->max_altitude - fdf->min_altitude)) * 2.0;
}

char	*ft_dtoa(double n, int precision)
{
	char	*str;
	char	*tmp;
	int		int_part;
	int		dec_part;
	int		i;

	int_part = (int)n;
	dec_part = (int)((n - int_part) * pow(10, precision));
	str = ft_itoa(int_part);
	tmp = ft_strjoin(str, ".");
	free(str);
	str = ft_strjoin(tmp, ft_itoa(dec_part));
	free(tmp);
	i = ft_strlen(str) - 1;
	while (str[i] == '0')
	{
		str[i] = '\0';
		i--;
	}
	if (str[i] == '.')
		str[i] = '\0';
	return (str);
}

static void	draw_map_lines(t_drawmap_ctx *ctx, int i, int j)
{
	t_point	a, b, gyro_a, gyro_b;

	a = project_point(i, j);
	gyro_a = project_point_scaled(i, j);
	if (j + 1 < ctx->fdf->width)
	{
		b = project_point(i, j + 1);
		gyro_b = project_point_scaled(i, j + 1);
		draw_line(a, b, ctx->fdf->img_data);
		draw_line(gyro_a, gyro_b, ctx->fdf->img_data);
	}
	if (i + 1 < ctx->fdf->height)
	{
		b = project_point(i + 1, j);
		gyro_b = project_point_scaled(i + 1, j);
		draw_line(a, b, ctx->fdf->img_data);
		draw_line(gyro_a, gyro_b, ctx->fdf->img_data);
	}
}

static void	draw_map_rows(t_drawmap_ctx *ctx)
{
	int j;
	int j_start;
	int j_step;

	j_start = (ctx->fdf->camera.rotation_y > 0) ? ctx->fdf->width - 1 : 0;
	j_step = (ctx->fdf->camera.rotation_y > 0) ? -1 : 1;
	j = j_start;
	while (j < ctx->fdf->width && j >= 0)
	{
		draw_map_lines(ctx, ctx->i, j);
		j += j_step;
	}
}

void	draw_map(void)
{
	t_drawmap_ctx ctx;
	int i_start;
	int i_step;

	ctx.fdf = _fdf();
	ctx.fdf->img_data = mlx_get_data_addr(ctx.fdf->img, &ctx.fdf->bpp, &ctx.fdf->size_line, &ctx.fdf->endian);
	ft_bzero(ctx.fdf->img_data, ctx.fdf->size_line * HEIGHT);
	i_start = (ctx.fdf->camera.rotation_x > 0) ? ctx.fdf->height - 1 : 0;
	i_step = (ctx.fdf->camera.rotation_x > 0) ? -1 : 1;
	ctx.i = i_start;
	while (ctx.i < ctx.fdf->height && ctx.i >= 0)
	{
		draw_map_rows(&ctx);
		ctx.i += i_step;
	}
	draw_gyroscope_sphere(ctx.fdf);
	mlx_put_image_to_window(ctx.fdf->mlx, ctx.fdf->win, ctx.fdf->img, 0, 0);
}

int	on_close(void)
{
	ft_free_int_tab(_fdf()->map, _fdf()->height);
	mlx_destroy_image(_fdf()->mlx, _fdf()->img);
	mlx_destroy_window(_fdf()->mlx, _fdf()->win);
	mlx_destroy_display(_fdf()->mlx);
	free(_fdf()->mlx);
	exit(0);
}

static void	handle_arrow_keys(int keycode)
{
	if (keycode == LEFT_ARROW)
		_fdf()->camera.x_offset -= 10;
	else if (keycode == RIGHT_ARROW)
		_fdf()->camera.x_offset += 10;
	else if (keycode == DOWN_ARROW)
		_fdf()->camera.y_offset += 10;
	else if (keycode == UP_ARROW)
		_fdf()->camera.y_offset -= 10;
}

static void	handle_rotation_keys(int keycode)
{
	if (keycode == ADD_X_ROTATE)
		_fdf()->camera.rotation_x += 0.1;
	else if (keycode == SUB_X_ROTATE)
		_fdf()->camera.rotation_x -= 0.1;
	else if (keycode == ADD_Y_ROTATE)
		_fdf()->camera.rotation_y += 0.1;
	else if (keycode == SUB_Y_ROTATE)
		_fdf()->camera.rotation_y -= 0.1;
	else if (keycode == ADD_Z_ROTATE)
		_fdf()->camera.rotation_z += 0.1;
	else if (keycode == SUB_Z_ROTATE)
		_fdf()->camera.rotation_z -= 0.1;
}

static void	handle_zoom_key(int keycode)
{
	if (keycode == ADD_ZOOM)
		_fdf()->camera.zoom += 1;
	else if (keycode == SUB_ZOOM)
		_fdf()->camera.zoom -= 1;
}

static void	handle_proj_key(int keycode)
{
	if (keycode == CH_PROJ)
	{
		_fdf()->proj_style++;
		if (_fdf()->proj_style >= 6)
			_fdf()->proj_style = 0;
	}
}

static void	handle_reset_key(int keycode)
{
	if (keycode == RESET)
	{
		_fdf()->camera.zoom = (WIDTH + HEIGHT) / 40;
		_fdf()->camera.x_offset = WIDTH / 2;
		_fdf()->camera.y_offset = HEIGHT / 2;
		_fdf()->camera.rotation_x = 0.0;
		_fdf()->camera.rotation_y = 0.0;
		_fdf()->camera.rotation_z = 0.0;
		_fdf()->proj_style = PROJ_ISOMETRIC;
	}
}

int	key_hook(int keycode)
{
	if (keycode == ESC)
		on_close();
	handle_arrow_keys(keycode);
	handle_rotation_keys(keycode);
	handle_zoom_key(keycode);
	handle_proj_key(keycode);
	handle_reset_key(keycode);
	mlx_clear_window(_fdf()->mlx, _fdf()->win);
	draw_map();
	return (0);
}

int	main(int ac, char **av)
{
	t_fdf	*fdf;
	int		fd;

	if (ac != 2)
	{
		ft_putstr_fd("Usage: ./fdf <filename>\n", 2);
		return (1);
	}
	fd = open(av[1], O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error: Could not open file\n", 2);
		return (1);
	}
	close(fd);
	parsing(av[1]);
	fdf = _fdf();
	compute_height_factor(fdf);
	draw_map();
	mlx_hook(fdf->win, 17, 0, on_close, 0);
	mlx_hook(fdf->win, ON_KEYDOWN, 1L << 0, key_hook, 0);
	mlx_hook(fdf->win, 4, 1L << 0, mouse_press, 0);
	mlx_hook(fdf->win, 5, 0, mouse_release, 0);
	mlx_hook(fdf->win, 6, 0, mouse_move, 0);
	mlx_loop(fdf->mlx);
	return (0);
}
