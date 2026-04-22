/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 12:26:48 by bdemouge          #+#    #+#             */
/*   Updated: 2026/04/22 12:44:02 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_access(t_minishell *data, t_command *cmd, char *path)
{
	struct stat	s;

	if (!path)
	{
		ft_fprintf(2, "%s: %s: %s\n", NAME, cmd->argv[0], ERR_NOCMD);
		exit_prog(data, 127);
	}
	if (stat(path, &s) != 0)
	{
		print_error(cmd->argv[0]);
		free(path);
		exit_prog(data, 127);
	}
	if (S_ISDIR(s.st_mode))
	{
		print_error_builtin(cmd->argv[0], ERR_ISDIR);
		free(path);
		exit_prog(data, 126);
	}
	if (access(path, X_OK) != 0)
	{
		print_error(cmd->argv[0]);
		free(path);
		exit_prog(data, 126);
	}
}

void	free_dir_lst(char **dir_lst)
{
	int	i;

	i = 0;
	while (dir_lst[i])
	{
		free(dir_lst[i]);
		i++;
	}
	free(dir_lst);
}
