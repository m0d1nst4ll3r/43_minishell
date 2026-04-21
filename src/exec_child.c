/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 12:58:13 by bdemouge          #+#    #+#             */
/*   Updated: 2026/04/21 21:15:09 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_argc(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}

static void	exec_shell(t_minishell *data, t_command *cmd, char *path)
{
	int		i;
	char	**new_argv;

	new_argv = malloc(sizeof(*new_argv) * (get_argc(cmd->argv) + 1));
	if (!new_argv)
		error_out(data, ERR_MALLOC);
	new_argv[0] = _PATH_BSHELL;
	new_argv[1] = path;
	i = 0;
	while (cmd->argv[++i])
		new_argv[i + 1] = cmd->argv[i];
	execve(_PATH_BSHELL, new_argv, data->env);
	free(new_argv);
}

void	child_process(t_minishell *data, t_command *cmd)
{
	char	*path;

	if (!cmd->argv[0])
		exit_prog(data, 0);
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
	if (errno == ENOEXEC)
		exec_shell(data, cmd, path);
	print_error("execve");
	free(path);
	exit_prog(data, 1);
}
