#include "../includes/fdf.h"

static void	get_axis_coords(
	t_gyro *g, double *x, double *y, double *z)
{
	if (g->axis == 'x')
	{
		*x = 0;
		*y = g->rad * cos(g->angle);
		*z = g->rad * sin(g->angle);
	}
	else if (g->axis == 'y')
	{
		*x = g->rad * cos(g->angle);
		*y = 0;
		*z = g->rad * sin(g->angle);
	}
	else
	{
		*x = g->rad * cos(g->angle);
		*y = g->rad * sin(g->angle);
		*z = 0;
	}
}

static void	apply_rot_proj(
	t_gyro *g, double *x, double *y, double *z, int *px, int *py)
{
	t_point	proj;

	apply_y_rotation(x, z, *g->ry);
	apply_x_rotation(y, z, *g->rx);
	apply_z_rotation(x, y, *g->rz);
	apply_proj(&proj, *x, *y, *z);
	*px = proj.x + g->cx;
	*py = proj.y + g->cy;
}

void	get_gyro_point(t_gyro *g, int *px, int *py)
{
	double	x;
	double	y;
	double	z;

	get_axis_coords(g, &x, &y, &z);
	apply_rot_proj(g, &x, &y, &z, px, py);
}
