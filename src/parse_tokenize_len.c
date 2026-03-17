/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokenize_len.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:43:47 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/17 17:42:05 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	get_expanded_envar_len(t_minishell *d, size_t *i, size_t *len)
{
	size_t	key_len;

	(*i)++;
	if (d->line[*i] == '?')
	{
		*len += get_uchar_len(d->last_return);
		(*i)++;
	}
	else
	{
		key_len = 0;
		while (is_envar_char(d->line[*i + key_len]))
			key_len++;
		*len += get_envar_len(d->line + *i, key_len, d->env);
		*i += key_len;
	}
}

static size_t	check_state(t_parse_state state, size_t len, int had_quotes)
{
	if (state != STATE_NONE)
	{
		if (state == STATE_QUOTE)
			print_error(ERR_UNFQUOTE);
		if (state == STATE_DQUOTE)
			print_error(ERR_UNFDQUOTE);
		return (-2);
	}
	if (!len && !had_quotes)
		return (-1);
	return (len);
}

static int	is_valid_envar_syntax(t_parse_state state, char *line)
{
	return (state != STATE_QUOTE && line[0] == '$' && (line[1] == '?'
			|| (!ft_isdigit(line[1]) && is_envar_char(line[1]))));
}

int	get_word_len(t_minishell *d, size_t i)
{
	t_parse_state	state;
	size_t			len;
	int				had_quotes;

	state = STATE_NONE;
	had_quotes = 0;
	len = 0;
	while (!is_end_of_word(d->line[i], state))
	{
		if ((d->line[i] == '\'' && state != STATE_DQUOTE)
			|| (d->line[i] == '"' && state != STATE_QUOTE))
		{
			had_quotes = 1;
			update_state(d->line[i], &state, &i);
		}
		else if (is_valid_envar_syntax(state, d->line + i))
			get_expanded_envar_len(d, &i, &len);
		else
		{
			i++;
			len++;
		}
	}
	return (check_state(state, len, had_quotes));
}
