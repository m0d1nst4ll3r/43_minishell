/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:59:04 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/01 14:55:36 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

extern int	g_signal;

// ================================== SYSTEM ==================================
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <limits.h>
# include <sys/types.h> //
# include <sys/stat.h> //
// =================================== USER ===================================
# include "libft.h"
# include "structure.h"
# include "prototype.h"
# include "macro.h"

#endif
