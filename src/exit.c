/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 11:38:07 by bdemouge          #+#    #+#             */
/*   Updated: 2026/03/26 12:03:37 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long	atol_safe(const char *str)
{
	int 	i;
	int 	n;
	long	result;
    long      lim[2];

	i = 0;
	n = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			n = -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
        lim[0] = LONG_MAX / 10 - ((str[i] - 48) * n);
        if (lim > )
		result = result * 10 + ((str[i] - 48) * n);
		i++;
	}
	return (result);
}

int builtin_exit(t_minishell *data, char *status)
{
    int last_return;

    last_return = data->last_return;
    
}