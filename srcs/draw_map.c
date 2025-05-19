#include "../includes/fdf.h"

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
