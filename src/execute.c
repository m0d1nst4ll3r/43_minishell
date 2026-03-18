/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:30:37 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 18:57:08 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* UTILS */
/*========================================================*/
int	count_cmd(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		cmd = cmd->next;
		i++;
	}
	return (1);
}

void	safe_close(int fd)
{
	if (fd == -1)
		return ;
	close(fd);
	fd = -1;
}

/*========================================================*/
/* GESTION PIPES */
/*========================================================*/
void	clear_pipes(int **pipe_fd, int nb_pipes)
{
	int	i;

	i = 0;
	while (i < nb_pipes)
	{
		safe_close(pipe_fd[i][0]);
		safe_close(pipe_fd[i][1]);
		free(pipe_fd[i]);
		i++;
	}
	free(pipe_fd);
}
int	**create_pipes(int nb_pipes)
{
	int	**pipe_fd;
	int	i;

	pipe_fd = malloc(sizeof(int *) * nb_pipes);
	if (!pipe_fd)
		return (perror("malloc"), NULL); /* gerer erreur*/
	i = 0;
	while (i < nb_pipes)
	{
		pipe_fd[i] = malloc(sizeof(int) * 2);
		if (!pipe_fd[i])
		{
			clear_pipes(pipe_fd, i);
			return (NULL);
		}
		pipe_fd[i][0] = -1;
		pipe_fd[i][1] = -1;
		if (pipe(pipe_fd[i]) == -1)
		{
			clear_pipes(pipe_fd, i);
			return (NULL);
		}
		i++;
	}
	return (pipe_fd);
}

void	handle_pipes(int **pipe_fd, int nb_cmd, int idx) /*(CHILD PROCESSs)*/
{
	if (idx == 0 && nb_cmd > 1)
	{
		safe_close(pipe_fd[idx][0]);
		dup2(pipe_fd[idx][1], STDOUT_FILENO);
		safe_close(pipe_fd[idx][1]);
	}
	else if (idx == nb_cmd - 1)
	{
		safe_close(pipe_fd[idx - 1][1]);
		dup2(pipe_fd[idx - 1][0], STDIN_FILENO);
		safe_close(pipe_fd[idx - 1][0]);
	}
	else
	{
		safe_close(pipe_fd[idx - 1][1]);
		safe_close(pipe_fd[idx][0]);
		dup2(pipe_fd[idx - 1][0], STDIN_FILENO);
		safe_close(pipe_fd[idx - 1][0]);
		dup2(pipe_fd[idx][1], STDOUT_FILENO);
		safe_close(pipe_fd[idx][1]);
	}
}

/*========================================================*/
/* GESTION REDIR (CHILD PROCESS) !!! PAS DE GESTION DES ERREURS !!!*/
/*========================================================*/
void	handle_redir_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return ; //(clean + exit avec le bon code)
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	handle_redir_out(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		return ; //(clean + exit avec le bon code)
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	handle_redir_append(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
		return ; //(clean + exit avec le bon code)
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	handle_redir_heredoc(char *limiter)
{
	(void)limiter;
}

void	handle_redir(t_command *cmd)
{
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			handle_redir_in(redir->file);
		else if (redir->type == REDIR_OUT)
			handle_redir_out(redir->file);
		else if (redir->type == REDIR_APPEND)
			handle_redir_append(redir->file);
		else if (redir->type == REDIR_HEREDOC)
			handle_redir_heredoc(redir->file);
	}
}

/*========================================================*/
int	execute(t_minishell *data)
{
	int			**pipe_fd;
	int			nb_cmd;
	int			idx;
	pid_t		pid;
	t_command	*cmd;

	cmd = data->cmd_list;
	if (!cmd)
		return (0);
	nb_cmd = count_cmd(cmd);
	pipe_fd = create_pipes(nb_cmd - 1);
	if (!pipe_fd)
		return (0);
	idx = 0;
	while (cmd)
	{
		pid = fork();
		if (pid == -1)
		{
			clear_pipes(pipe_fd, nb_cmd - 1);
			return (0);
		}
		if (pid == 0)
		{
			handle_pipes(pipe_fd, nb_cmd, idx);
			handle_redir(cmd);
		}
		cmd = cmd->next;
		idx++;
	}
	return (1);
}
