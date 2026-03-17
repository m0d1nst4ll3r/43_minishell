/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:40:29 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/17 16:37:05 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_env(char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	cleanup_token_list(t_token *token_list, int clean_contents)
{
	t_token	*last;

	while (token_list)
	{
		last = token_list;
		token_list = token_list->next;
		if (clean_contents)
			free(last->word);
		free(last);
	}
}

static void	cleanup_argv(char **argv, int clean_contents)
{
	size_t	i;

	if (clean_contents)
	{
		i = 0;
		while (argv[i])
		{
			free(argv[i]);
			i++;
		}
	}
	free(argv);
}

static void	cleanup_redir_list(t_redir *redir_list, int clean_contents)
{
	t_redir	*last;

	while (redir_list)
	{
		last = redir_list;
		redir_list = redir_list->next;
		if (clean_contents)
			free(last->file);
		free(last);
	}
}

void	cleanup_cmd_list(t_command *cmd_list, int clean_contents)
{
	t_command	*last;

	while (cmd_list)
	{
		last = cmd_list;
		cmd_list = cmd_list->next;
		cleanup_argv(last->argv, clean_contents);
		cleanup_redir_list(last->redir, clean_contents);
		free(last);
	}
}
