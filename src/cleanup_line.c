/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:33:14 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/13 19:07:29 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_argv(char **argv)
{
	size_t			i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static void	cleanup_redir(t_redir *redir)
{
	t_redir *last;

	while (redir)
	{
		last = redir;
		redir = redir->next;
		free(last->file);
		free(last);
	}
}

void	cleanup_line(char *line, t_command_list *cmd_list)
{
	t_command_list	*last;

	free(line);
	while (cmd_list)
	{
		last = cmd_list;
		cmd_list = cmd_list->next;
		cleanup_argv(last->argv);
		cleanup_redir(last->redir);
		free(last);
	}
}
