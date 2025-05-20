/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection_rotation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:14:52 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/20 15:15:03 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	apply_y_rotation(double *x, double *z, double theta)
{
	double x_new;
	double z_new;

	x_new = *x * cos(theta) + *z * sin(theta);
	z_new = *x * -sin(theta) + *z * cos(theta);
	*x = x_new;
	*z = z_new;
}

void	apply_x_rotation(double *y, double *z, double phi)
{
	double y_new;
	double z_new;

	y_new = *y * cos(phi) + *z * sin(phi);
	z_new = *y * -sin(phi) + *z * cos(phi);
	*y = y_new;
	*z = z_new;
}

void	apply_z_rotation(double *x, double *y, double psi)
{
	double x_new;
	double y_new;

	x_new = *x * cos(psi) - *y * sin(psi);
	y_new = *x * sin(psi) + *y * cos(psi);
	*x = x_new;
	*y = y_new;
}

