/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:30:37 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/23 16:30:16 by bdemouge         ###   ########.fr       */
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
	return (i);
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

void	handle_pipes(int **pipe_fd, int nb_cmd, int idx)
{
	if (nb_cmd == 1)
		return ;
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
/* GESTION HEREDOC (PARENT PROCESS) !!! PAS DE GESTION DES ERREURS !!!*/
/*========================================================*/

int exec_heredoc(char *limiter)
{
	int fd[2];
	char *line;

	if(pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		write(1, ">", 1);
		line = get_next_line(0);
		if (line == NULL)
			break ;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (ft_strncmp(limiter, line, ft_strlen(limiter)) == 0 && ft_strlen(limiter) == ft_strlen(line))
			break ;
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
	}
	close(fd[1]);
	return (fd[0]);
}

int handle_heredoc(t_redir *redir)
{
	int fd;

	fd = -1;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			fd = exec_heredoc(redir->file);
		}
		redir = redir->next;
	}
	return (fd);
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

void	handle_redir(t_command *cmd)
{
	t_redir	*redir;

	//printf("handle_redir\n");
	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			handle_redir_in(redir->file);
		else if (redir->type == REDIR_OUT)
			handle_redir_out(redir->file);
		else if (redir->type == REDIR_APPEND)
			handle_redir_append(redir->file);
		redir = redir->next;
	}
}

/*========================================================*/
/* BUILT-IN*/
/*========================================================*/

int is_builtin(char *cmd)
{
	if (ft_strncmp("echo", cmd, ft_strlen("echo")) == 0 && ft_strlen("echo") == ft_strlen(cmd))
		return (1);
	else if (ft_strncmp("cd", cmd, ft_strlen("cd")) == 0 && ft_strlen("cd") == ft_strlen(cmd))
		return (1);
	else if (ft_strncmp("pwd", cmd, ft_strlen("pwd")) == 0 && ft_strlen("pwd") == ft_strlen(cmd))
		return (1);
	// else if (ft_strncmp("export", cmd, ft_strlen("export")) && ft_strlen("export") == ft_strlen(cmd))
	// 	return (1);
	else if (ft_strncmp("unset", cmd, ft_strlen("unset")) == 0 && ft_strlen("unset") == ft_strlen(cmd))
		return (1);
	else if (ft_strncmp("env", cmd, ft_strlen("env")) == 0 && ft_strlen("env") == ft_strlen(cmd))
		return (1);
	// else if (ft_strncmp("exit", cmd, ft_strlen("exit")) && ft_strlen("exit") == ft_strlen(cmd))
	// 	return (1);
	return (0);
}

int exec_builtin(t_command *cmd, char ***ep)
{
	int ac;

	ac = 0;
	while (cmd->argv[ac])
		ac++;
	if (ft_strncmp("echo", cmd->argv[0], ft_strlen("echo")) == 0)
		return (builtin_echo(ac, cmd->argv, *ep));
	else if (ft_strncmp("cd", cmd->argv[0], ft_strlen("cd")) == 0)
		return (builtin_cd(ac, cmd->argv, *ep));
	else if (ft_strncmp("pwd", cmd->argv[0], ft_strlen("pwd")) == 0)
		return (builtin_pwd(ac, cmd->argv, *ep));
	// else if (ft_strncmp("export", cmd->argv[0], ft_strlen("export")) == 0)
	// 	return (builtin_export(ac, cmd->argv, ep));
	else if (ft_strncmp("unset", cmd->argv[0], ft_strlen("unset")) == 0)
		return (builtin_unset(ac, cmd->argv, ep));
	else if (ft_strncmp("env", cmd->argv[0], ft_strlen("env")) == 0)
		return (builtin_env(ac, cmd->argv, *ep));
	// else if (ft_strncmp("exit", cmd->argv[0], ft_strlen("exit")))
	// 	return (1);
	return (1);
}

/*========================================================*/
/* CHILD PROCESS*/
/*========================================================*/



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

char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	**paths;

	if (!cmd || !envp)
		return (NULL);
	i = 0;
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) != 0)
			exit(127); //clean avant
		else if (access(cmd, X_OK) != 0)
			exit(126); //clean avant
		else
			return (ft_strdup(cmd));
	}
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL); //pas de PATH dans l'envp
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(cmd, paths));
}

void child_process(t_command *cmd, char ***envp)
{
	char *path;

	if (is_builtin(cmd->argv[0]))
		exit (exec_builtin(cmd, envp));
	path = get_path(cmd->argv[0], *envp);
	if (!path) //path not found
	{
		//clean_all;
		ft_fprintf(2, "command not found: %s\n", cmd->argv[0]);
		exit (127);
	}
	execve(path, cmd->argv, *envp);
	perror("execve");
	//clean
	exit (1);
}


/*========================================================*/
int wait_process(pid_t pid)
{
	pid_t wpid;
	int status;
	int retval;

	wpid = 1;
	retval = 1;
	while (wpid > 0)
	{
		wpid = waitpid(-1, &status, 0);
		if (wpid == pid)
		{
			if (WIFEXITED(status))
				retval = WEXITSTATUS(status);
			else
				retval = 1;
		}
	}
	return (retval);
}

typedef struct s_exec
{
	int **pipe_fd;
	int nb_cmd;
	int idx;
	int heredoc_fd;
	pid_t pid;
	t_command *cmd;
	
}	t_exec;

int	execute(t_minishell *data)
{
	int			**pipe_fd;
	int			nb_cmd;
	int			idx;
	pid_t		pid;
	t_command	*cmd;
	int heredoc_fd;
	
	cmd = data->cmd_list;
	if (!cmd)
		return (0);
	nb_cmd = count_cmd(cmd);
	pipe_fd = create_pipes(nb_cmd - 1);
	if (!pipe_fd)
		return (0);
	if (nb_cmd == 1 && is_builtin(cmd->argv[0]))
	{
		return (exec_builtin(cmd, &data->env));
	}
	idx = 0;
	while (cmd)
	{
		heredoc_fd = handle_heredoc(cmd->redir);
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			clear_pipes(pipe_fd, nb_cmd - 1);
			return (0);
		}
		if (pid == 0)
		{
			if (heredoc_fd != -1)
			{
				dup2(heredoc_fd, STDIN_FILENO);
				safe_close(heredoc_fd);
			}
			handle_pipes(pipe_fd, nb_cmd, idx);
			handle_redir(cmd);
			child_process(cmd, &data->env);
		}
		else
		{
			safe_close(heredoc_fd);
			cmd = cmd->next;
			idx++;
		}
	}
	clear_pipes(pipe_fd, nb_cmd - 1);
	return (wait_process(pid));
}
