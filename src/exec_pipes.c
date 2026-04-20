/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:33:20 by bdemouge          #+#    #+#             */
/*   Updated: 2026/04/20 20:18:34 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_pipes(int ***pipe_fd)
{
	int	i;

	i = 0;
	if (!*pipe_fd)
		return ;
	while ((*pipe_fd)[i])
	{
		safe_close((*pipe_fd)[i]);
		safe_close((*pipe_fd)[i] + 1);
		free((*pipe_fd)[i]);
		i++;
	}
	ft_free((void **)pipe_fd);
}

int	**create_pipes(t_minishell *data, int nb_pipes)
{
	int	**pipe_fd;
	int	i;

	pipe_fd = malloc(sizeof(int *) * (nb_pipes + 1));
	if (!pipe_fd)
		error_out(data, ERR_MALLOC);
	ft_memset(pipe_fd, 0, sizeof(*pipe_fd) * (nb_pipes + 1));
	i = -1;
	while (++i < nb_pipes)
	{
		pipe_fd[i] = malloc(sizeof(int) * 2);
		if (!pipe_fd[i])
			error_out(data, ERR_MALLOC);
		pipe_fd[i][0] = -1;
		pipe_fd[i][1] = -1;
		if (pipe(pipe_fd[i]) == -1)
			error_out(data, ERR_PIPE);
	}
	pipe_fd[i] = NULL;
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
