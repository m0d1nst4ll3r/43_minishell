/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 12:58:13 by bdemouge          #+#    #+#             */
/*   Updated: 2026/04/20 18:16:51 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process(t_minishell *data, t_command *cmd)
{
	char	*path;

	if (!cmd->argv[0][0])
	{
		print_error_builtin(cmd->argv[0], ERR_NOCMD);
		exit_prog(data, 127);
	}
	if (is_builtin(cmd->argv[0]))
		exit_prog(data, exec_builtin(data, cmd, &data->env));
	path = get_path(data, cmd);
	check_access(data, cmd, path);
	execve(path, cmd->argv, data->env);
	perror("execve");
	free(path);
	exit_prog(data, 1);
}
