/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 13:00:06 by bdemouge          #+#    #+#             */
/*   Updated: 2026/04/20 18:55:48 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_dir_lst(char **dir_lst)
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

static char	*make_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*path;

	if (!dir || !cmd)
		return (NULL);
	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!path)
		return (NULL);
	return (path);
}

static char	**get_dir_lst(char **envp)
{
	int	i;

	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	return (ft_split(envp[i] + 5, ':'));
}

char	*get_path(t_minishell *data, t_command *cmd)
{
	char	*path;
	char	**dir_lst;
	int		i;

	if (ft_strchr(cmd->argv[0], '/'))
		return (ft_strdup(cmd->argv[0]));
	dir_lst = get_dir_lst(data->env);
	if (!dir_lst)
		return (NULL);
	i = 0;
	while (dir_lst[i])
	{
		path = make_path(dir_lst[i], cmd->argv[0]);
		if (!path)
		{
			free_dir_lst(dir_lst);
			exit_prog(data, 1);
		}
		if (access(path, F_OK) == 0)
			return (free_dir_lst(dir_lst), path);
		free(path);
		i++;
	}
	free_dir_lst(dir_lst);
	return (NULL);
}

void	check_access(t_minishell *data, t_command *cmd, char *path)
{
	struct stat	s;

	if (!path)
	{
		print_error_builtin(cmd->argv[0], ERR_NOCMD);
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
