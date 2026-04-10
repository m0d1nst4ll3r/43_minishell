/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:39:51 by bdemouge          #+#    #+#             */
/*   Updated: 2026/04/10 15:12:57 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *read_heredoc_line(void)
{
	char *line;
	
	write(1, ">", 1);
	line = get_next_line(0);
	if (line == NULL)
		return (NULL);
	if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	return (line);
}

static void write_line(char *line, int fd, t_minishell *data)
{
	char *expand;

	expand = expand_line(line, data);
	if (expand)
	{
		free(line);
		line = expand;
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

static int exec_heredoc(char *limiter, t_minishell *data)
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
		if (g_signal == SIGINT)
			return (close(fd[0]), close(fd[1]), -1);
		line = read_heredoc_line();
		if (!line)
			break ;
		if (ft_strncmp(limiter, line, ft_strlen(limiter)) == 0 && ft_strlen(limiter) == ft_strlen(line))
		{
			free (line);
			break ;	
		}
		write_line(line, fd[1], data);
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
