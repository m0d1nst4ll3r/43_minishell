/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:24:43 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 19:47:03 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(int ac, char **av, char **ep)
{
	size_t	i;

	(void)ac;
	(void)av;
	if (!ep)
		return (0);
	i = 0;
	while (ep[i])
		printf("%s\n", ep[i++]);
	return (0);
}
