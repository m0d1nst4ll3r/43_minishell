/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:39:51 by bdemouge          #+#    #+#             */
/*   Updated: 2026/03/27 14:24:02 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int exec_heredoc(char *limiter)
{
	int fd[2];
	char *line;

	if(pipe(fd) == -1)
	{
		print_error(ERR_PIPE);
		return (-1);
	}
	while (1)
	{
		write(1, ">", 1);
		line = get_next_line(0);
		if (line == NULL)
			break ;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (ft_strncmp(limiter, line, ft_strlen(limiter)) == 0 && ft_strlen(limiter) == ft_strlen(line))
			break ;
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
	}
	close(fd[1]);
	return (fd[0]);
}

void handle_heredoc(t_command *cmd)
{
	t_redir *redir;

	while (cmd)
	{
		cmd->heredoc_fd = -1;
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				safe_close(&cmd->heredoc_fd);
				cmd->heredoc_fd	= exec_heredoc(redir->file);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}
