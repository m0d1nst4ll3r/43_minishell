/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:33:04 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/22 14:01:05 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(char *name, char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '=')
	{
		print_error_builtin_file(name, str, ERR_EXPIDEN);
		return (1);
	}
	while (str[i] && str[i] != '=')
		i++;
	if (!str[i])
	{
		print_error_builtin_file(name, str, ERR_EXPEQ);
		return (1);
	}
	i = -1;
	while (str[++i] != '=')
	{
		if ((!i && ft_isdigit(str[i])) || !is_envar_char(str[i]))
		{
			print_error_builtin_file(name, str, ERR_EXPIDEN);
			return (1);
		}
	}
	return (0);
}

static char	**get_to_replace(char *envar, char **env)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (envar[j] != '=' && envar[j] == env[i][j])
			j++;
		if (envar[j] == env[i][j])
			return (env + i);
		i++;
	}
	return (NULL);
}

static int	add_new_envar(char *new_envar, char ***old_env, t_minishell *d)
{
	char	**new_env;
	size_t	i;

	new_env = malloc(sizeof(*new_env) * (get_env_size(*old_env) + 2));
	if (!new_env)
		error_out(d, ERR_MALLOC);
	i = 0;
	while (old_env[0][i])
	{
		new_env[i] = old_env[0][i];
		i++;
	}
	new_env[i++] = new_envar;
	new_env[i] = NULL;
	free(*old_env);
	*old_env = new_env;
	return (0);
}

static int	export_envar(char *envar, char ***ep, t_minishell *d)
{
	char	**to_replace;
	char	*new_envar;

	new_envar = ft_strdup(envar);
	if (!new_envar)
		error_out(d, ERR_MALLOC);
	to_replace = get_to_replace(envar, *ep);
	if (!to_replace)
		return (add_new_envar(new_envar, ep, d));
	free(*to_replace);
	*to_replace = new_envar;
	return (0);
}

int	builtin_export(int ac, char **av, char ***ep, t_minishell *d)
{
	size_t	i;
	int		ret;

	if (ac < 2 || !*ep)
		return (0);
	i = 1;
	ret = 0;
	while (av[i])
	{
		if (is_valid_identifier(av[0], av[i]))
			ret = 1;
		else if (export_envar(av[i], ep, d))
		{
			print_error_builtin(av[0], ERR_MALLOC);
			return (1);
		}
		i++;
	}
	return (ret);
}
