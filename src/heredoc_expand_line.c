/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 11:46:59 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/26 12:10:41 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_expanded_line(char *line, t_minishell *d, char *new_line)
{
	size_t	len;
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (is_valid_envar_syntax(line + i))
			i += write_expanded_envar(line + i, d, &len, new_line);
		else
			new_line[len++] = line[i++];
	}
}

static size_t	get_new_len(char *line, t_minishell *d)
{
	size_t	len;
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (is_valid_envar_syntax(line + i))
			get_expanded_envar_len(line, d, &i, &len);
		else
		{
			i++;
			len++;
		}
	}
	return (len);
}

char	*expand_line(char *line, t_minishell *d)
{
	size_t	new_len;
	char	*new_line;

	new_len = get_new_len(line, d);
	new_line = malloc(new_len + 1);
	if (!new_line)
		return (NULL); // Will call error_out if we ever change
	fill_expanded_line(line, d, new_line);
	return (new_line);
}
