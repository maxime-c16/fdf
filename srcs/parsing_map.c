/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:28:49 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/23 12:51:33 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

// static int	check_range(long value)
// {
// 	if (value > INT_MAX || value < INT_MIN)
// 		return (0);
// 	return (1);
// }

int	check_size(char *line)
{
	int		width;
	char	**split;

	split = ft_split(line, ' ');
	if (!split)
	{
		ft_putstr_fd("Error: ft_split() failed\n", 2);
		exit(1);
	}
	width = 0;
	while (split[width])
		width++;
	ft_free_tab(split);
	if (width != _fdf()->width)
		return (0);
	return (1);
}

static int	insert_values(char *line, int i)
{
	t_fdf	*fdf;
	char	**split;
	int		j;
	int		flag;

	j = 0;
	flag = 0;
	fdf = _fdf();
	split = ft_split(line, ' ');
	if (!split)
	{
		ft_putstr_fd("Error: ft_split() failed\n", 2);
		exit(1);
	}
	while (split[j])
	{
		flag = check_size(line);
		fdf->map[i][j] = (int)ft_atoi(split[j]);
		free(split[j]);
		j++;
	}
	fdf->width = j;
	free(split);
	return (flag);
}

int	read_from_fd(int fd)
{
	char	*line;
	int		i;
	int		flag;

	i = 0;
	flag = 1;
	line = get_next_line(fd);
	_fdf()->map = (int **)malloc(sizeof(int *) * _fdf()->height);
	if (!_fdf()->map)
		return (0);
	while (line)
	{
		_fdf()->map[i] = (int *)malloc(sizeof(int) * _fdf()->width);
		if (!_fdf()->map[i])
		{
			ft_putstr_fd("Error: malloc() failed\n", 2);
			exit(1);
		}
		flag = insert_values(line, i);
		i++;
		free(line);
		line = get_next_line(fd);
	}
	return (flag);
}
