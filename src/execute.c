/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:30:37 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 16:06:53 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_cmd(t_command *cmd)
{
	int i;

	i = 0;
	while (cmd)
	{
		cmd = cmd->next;
		i++;
	}
	return (1);
}

void clear_pipes(int **pipe_fd, int nb_pipes)
{
	int i;

	i = 0;
	while (i < nb_pipes)
	{
		close(pipe_fd[i][0]);
		close(pipe_fd[i][1]);
		free(pipe_fd[i]);
		i++;
	}
	free(pipe_fd);
}

int **create_pipes(int nb_pipes)
{
	int **pipe_fd;
	int i;
	
	pipe_fd = malloc(sizeof(int *) * nb_pipes);
	if (!pipe_fd)
		return (NULL); //gerer erreur
	i = 0;
	while (i < nb_pipes)
	{
		pipe_fd[i] = malloc(sizeof(int) * 2);
		if (!pipe_fd[i])
		{
			clear_pipes(pipe_fd, i);
			return (NULL);
		}
		if(pipe(pipe_fd[i]) == -1)
		{
			clear_pipes(pipe_fd, i);
			return (NULL);
		}	
		i++;
	}
	return (pipe_fd);
}

int	execute(t_minishell *data)
{
	int			**pipe_fd;
	pid_t		pid;
	t_command	*cmd;

	// cmd = data->cmd_list;
	// if (!cmd)
	// 	return (0);
	// while (cmd)
	// {
	// 	if (cmd->next)
	// 	{
	// 		if(pipe(pipe_fd) == -1)
	// 			return (perror("pipe"), 0);
	// 	}
	// 	pid = fork();
	// 	if (pid == -1)
	// 	{
	// 		perror("fork");
	// 		return (0);
	// 	}
	// 	if (pid == 0)
	// 	{
	// 		if (cmd->redir == NULL)
	// 		{
	// 			close(pipe_fd[0])
	// 		}
	// 	}
	// }
	return (0);
}
