/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_util_len.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 11:58:45 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/26 12:20:32 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_uchar_len(unsigned char u)
{
	if (u < 10)
		return (1);
	if (u < 100)
		return (2);
	return (3);
}

static size_t	get_envar_len(char *key, size_t key_len, char **env)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (j < key_len && env[i][j] == key[j])
			j++;
		if (j == key_len && env[i][j] == '=')
			return (ft_strlen(env[i]) - key_len - 1);
		i++;
	}
	return (0);
}

void	get_expanded_envar_len(char *line, t_minishell *d, size_t *i,
		size_t *len)
{
	size_t	key_len;

	(*i)++;
	if (line[*i] == '?')
	{
		*len += get_uchar_len(d->last_return);
		(*i)++;
	}
	else
	{
		key_len = 0;
		while (is_envar_char(line[*i + key_len]))
			key_len++;
		*len += get_envar_len(line + *i, key_len, d->env);
		*i += key_len;
	}
}
