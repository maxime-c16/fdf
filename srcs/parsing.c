/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:35:05 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/23 13:10:43 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	parsing(char *filename)
{
	t_fdf	*fdf;

	fdf = _fdf();
	count_lines(filename);
	fdf->fd = open(filename, O_RDONLY);
	if (fdf->fd < 0)
	{
		ft_putstr_fd("Error: open() failed\n", 2);
		exit(1);
	}
	if (!read_from_fd(fdf->fd))
	{
		ft_putstr_fd("Error: Invalid map data\n", 2);
		close(fdf->fd);
		on_close();
		exit(1);
	}
	close(fdf->fd);
	fdf->camera.zoom = HEIGHT / max(1, fdf->width);
	fdf->center_x = ((fdf->width - 1) * fdf->camera.zoom) / 2.0;
	fdf->center_y = ((fdf->height - 1) * fdf->camera.zoom) / 2.0;
}
