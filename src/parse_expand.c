/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 16:06:37 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 17:22:30 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_expanded_envar_len(t_minishell *d, size_t *i, size_t *len)
{
	size_t	key_len;
	char	*envar_value;

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
		envar_value = get_env_n(d->line + *i, d->env, key_len);
		if (envar_value)
			*len += ft_strlen(envar_value);
		*i += key_len;
	}
}

static size_t	get_expanded_len(t_minishell *d)
{
	size_t			i;
	size_t			len;
	t_parse_state	state;

	i = 0;
	len = 0;
	state = STATE_NONE;
	while (d->line[i])
	{
		if (d->line[i] == '\'' && state == STATE_NONE)
			state = STATE_QUOTE;
		else if (d->line[i] == '\'' && state == STATE_QUOTE)
			state = STATE_NONE;
		if (state == STATE_NONE && d->line[i] == '$'
				&& (d->line[i + 1] == '?' || (!ft_isdigit(d->line[i + 1])
						&& is_envar_char(d->line[i + 1]))))
			get_expanded_envar_len(d, &i, &len);
		else
		{
			i++;
			len++;
		}
	}
	return (len);
}

char	*get_expanded_line(t_minishell *d)
{
	size_t	len;
	char	*exp_line;

	len = get_expanded_len(d);
	exp_line = ft_malloc(len + 1);
	if (!exp_line)
		return (NULL);
	fill_expanded_line(d, exp_line);
	return (exp_line);
}
