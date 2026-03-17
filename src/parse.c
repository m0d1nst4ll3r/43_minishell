/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:30:10 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/17 12:35:17 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*organize(t_token *token_list)
{
	(void)token_list;
	return (NULL);
}

t_command	*parse(t_minishell *d)
{
	t_token		*token_list;
	t_command	*cmd_list;

	token_list = tokenize(d);
	cmd_list = organize(token_list);
	cleanup_token_list(token_list);
	return (cmd_list);
}
