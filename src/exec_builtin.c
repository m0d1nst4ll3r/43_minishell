/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 14:16:35 by bdemouge          #+#    #+#             */
/*   Updated: 2026/03/27 14:16:48 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *cmd)
{
	if (ft_strncmp("echo", cmd, ft_strlen("echo")) == 0 && ft_strlen("echo") == ft_strlen(cmd))
		return (1);
	else if (ft_strncmp("cd", cmd, ft_strlen("cd")) == 0 && ft_strlen("cd") == ft_strlen(cmd))
		return (1);
	else if (ft_strncmp("pwd", cmd, ft_strlen("pwd")) == 0 && ft_strlen("pwd") == ft_strlen(cmd))
		return (1);
	else if (ft_strncmp("export", cmd, ft_strlen("export")) == 0 && ft_strlen("export") == ft_strlen(cmd))
		return (1);
	else if (ft_strncmp("unset", cmd, ft_strlen("unset")) == 0 && ft_strlen("unset") == ft_strlen(cmd))
		return (1);
	else if (ft_strncmp("env", cmd, ft_strlen("env")) == 0 && ft_strlen("env") == ft_strlen(cmd))
		return (1);
	else if (ft_strncmp("exit", cmd, ft_strlen("exit")) == 0 && ft_strlen("exit") == ft_strlen(cmd))
		return (1);
	return (0);
}

int exec_builtin(t_minishell *data, t_command *cmd, char ***ep)
{
	int ac;
	int retval;

	ac = 0;
	handle_redir(cmd);
	retval = 1;
	while (cmd->argv[ac])
		ac++;
	if (ft_strncmp("echo", cmd->argv[0], ft_strlen("echo")) == 0)
		retval = builtin_echo(ac, cmd->argv, *ep);
	else if (ft_strncmp("cd", cmd->argv[0], ft_strlen("cd")) == 0)
		retval = builtin_cd(ac, cmd->argv, *ep);
	else if (ft_strncmp("pwd", cmd->argv[0], ft_strlen("pwd")) == 0)
		retval = builtin_pwd(ac, cmd->argv, *ep);
	else if (ft_strncmp("export", cmd->argv[0], ft_strlen("export")) == 0)
		retval = builtin_export(ac, cmd->argv, ep);
	else if (ft_strncmp("unset", cmd->argv[0], ft_strlen("unset")) == 0)
		retval = builtin_unset(ac, cmd->argv, ep);
	else if (ft_strncmp("env", cmd->argv[0], ft_strlen("env")) == 0)
		retval = builtin_env(ac, cmd->argv, *ep);
	else if (ft_strncmp("exit", cmd->argv[0], ft_strlen("exit")) == 0)
	 	retval = builtin_exit(ac, cmd->argv, data);
	return (retval);
}