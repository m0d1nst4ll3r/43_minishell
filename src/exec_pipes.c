/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:33:20 by bdemouge          #+#    #+#             */
/*   Updated: 2026/03/25 16:42:23 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_pipes(int **pipe_fd, int nb_pipes)
{
	int	i;

	i = 0;
    if (!pipe_fd)
    {
        return ;
    }
	while (i < nb_pipes)
	{
		safe_close(&pipe_fd[i][0]);
		safe_close(&pipe_fd[i][1]);
        if (pipe_fd[i])
        {
		    free(pipe_fd[i]);
            pipe_fd[i] = NULL;
        }
		i++;
	}
	free(pipe_fd);
}

int	**create_pipes(int nb_pipes)
{
	int	**pipe_fd;
	int	i;

	pipe_fd = ft_malloc(sizeof(int *) * nb_pipes);
	if (!pipe_fd)
		return (NULL);
	i = -1;
	while (++i < nb_pipes)
	{
		pipe_fd[i] = ft_malloc(sizeof(int) * 2);
		if (!pipe_fd[i])
		{
			clear_pipes(pipe_fd, i);
			return (NULL);
		}
		pipe_fd[i][0] = -1;
		pipe_fd[i][1] = -1;
		if (pipe(pipe_fd[i]) == -1)
		{
			print_error("pipe error");
			clear_pipes(pipe_fd, i);
			return (NULL);
		}
	}
	return (pipe_fd);
}

void	handle_pipes(int **pipe_fd, int nb_cmd, int idx)
{
	if (nb_cmd == 1)
		return ;
	if (idx == 0 && nb_cmd > 1)
	{
		safe_close(&pipe_fd[idx][0]);
		dup2(pipe_fd[idx][1], STDOUT_FILENO);
		safe_close(&pipe_fd[idx][1]);
	}
	else if (idx == nb_cmd - 1)
	{
		safe_close(&pipe_fd[idx - 1][1]);
		dup2(pipe_fd[idx - 1][0], STDIN_FILENO);
		safe_close(&pipe_fd[idx - 1][0]);
	}
	else
	{
		safe_close(&pipe_fd[idx - 1][1]);
		safe_close(&pipe_fd[idx][0]);
		dup2(pipe_fd[idx - 1][0], STDIN_FILENO);
		safe_close(&pipe_fd[idx - 1][0]);
		dup2(pipe_fd[idx][1], STDOUT_FILENO);
		safe_close(&pipe_fd[idx][1]);
	}
}