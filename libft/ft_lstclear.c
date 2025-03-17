/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcauchy <mcauchy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 13:17:43 by maximecauch       #+#    #+#             */
/*   Updated: 2025/02/09 18:59:32 by mcauchy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **alst, void (*del)(void *))
{
	t_list	*ptr;

	if (!del || !alst || !*alst)
		return ;
	while (*alst && alst)
	{
		ptr = (*alst)->next;
		ft_lstdelone(*alst, del);
		*alst = ptr;
	}
	return ;
}
