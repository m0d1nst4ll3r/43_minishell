/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:30:10 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/10 17:30:42 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parse(t_minishell *d)
{
	t_token		*token_list;
	t_command	*cmd_list;

	token_list = tokenize(d);
	cmd_list = organize(token_list, &d->last_return);
	if (!cmd_list)
		cleanup_token_list(token_list, 1);
	else
		cleanup_token_list(token_list, 0);
	return (cmd_list);
}
