/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 18:49:44 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/24 14:45:04 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_option(char *str)
{
	int	i;

	if (str[0] != '-' || !str[1])
		return (0);
	i = 1;
	while (str[i])
		if (str[i++] != 'n')
			return (0);
	return (1);
}

int	builtin_echo(int ac, char **av, char **ep)
{
	int	option_n;
	int	i;

	(void)ac;
	(void)ep;
	option_n = 0;
	i = 1;
	while (av[i])
	{
		if (is_option(av[i]))
			option_n = 1;
		else
			break ;
		i++;
	}
	while (av[i])
	{
		printf("%s", av[i]);
		if (av[++i])
			printf(" ");
	}
	if (!option_n)
		printf("\n");
	return (0);
}
