/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:39:51 by bdemouge          #+#    #+#             */
/*   Updated: 2026/03/30 14:24:03 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int exec_heredoc(char *limiter, t_minishell *data)
{
	int fd[2];
	char *line;
	char *expand = NULL;

	if(pipe(fd) == -1)
	{
		print_error(ERR_PIPE);
		return (-1);
	}
	while (1)
	{
		if (g_signal == SIGINT)
		{
			close(fd[0]);
			close(fd[1]);
			return (-1);
		}
		write(1, ">", 1);
		line = get_next_line(0);
		if (line == NULL)
			break ;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (ft_strncmp(limiter, line, ft_strlen(limiter)) == 0 && ft_strlen(limiter) == ft_strlen(line))
			break ;
		expand = expand_line(line, data);
		if (expand)
		{
			free(line);
			line = expand;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}

int handle_heredoc(t_minishell *data)
{
	t_command *cmd;
	t_redir *redir;

	cmd = data->cmd_list;
	while (cmd)
	{
		cmd->heredoc_fd = -1;
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				safe_close(&cmd->heredoc_fd);
				cmd->heredoc_fd	= exec_heredoc(redir->file, data);
				if (cmd->heredoc_fd == -1)
					return (0);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (1);
}
