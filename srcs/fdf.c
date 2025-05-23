/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:54:29 by mcauchy           #+#    #+#             */
/*   Updated: 2025/05/23 15:21:37 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

int	str_is_numeric(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]) && str[i] != ' ' && str[i] != '-' \
				&& str[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

static int	open_and_parse(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error: Could not open file\n", 2);
		return (0);
	}
	close(fd);
	parsing(filename);
	return (1);
}

static int	check_map_format(char *filename)
{
	int		fd;
	char	*line;
	int		flag;

	flag = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error: Could not open file\n", 2);
		return (0);
	}
	line = get_next_line(fd);
	while (line)
	{
		if (!str_is_numeric(line))
			flag = 1;
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	return (flag == 0);
}

static void	setup_hooks(t_fdf *fdf)
{
	mlx_hook(fdf->win, 17, 0, on_close, 0);
	mlx_hook(fdf->win, ON_KEYDOWN, 1L << 0, key_hook, 0);
	mlx_hook(fdf->win, 4, 1L << 2, mouse_press, 0);
	mlx_hook(fdf->win, 5, 1L << 3, mouse_release, 0);
	mlx_hook(fdf->win, 6, 1L << 6, mouse_move, 0);
}

int	main(int ac, char **av)
{
	t_fdf	*fdf;

	fdf = NULL;
	if (ac != 2)
	{
		ft_putstr_fd("Usage: ./fdf <map>.fdf\n", 2);
		return (1);
	}
	if (!check_map_format(av[1]))
		return (1);
	if (!open_and_parse(av[1]))
		return (1);
	fdf = _fdf();
	compute_height_factor(fdf);
	draw_map();
	setup_hooks(fdf);
	mlx_loop(fdf->mlx);
	return (0);
}
