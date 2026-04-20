/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 20:19:52 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/20 20:20:26 by rapohlen         ###   ########.fr       */
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

void	print_error_exit(char *arg, char *err_str)
{
	if (!err_str)
		err_str = ERR_DEFAULT;
	if (arg)
		ft_fprintf(2, "%s: exit: %s: %s\n", NAME, arg, err_str);
	else
		ft_fprintf(2, "%s: exit: %s\n", NAME, err_str);
}

void	error_out(t_minishell *data, char *err_str)
{
	print_error(err_str);
	exit_prog(data, 1);
}
