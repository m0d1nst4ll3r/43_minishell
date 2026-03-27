/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 11:46:59 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/27 15:38:40 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_expanded_line(char *line, t_minishell *d, char *new_line)
{
	size_t	len;
	size_t	i;

	len = 0;
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
	//printf("new_len : %zu\n", new_len);
	new_line = malloc(new_len + 1);
	if (!new_line)
		return (NULL); // Will call error_out if we ever change
	if (new_len == 0)
	{
		new_line[0] = '\0';
		return (new_line);
	}
	fill_expanded_line(line, d, new_line);
	//printf("%s\n", new_line);
	return (new_line);
}
