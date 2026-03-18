/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:37:41 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 16:12:55 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_malloc(size_t size)
{
	void	*p;

	p = malloc(size);
	if (!p)
		print_error(ERR_MALLOC);
	return (p);
}

int	is_envar_char(char c)
{
	return (c == '_' || ft_isdigit(c) || ft_islower(c) || ft_isupper(c));
}

size_t	get_uchar_len(unsigned char u)
{
	if (u < 10)
		return (1);
	if (u < 100)
		return (2);
	return (3);
}

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

// Limited to n bytes, when the key string doesn't end with \0
char	*get_env_n(char *key, char **env, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (j < n && key[j] == env[i][j])
			j++;
		if (j == n && env[i][j] == '=')
			return (env[i] + j + 1);
		i++;
	}
	return (NULL);
}
