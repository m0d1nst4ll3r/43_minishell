/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 14:19:13 by bdemouge          #+#    #+#             */
/*   Updated: 2026/03/27 14:20:23 by bdemouge         ###   ########.fr       */
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

static void	handle_redir_in(t_redir *redir, char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror(file);
		return ; //(clean + exit avec le bon code)
	}
	if (!check_heredoc(redir))
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

static void	handle_redir_out(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		perror(file);
		return ; //(clean + exit avec le bon code)
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void	handle_redir_append(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
	{
		perror(file);
		return ; //(clean + exit avec le bon code)
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	handle_redir(t_command *cmd)
{
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			handle_redir_in(redir, redir->file);
		else if (redir->type == REDIR_OUT)
			handle_redir_out(redir->file);
		else if (redir->type == REDIR_APPEND)
			handle_redir_append(redir->file);
		redir = redir->next;
	}
}