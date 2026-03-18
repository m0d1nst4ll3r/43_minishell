/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:30:10 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/17 15:51:28 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parse(t_minishell *d)
{
	t_token		*token_list;
	t_command	*cmd_list;

	token_list = tokenize(d);
	cmd_list = organize(token_list);
	if (!cmd_list)
		cleanup_token_list(token_list, 1);
	else
		cleanup_token_list(token_list, 0);
	return (cmd_list);
}
