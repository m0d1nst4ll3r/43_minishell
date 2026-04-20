/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 14:20:06 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/20 20:18:55 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	delete_from_env(char *to_delete, char **old_env, char **new_env)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (old_env[i])
	{
		if (old_env[i] != to_delete)
		{
			new_env[j] = old_env[i];
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
}

static char	*get_to_delete(char *envar, char **env)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (envar[j] == env[i][j])
			j++;
		if (!envar[j] && env[i][j] == '=')
			return (env[i]);
		i++;
	}
	return (NULL);
}

static int	unset_envar(char *to_unset, char ***ep, t_minishell *d)
{
	char	**new_env;
	char	*to_delete;

	to_delete = get_to_delete(to_unset, *ep);
	if (!to_delete)
		return (0);
	new_env = malloc(sizeof(*new_env) * get_env_size(*ep));
	if (!new_env)
		error_out(d, ERR_MALLOC);
	delete_from_env(to_delete, *ep, new_env);
	free(to_delete);
	free(*ep);
	*ep = new_env;
	return (0);
}

int	builtin_unset(int ac, char **av, char ***ep, t_minishell *d)
{
	while (--ac)
	{
		av++;
		if (unset_envar(*av, ep, d))
			return (1);
	}
	return (0);
}
