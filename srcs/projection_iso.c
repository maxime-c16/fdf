/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection_iso.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:45:52 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/20 15:45:59 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	apply_isometric_projection(t_point *point, double x, double y, double z)
{
	point->x = (x - y) * cos(M_PI / 6);
	point->y = (x + y) * sin(M_PI / 6) - z;
}
