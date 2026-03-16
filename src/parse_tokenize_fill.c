/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokenize_fill.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:58:24 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/16 22:38:02 by rapohlen         ###   ########.fr       */
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

static void	write_envar(char *key, char **env, char *word, size_t *len)
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

static void	write_expanded_envar(t_minishell *d, size_t *i, size_t *len,
		char *word)
{
	size_t	key_len;

	(*i)++;
	if (d->line[*i] == '?')
	{
		write_last_return(d->last_return, word, len);
		(*i)++;
	}
	else
	{
		key_len = 0;
		while (is_envar_char(d->line[*i + key_len]))
			key_len++;
		write_envar(d->line + *i, d->env, word, len);
		*i += key_len;
	}
}

void	fill_word(t_minishell *d, size_t *i, char *word)
{
	t_parse_state	state;
	size_t			len;

	state = STATE_NONE;
	len = 0;
	while (!is_end_of_word(d->line[*i], state))
	{
		if ((d->line[*i] == '\'' && state != STATE_DQUOTE)
			|| (d->line[*i] == '"' && state != STATE_QUOTE))
			update_state(d->line[*i], &state, i);
		else if (state != STATE_QUOTE && d->line[*i] == '$'
			&& (d->line[*i + 1] == '?'
				|| (!ft_isdigit(d->line[*i + 1])
					&& is_envar_char(d->line[*i + 1]))))
			write_expanded_envar(d, i, &len, word);
		else
			word[len++] = d->line[(*i)++];
	}
}
