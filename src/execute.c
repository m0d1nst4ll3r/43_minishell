/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:30:37 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/21 14:30:02 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_process(pid_t pid)
{
	pid_t	wpid;
	int		status;
	int		retval;

	retval = 1;
	wpid = 1;
	while (wpid > 0)
	{
		wpid = waitpid(-1, &status, 0);
		if (wpid == pid)
		{
			if (WIFEXITED(status))
				retval = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				write(1, "\n", 1);
				retval = 128 + WTERMSIG(status);
			}
		}
	}
	return (retval);
}

static void	exec_child(t_minishell *data, t_command *cmd, int idx)
{
	data->forked = true;
	if (reset_signal_handlers())
	{
		clear_pipes(&(data->exec.pipe_fd));
		exit_prog(data, 1);
	}
	handle_pipes(data->exec.pipe_fd, data->exec.nb_cmd, idx);
	clear_pipes(&(data->exec.pipe_fd));
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

static int	exec_cmd(t_minishell *data)
{
	pid_t		pid;
	int			idx;

	idx = 0;
	while (data->exec.cmd)
	{
		pid = fork();
		if (pid == -1)
		{
			error_out(data, ERR_FORK);
		}
		if (pid == 0)
			exec_child(data, data->exec.cmd, idx);
		else
			exec_parent(&data->exec.cmd, &data->exec.last_pid, pid, &idx);
	}
	return (1);
}

int	execute(t_minishell *data)
{
	data->exec.last_pid = 0;
	data->exec.cmd = data->cmd_list;
	if (!data->exec.cmd)
		return (0);
	data->exec.nb_cmd = count_cmd(data->exec.cmd);
	data->exec.pipe_fd = create_pipes(data, data->exec.nb_cmd - 1);
	if (!data->exec.pipe_fd)
		return (0);
	if (!handle_heredoc(data))
	{
		clear_pipes(&(data->exec.pipe_fd));
		return (128 + g_signal);
	}
	if (unset_sigint())
		error_out(data, ERR_SIGNAL);
	if (data->exec.nb_cmd == 1 && is_builtin(data->exec.cmd->argv[0]))
	{
		clear_pipes(&(data->exec.pipe_fd));
		return (exec_one_builtin(data));
	}
	if (!exec_cmd(data))
		return (1);
	clear_pipes(&(data->exec.pipe_fd));
	printf("");
	return (wait_process(data->exec.last_pid));
}
