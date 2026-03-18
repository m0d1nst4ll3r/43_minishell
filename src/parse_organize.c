/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_organize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 15:47:59 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 16:50:49 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_state_organize(t_parse_state *state, t_token_type token)
{
	if (token == TOKEN_PIPE)
		*state = STATE_PIPE;
	else if (token < 4)
		*state = STATE_REDIR;
	else
		*state = STATE_WORD;
}

static int	is_valid_token_list(t_token *token_list)
{
	t_parse_state	state;

	state = STATE_BEGIN;
	while (token_list)
	{
		if ((state == STATE_BEGIN && token_list->type == TOKEN_PIPE)
			|| (state == STATE_PIPE && token_list->type == TOKEN_PIPE)
			|| (state == STATE_REDIR && token_list->type != TOKEN_WORD))
			return (0);
		update_state_organize(&state, token_list->type);
		token_list = token_list->next;
	}
	if (state == STATE_PIPE || state == STATE_REDIR)
		return (0);
	return (1);
}

static size_t	get_argv_count(t_token *token_list)
{
	t_parse_state	state;
	size_t			count;

	state = STATE_BEGIN;
	count = 0;
	while (token_list && token_list->type != TOKEN_PIPE)
	{
		if (state != STATE_REDIR && token_list->type == TOKEN_WORD)
			count++;
		update_state_organize(&state, token_list->type);
		token_list = token_list->next;
	}
	return (count);
}

static size_t	get_redir_count(t_token *token_list)
{
	size_t	count;

	count = 0;
	while (token_list && token_list->type != TOKEN_PIPE)
	{
		if (token_list->type < 4)
			count++;
		token_list = token_list->next;
	}
	return (count);
}

t_command	*organize(t_token *token_list)
{
	t_command	*cmd_list;
	t_command	*last;
	size_t		argv_count;

	if (!is_valid_token_list(token_list))
	{
		print_error(ERR_SYNTAX);
		return (NULL);
	}
	cmd_list = NULL;
	while (token_list)
	{
		argv_count = get_argv_count(token_list);
		if ((!argv_count && !get_redir_count(token_list))
			|| !create_new_cmd(&cmd_list, argv_count, &last)
			|| !fill_cmd(&token_list, last))
		{
			cleanup_cmd_list(cmd_list, 0);
			return (NULL);
		}
		if (token_list)
			token_list = token_list->next;
	}
	return (cmd_list);
}
