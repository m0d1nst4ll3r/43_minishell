/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 13:42:29 by bdemouge          #+#    #+#             */
/*   Updated: 2026/03/25 13:45:09 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	safe_close(int *fd)
{
	if (*fd == -1)
		return ;
	close(*fd);
	*fd = -1;
}