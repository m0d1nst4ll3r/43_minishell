/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:30:37 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/14 15:45:14 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_process(pid_t pid)
{
	pid_t	wpid;
	int		status;
	int		retval;

	retval = 130;
	wpid = 1;
	while (wpid > 0)
	{
		wpid = waitpid(-1, &status, 0);
		if (wpid == pid)
		{
			if (WIFEXITED(status))
				retval = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				retval = 128 + WTERMSIG(status);
		}
	}
	return (retval);
}

static void	exec_child(t_minishell *data, t_command *cmd, t_exec *exec, int idx)
{
	if (reset_signal_handlers())
	{
		clear_pipes(exec->pipe_fd, exec->nb_cmd - 1);
		exit_prog(data, 1);
	}
	handle_pipes(exec->pipe_fd, exec->nb_cmd, idx);
	clear_pipes(exec->pipe_fd, exec->nb_cmd - 1);
	if (cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		safe_close(&cmd->heredoc_fd);
	}
	if (!handle_redir(cmd))
		exit_prog(data, 1);
	child_process(data, cmd);
}

static void	exec_parent(t_command **cmd, pid_t *pid_last_process, pid_t pid,
		int *idx)
{
	safe_close(&(*cmd)->heredoc_fd);
	*cmd = (*cmd)->next;
	(*idx)++;
	*pid_last_process = pid;
}

static int	exec_cmd(t_minishell *data, t_exec *exec)
{
	pid_t		pid;
	int			idx;

	idx = 0;
	while (exec->cmd)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			clear_pipes(exec->pipe_fd, exec->nb_cmd - 1);
			return (0);
		}
		if (pid == 0)
			exec_child(data, exec->cmd, exec, idx);
		else
			exec_parent(&exec->cmd, &exec->last_pid, pid, &idx);
	}
	return (1);
}

int	execute(t_minishell *data)
{
	t_exec	exec;

	exec.last_pid = 0;
	exec.cmd = data->cmd_list;
	if (!exec.cmd)
		return (0);
	exec.nb_cmd = count_cmd(exec.cmd);
	exec.pipe_fd = create_pipes(exec.nb_cmd - 1);
	if (!exec.pipe_fd)
		return (0);
	if (!handle_heredoc(data))
	{
		clear_pipes(exec.pipe_fd, exec.nb_cmd - 1);
		return (128 + g_signal);
	}
	if (exec.nb_cmd == 1 && is_builtin(exec.cmd->argv[0]))
	{
		clear_pipes(exec.pipe_fd, exec.nb_cmd - 1);
		return (exec_one_builtin(data));
	}
	if (!exec_cmd(data, &exec))
		return (1);
	clear_pipes(exec.pipe_fd, exec.nb_cmd - 1);
	return (wait_process(exec.last_pid));
}
