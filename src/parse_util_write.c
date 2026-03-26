/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_util_write.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 11:55:45 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/26 12:16:27 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_last_return(unsigned char last_return, char *word,
		size_t *len)
{
	unsigned char	h;
	unsigned char	t;
	unsigned char	u;

	h = last_return / 100;
	t = (last_return / 10) % 10;
	u = last_return % 10;
	if (h)
		word[(*len)++] = h + '0';
	if (h || t)
		word[(*len)++] = t + '0';
	word[(*len)++] = u + '0';
}

static void	write_envar(char *key, char **env, size_t *len, char *word)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (is_envar_char(key[j]) && env[i][j] == key[j])
			j++;
		if (!is_envar_char(key[j]) && env[i][j] == '=')
		{
			j++;
			while (env[i][j])
				word[(*len)++] = env[i][j++];
			return ;
		}
		i++;
	}
}

size_t	write_expanded_envar(char *line, t_minishell *d, size_t *len,
		char *word)
{
	size_t	key_len;
	size_t	i;

	i = 1;
	if (line[i] == '?')
	{
		write_last_return(d->last_return, word, len);
		i++;
	}
	else
	{
		key_len = 0;
		while (is_envar_char(line[i + key_len]))
			key_len++;
		write_envar(line + i, d->env, len, word);
	}
	return (i + key_len);
}
