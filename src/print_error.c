/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:29:41 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/19 16:18:01 by rapohlen         ###   ########.fr       */
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

void	print_error_builtin(char *name, char *err_str)
{
	if (!err_str)
		err_str = ERR_DEFAULT;
	if (errno)
		ft_fprintf(2, "%s: %s: %s: %s\n", NAME, name, err_str, strerror(errno));
	else
		ft_fprintf(2, "%s: %s: %s\n", NAME, name, err_str);
}

void	print_error_builtin_file(char *name, char *file, char *err_str)
{
	if (!err_str)
		err_str = ERR_DEFAULT;
	if (errno)
		ft_fprintf(2, "%s: %s: %s: %s: %s\n",
			NAME, name, file, err_str, strerror(errno));
	else
		ft_fprintf(2, "%s: %s: %s: %s\n", NAME, name, file, err_str);
}
