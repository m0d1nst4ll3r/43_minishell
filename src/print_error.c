/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:29:41 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/13 15:29:50 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *err_str)
{
	if (!err_str)
		err_str = ERR_DEFAULT;
	if (errno)
		ft_fprintf(2, "%s: %s: %s\n", NAME, err_str, strerror(errno));
	else
		ft_fprintf(2, "%s: %s\n", NAME, err_str);
}
