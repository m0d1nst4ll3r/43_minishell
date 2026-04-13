/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 14:19:13 by bdemouge          #+#    #+#             */
/*   Updated: 2026/04/13 16:46:12 by bdemouge         ###   ########.fr       */
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

static void	handle_redir_in(t_minishell *data, t_redir *redir, char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		print_error(file);
		exit_prog(data, 1);
	}
	if (!check_heredoc(redir))
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

static void	handle_redir_out(t_minishell *data, char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		print_error(file);
		exit_prog(data, 1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void	handle_redir_append(t_minishell *data, char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
	{
		print_error(file);
		exit_prog(data, 1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	handle_redir(t_minishell *data, t_command *cmd)
{
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			handle_redir_in(data, redir, redir->file);
		else if (redir->type == REDIR_OUT)
			handle_redir_out(data, redir->file);
		else if (redir->type == REDIR_APPEND)
			handle_redir_append(data, redir->file);
		redir = redir->next;
	}
}