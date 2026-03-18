/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 17:44:40 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 18:49:07 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static unsigned char	cd_home(char *name, char **ep)
{
	char	*home;

	home = get_env("HOME", ep);
	if (!home)
	{
		print_error_builtin(name, ERR_CDHOME);
		return (1);
	}
	if (chdir(home))
	{
		print_error_builtin(name, home);
		return (1);
	}
	return (0);
}

// cd built-in
// needs environment to search for HOME when used without args
// does not change PWD or OLDPWD
unsigned char	cd(int ac, char **av, char **ep)
{
	if (ac > 2)
	{
		print_error_builtin(av[0], ERR_CDARGS);
		return (1);
	}
	else if (ac == 1)
		return (cd_home(av[0], ep));
	else
		if (chdir(av[1]))
		{
			print_error_builtin(av[0], av[1]);
			return (1);
		}
	return (0);
}
