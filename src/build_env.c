/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:58:12 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/20 19:38:05 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_env(t_minishell *d, char **env, char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
			error_out(d, ERR_MALLOC);
		i++;
	}
	env[i] = NULL;
}

char	**build_env(t_minishell *d, char **envp)
{
	char	**env;
	size_t	len;

	len = 0;
	while (envp[len])
		len++;
	env = malloc(sizeof(*env) * (len + 1));
	if (!env)
		error_out(d, ERR_MALLOC);
	fill_env(d, env, envp);
	return (env);
}
