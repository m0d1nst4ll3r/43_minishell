/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_organize_fill.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 15:51:44 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 16:05:04 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_new_cmd(t_command **cmd_list, size_t argv_count, t_command **last)
{
	t_command	*new_cmd;

	new_cmd = ft_malloc(sizeof(*new_cmd));
	if (!new_cmd)
		return (0);
	new_cmd->argv = ft_malloc(sizeof(*new_cmd->argv) * (argv_count + 1));
	if (!new_cmd->argv)
	{
		free(new_cmd);
		return (0);
	}
	ft_memset(new_cmd->argv, 0, sizeof(*new_cmd->argv) * (argv_count + 1));
	new_cmd->redir = NULL;
	new_cmd->next = NULL;
	if (!*cmd_list)
		*cmd_list = new_cmd;
	else
		(*last)->next = new_cmd;
	*last = new_cmd;
	return (1);
}

static int	add_redir(t_command *cmd, t_token *token_list, t_redir **last)
{
	t_redir	*new;

	new = ft_malloc(sizeof(*new));
	if (!new)
		return (0);
	new->type = (t_redir_type)token_list->type;
	new->file = token_list->next->word;
	new->next = NULL;
	if (!cmd->redir)
		cmd->redir = new;
	else
		(*last)->next = new;
	*last = new;
	return (1);
}

int	fill_cmd(t_token **token_list, t_command *cmd)
{
	size_t	argv_count;
	t_redir	*last;

	argv_count = 0;
	while (*token_list && (*token_list)->type != TOKEN_PIPE)
	{
		if ((*token_list)->type < 4 && !add_redir(cmd, *token_list, &last))
			return (0);
		else if ((*token_list)->type == TOKEN_WORD)
			cmd->argv[argv_count++] = (*token_list)->word;
		if ((*token_list)->type < 4)
			*token_list = (*token_list)->next;
		*token_list = (*token_list)->next;
	}
	return (1);
}
