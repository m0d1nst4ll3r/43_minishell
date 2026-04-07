/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:00:37 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/07 14:59:48 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(int ac, char **av, char **ep)
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
	free(pwd);
	return (0);
}
