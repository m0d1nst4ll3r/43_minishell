/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand_fill.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 16:39:27 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 17:22:30 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_last_return(unsigned char last_return, size_t *len,
		char *exp_line)
{
	unsigned char	h;
	unsigned char	t;
	unsigned char	u;

	h = last_return / 100;
	t = (last_return / 10) % 10;
	u = last_return % 10;
	if (h)
		exp_line[(*len)++] = h + '0';
	if (h || t)
		exp_line[(*len)++] = t + '0';
	exp_line[(*len)++] = u + '0';
}

static void	write_expanded_envar(t_minishell *d, size_t *i, size_t *len,
		char *exp_line)
{
	size_t	key_len;
	char	*envar_value;

	(*i)++;
	if (d->line[*i] == '?')
	{
		write_last_return(d->last_return, len, exp_line);
		(*i)++;
	}
	else
	{
		key_len = 0;
		while (is_envar_char(d->line[*i + key_len]))
			key_len++;
		envar_value = get_env_n(d->line + *i, d->env, key_len);
		if (envar_value)
		{
			ft_strcpy(exp_line + *len, envar_value);
			*len += ft_strlen(envar_value);
		}
		*i += key_len;
	}
}

void	fill_expanded_line(t_minishell *d, char *exp_line)
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
			write_expanded_envar(d, &i, &len, exp_line);
		else
			exp_line[len++] = d->line[i++];
	}
	exp_line[len] = 0;
}
