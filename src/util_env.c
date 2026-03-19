/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 14:21:20 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/19 14:21:39 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env(char *key, char **env)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (key[j] == env[i][j])
			j++;
		if (!key[j] && env[i][j] == '=')
			return (env[i] + j + 1);
		i++;
	}
	return (NULL);
}

size_t	get_env_size(char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

int	is_envar_char(char c)
{
	return (c == '_' || ft_isdigit(c) || ft_islower(c) || ft_isupper(c));
}
