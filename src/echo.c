/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 18:49:44 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 19:46:56 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(int ac, char **av, char **ep)
{
	int	option_n;

	(void)ep;
	option_n = 0;
	if (ac > 1 && !ft_strcmp(av[1], "-n"))
	{
		av++;
		ac--;
		option_n = 1;
	}
	while (--ac)
	{
		printf("%s", av[1]);
		if (ac > 1)
			printf(" ");
		av++;
	}
	if (!option_n)
		printf("\n");
	return (0);
}
