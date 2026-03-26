/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:37:41 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/26 11:59:13 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_malloc(size_t size)
{
	void	*p;

	p = malloc(size);
	if (!p)
		print_error(ERR_MALLOC);
	return (p);
}
