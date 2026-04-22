/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 13:00:06 by bdemouge          #+#    #+#             */
/*   Updated: 2026/04/22 12:48:17 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_alloc_error(t_minishell *data, char **dir_lst, char *path)
{
	ft_free((void **)&path);
	free_dir_lst(dir_lst);
	error_out(data, ERR_MALLOC);
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

static char	*search_in_dirs(t_minishell *data, char **dir_lst, char *cmd)
{
	char	*path;
	char	*tmp_path;
	int		i;

	path = NULL;
	i = -1;
	while (dir_lst[++i])
	{
		tmp_path = make_path(dir_lst[i], cmd);
		if (!tmp_path)
			handle_alloc_error(data, dir_lst, path);
		if (access(tmp_path, F_OK) == 0)
		{
			path = tmp_path;
			tmp_path = NULL;
			if (access(path, X_OK) == 0)
				break ;
		}
		else
			ft_free((void **)&tmp_path);
	}
	return (path);
}

char	*get_path(t_minishell *data, t_command *cmd)
{
	char	*path;
	char	**dir_lst;

	if (ft_strchr(cmd->argv[0], '/'))
		return (ft_strdup(cmd->argv[0]));
	dir_lst = get_dir_lst(data->env);
	if (!dir_lst)
		return (NULL);
	path = search_in_dirs(data, dir_lst, cmd->argv[0]);
	free_dir_lst(dir_lst);
	return (path);
}
