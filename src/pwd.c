/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:00:37 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 19:09:24 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	pwd(int ac, char **av, char **ep)
{
	char	*pwd;

	(void)ac;
	(void)ep;
	pwd = getcwd(NULL, 0);
	if (pwd)
		printf("%s\n", pwd);
	else
	{
		print_error_builtin(av[0], ERR_PWD);
		return (1);
	}
	return (0);
}
