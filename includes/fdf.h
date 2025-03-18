/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:04:24 by macauchy          #+#    #+#             */
/*   Updated: 2025/03/18 14:52:33 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H

# define FDF_H

# include "../mlx/mlx.h"
# include "../libft/libft.h"

# define WIDTH 800
# define HEIGHT 600

typedef struct s_fdf
{
	void	*mlx;
	void	*win;
}			t_fdf;

typedef struct s_point
{
	int		x;
	int		y;
}			t_point;

t_fdf	*_fdf(void);

#endif
