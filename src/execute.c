/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:30:37 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/02 15:56:48 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*========================================================*/
/* CHILD PROCESS*/
/*========================================================*/

int		is_a_directory(char *path)
{
	struct stat s;

	stat(path, &s);
	if (S_ISDIR(s.st_mode))
		return (1);
	return (0);
}

char	*search_in_paths(char *cmd, char **paths)
{
	int		i;
	char	*tmp;
	char	*path;

	if (!cmd || !paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!path)
			return (NULL);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

char	**get_path_lst(char *cmd, char **envp)
{
	int		i;

	if (!cmd || !envp)
		return (NULL);
	i = 0;
	// if (ft_strchr(cmd, '/'))
	// {
	// 	if (access(cmd, F_OK) != 0)
	// 	{
	// 		print_error(cmd);
	// 		exit_prog(data, 127);
	// 	}
	// 	else if (is_a_directory(cmd))
	// 	{
	// 		ft_fprintf(2, "%s: %s: Is a directory\n", NAME, cmd);
	// 		exit_prog(data, 126);
	// 	}
	// 	else if (access(cmd, X_OK) != 0)
	// 	{
	// 		print_error(cmd);
	// 		exit_prog(data, 126);
	// 	}
	// 	else
	// 		return (ft_strdup(cmd));
	// }
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	return (ft_split(envp[i] + 5, ':'));
}

void child_process(t_minishell *data, t_command *cmd, char ***envp)
{
	char **path_lst;
	char *path;

	if (is_builtin(cmd->argv[0]))
		exit (exec_builtin(data, cmd, envp));
	path_lst = get_path_lst(cmd->argv[0], *envp);
	if (!path_lst)
	{
		ft_fprintf(2, "%s: %s: command not found\n", NAME, cmd->argv[0]);
		exit_prog(data, 127);
	}
	execve(path, cmd->argv, *envp);
	perror("execve");
	exit_prog(data, 1);
}

/*========================================================*/
int wait_process(pid_t pid)
{
	pid_t wpid;
	int status;
	int retval;

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

int exec_cmd(t_minishell *data, pid_t *pid_last_process, int **pipe_fd)
{
	t_command *cmd;
	pid_t pid;
	int idx;
	int nb_cmd;
	
	cmd = data->cmd_list;
	idx = 0;
	nb_cmd = count_cmd(cmd);
	while (cmd)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			clear_pipes(pipe_fd, nb_cmd - 1);
			return (0);
		}
		if (pid == 0)
		{
			if (reset_signal_handlers())
			{
				clear_pipes(pipe_fd, nb_cmd - 1);
				exit_prog(data, 1);
			}
			handle_pipes(pipe_fd, nb_cmd, idx);
			if (cmd->heredoc_fd != -1)
			{
				dup2(cmd->heredoc_fd, STDIN_FILENO);
				safe_close(&cmd->heredoc_fd);
			}
			handle_redir(cmd);
			clear_pipes(pipe_fd, nb_cmd - 1);
			child_process(data, cmd, &data->env);
		}
		else
		{
			safe_close(&cmd->heredoc_fd);
			cmd = cmd->next;
			idx++;
			*pid_last_process = pid;
		}
	}
	return (1);
}

int	execute(t_minishell *data)
{
	int			**pipe_fd;
	pid_t		pid;
	t_command	*cmd;
	int retval;
	
	pid = 0;
	cmd = data->cmd_list;
	if (!cmd)
		return (0);
	pipe_fd = create_pipes(count_cmd(cmd) - 1);
	if (!pipe_fd)
		return (0);
	if (!handle_heredoc(data))
		return (128 + g_signal);
	if (count_cmd(cmd) == 1 && is_builtin(cmd->argv[0]))
	{
		free(pipe_fd);
		retval = exec_one_builtin(data);
		safe_close(&cmd->heredoc_fd);
		return (retval);
	}
	if (!exec_cmd(data, &pid, pipe_fd))
		return (1);
	clear_pipes(pipe_fd, count_cmd(cmd) - 1);
	return (wait_process(pid));
}
