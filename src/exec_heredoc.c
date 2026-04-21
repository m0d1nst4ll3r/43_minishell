/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:39:51 by bdemouge          #+#    #+#             */
/*   Updated: 2026/04/21 20:01:23 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	event_hook(void)
{
	if (g_signal == SIGINT)
	{
		write(1, "^C", 2);
		rl_done = 1;
	}
	return (0);
}

static char	*read_heredoc_line(void)
{
	int		len;
	char	*line;

	rl_event_hook = event_hook;
	line = readline("> ");
	if (line == NULL)
		return (NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

static void	write_line(char *line, int fd, t_minishell *data, char *limiter)
{
	char	*expand;

	expand = NULL;
	if (!ft_strchr(limiter, '\'') && !ft_strchr(limiter, '\"'))
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

static int	exec_heredoc(char *limiter, t_minishell *data)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		error_out(data, ERR_PIPE);
	while (1)
	{
		line = read_heredoc_line();
		if (g_signal == SIGINT)
			return (close(fd[0]), close(fd[1]), -1);
		if (!line)
			break ;
		if (ft_strncmp(limiter, line, ft_strlen(limiter)) == 0
			&& ft_strlen(limiter) == ft_strlen(line))
		{
			free(line);
			break ;
		}
		write_line(line, fd[1], data, limiter);
	}
	close(fd[1]);
	return (fd[0]);
}

int	handle_heredoc(t_minishell *data)
{
	t_command	*cmd;
	t_redir		*redir;

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
				cmd->heredoc_fd = exec_heredoc(redir->file, data);
				if (cmd->heredoc_fd == -1)
					return (0);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (1);
}
