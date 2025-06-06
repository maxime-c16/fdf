/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:35:51 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/23 13:00:01 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

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
		if (fdf->flag == 1)
			color = fdf->color;
		else
			color = linear_altitude_color(current_z, fdf->min_altitude,
					fdf->max_altitude);
		buf[offset] = color;
	}
}

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

void	compute_height_factor(t_fdf *fdf)
{
	int		i;
	int		j;

	i = 0;
	fdf->scale = 2.0;
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
			/ max(1, fdf->max_altitude - fdf->min_altitude)) * fdf->scale;
}
