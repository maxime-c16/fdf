/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection_styles.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:15:23 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/22 11:25:41 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static void	apply_perspective_projection(t_point *point, double x, double y, \
			double z)
{
	point->x = x / (1 - z / 1000);
	point->y = y / (1 - z / 1000);
}

static void	apply_oblique_projection(t_point *point, double x, double y, \
			double z)
{
	point->x = x + 0.5 * z * cos(M_PI / 6);
	point->y = y + 0.5 * z * sin(M_PI / 6);
}

static void	apply_cabinet_projection(t_point *point, double x, double y, \
			double z)
{
	point->x = 0.5 * x + z * cos(M_PI / 6);
	point->y = 0.5 * y + z * sin(M_PI / 6);
}

static void	apply_conic_projection(t_point *point, double x, double y, double z)
{
	point->x = x * z;
	point->y = y * z;
}

void	apply_proj(t_point *point, double x, double y, double z)
{
	t_fdf	*fdf;

	fdf = _fdf();
	if (fdf->proj_style == PROJ_ISOMETRIC)
		apply_isometric_projection(point, x, y, z);
	else if (fdf->proj_style == PROJ_PERSPECTIVE)
		apply_perspective_projection(point, x, y, z);
	else if (fdf->proj_style == PROJ_OBLIQUE)
		apply_oblique_projection(point, x, y, z);
	else if (fdf->proj_style == PROJ_CABINET)
		apply_cabinet_projection(point, x, y, z);
	else if (fdf->proj_style == PROJ_CONIC)
		apply_conic_projection(point, x, y, z);
	else
	{
		point->x = x;
		point->y = y;
	}
}
