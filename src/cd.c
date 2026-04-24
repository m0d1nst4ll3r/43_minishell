/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 17:44:40 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/24 16:04:55 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_home(char *name, char **ep)
{
	char	*home;

	home = get_env("HOME", ep);
	if (!home)
	{
		print_error_builtin(name, ERR_CDHOME);
		return (1);
	}
	if (chdir(home))
	{
		print_error_builtin(name, home);
		return (1);
	}
	return (0);
}

static int	only_update_pwd(char *pwd, char ***env, t_minishell *d)
{
	char	**export_argv;
	char	**unset_argv;

	export_argv = malloc(sizeof(*export_argv) * 3);
	unset_argv = malloc(sizeof(*unset_argv) * 3);
	if (!pwd || !export_argv || !unset_argv)
	{
		free(pwd);
		free(export_argv);
		free(unset_argv);
		return (1);
	}
	export_argv[0] = "export";
	export_argv[1] = pwd;
	export_argv[2] = NULL;
	unset_argv[0] = "unset";
	unset_argv[1] = "OLDPWD";
	unset_argv[2] = NULL;
	builtin_export(2, export_argv, env, d);
	builtin_unset(2, unset_argv, env, d);
	free(unset_argv);
	free(export_argv);
	free(pwd);
	return (0);
}

static int	update_pwd_and_oldpwd(char *pwd, char *oldpwd, char ***env,
		t_minishell *d)
{
	char	**export_argv;

	export_argv = malloc(sizeof(*export_argv) * 4);
	if (!pwd || !oldpwd || !export_argv)
	{
		free(pwd);
		free(oldpwd);
		free(export_argv);
		return (1);
	}
	export_argv[0] = "export";
	export_argv[1] = pwd;
	export_argv[2] = oldpwd;
	export_argv[3] = NULL;
	builtin_export(3, export_argv, env, d);
	free(export_argv);
	free(pwd);
	free(oldpwd);
	return (0);
}

static int	update_pwd(char ***env, t_minishell *d)
{
	char	*oldpwd;
	char	*pwd;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	oldpwd = get_env("PWD", *env);
	pwd = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (oldpwd && update_pwd_and_oldpwd(pwd,
			ft_strjoin("OLDPWD=", oldpwd), env, d))
		return (1);
	else if (!oldpwd && only_update_pwd(pwd, env, d))
		return (1);
	return (0);
}

// cd built-in
// needs environment to search for HOME when used without args
int	builtin_cd(int ac, char **av, char ***ep, t_minishell *d)
{
	if (ac > 2)
	{
		print_error_builtin(av[0], ERR_CDARGS);
		return (1);
	}
	else if (ac == 1 && cd_home(av[0], *ep))
		return (1);
	else if (ac > 1 && chdir(av[1]))
	{
		print_error_builtin(av[0], av[1]);
		return (1);
	}
	if (update_pwd(ep, d))
		return (1);
	return (0);
}
