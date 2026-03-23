/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:30:10 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 17:00:58 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parse(t_minishell *d)
{
	char		*expanded_line;
	t_token		*token_list;
	t_command	*cmd_list;

	expanded_line = get_expanded_line(d);
	if (!expanded_line)
		return (NULL);
	token_list = tokenize(expanded_line);
	cmd_list = organize(token_list);
	if (!cmd_list)
		cleanup_token_list(token_list, 1);
	else
		cleanup_token_list(token_list, 0);
	return (cmd_list);
}
