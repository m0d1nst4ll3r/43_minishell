/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 11:38:07 by bdemouge          #+#    #+#             */
/*   Updated: 2026/04/23 16:16:24 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_overflow(long result, int digit, int neg)
{
	if (neg == 1)
	{
		if (result > (LONG_MAX / 10))
			return (1);
		result *= 10;
		if (result > LONG_MAX - digit)
			return (1);
	}
	else if (neg == -1)
	{
		if (result < (LONG_MIN / 10))
			return (1);
		result *= 10;
		if (result < LONG_MIN + digit)
			return (1);
	}
	return (0);
}

static long	atol_safe(char *str)
{
	int		i;
	int		n;
	long	result;

	i = 0;
	n = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i++] == '-')
			n = -1;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]) || check_overflow(result, str[i] - 48, n))
		{
			print_error_exit(str, "numeric argument required");
			return (2);
		}
		result = result * 10 + ((str[i] - 48) * n);
		i++;
	}
	return (result);
}

void	exit_prog(t_minishell *data, int status)
{
	clear_pipes(&(data->exec.pipe_fd));
	free(data->line);
	cleanup_cmd_list(data->cmd_list, 1);
	cleanup_env(data->env);
	rl_clear_history();
	safe_close(&data->stdin);
	safe_close(&data->stdout);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit(status);
}

int	builtin_exit(int ac, char **argv, t_minishell *data)
{
	unsigned char	status;

	status = data->last_return ;
	if (ac > 1)
	{
		if (!is_numeric(argv[1]))
		{
			if (!data->forked)
				printf("exit\n");
			print_error_exit(argv[1], "numeric argument required");
			exit_prog(data, 2);
		}
		else if (ac > 2)
		{
			print_error_exit(NULL, "too many arguments");
			return (1);
		}
	}
	if (!data->forked)
		printf("exit\n");
	if (ac == 2)
		status = atol_safe(argv[1]);
	exit_prog(data, status);
	return (1);
}
