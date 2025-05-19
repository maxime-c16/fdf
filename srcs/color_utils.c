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
