/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 14:19:13 by bdemouge          #+#    #+#             */
/*   Updated: 2026/04/14 12:27:58 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int check_heredoc(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
			return (1);
		redir = redir->next;
	}
	return (0);
}

static int	handle_redir_in(t_redir *redir, char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		print_error(file);
		return (0);
	}
	if (!check_heredoc(redir))
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (1);
}

static int	handle_redir_out(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		print_error(file);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

static int	handle_redir_append(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
	{
		print_error(file);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	handle_redir(t_command *cmd)
{
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (!handle_redir_in(redir, redir->file))
				return (0);
		}
		else if (redir->type == REDIR_OUT)
		{
			if (!handle_redir_out(redir->file))
				return (0);
		}
		else if (redir->type == REDIR_APPEND)
		{
			if (!handle_redir_append(redir->file))
				return (0);
		}
		redir = redir->next;
	}
	return (1);
}
