/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macro.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:04:36 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/19 14:49:31 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACRO_H
# define MACRO_H

# define PROMPT			"minishell> "
# define NAME			"minishell"

# define ERR_DEFAULT	"unspecified error"
# define ERR_MALLOC		"malloc error"
# define ERR_SIGNAL		"sigaction error"
# define ERR_UNFQUOTE	"unexpected EOF while looking for matching `''"
# define ERR_UNFDQUOTE	"unexpected EOF while looking for matching `\"'"
# define ERR_SYNTAX		"syntax error"

# define ERR_CDARGS		"too many arguments"
# define ERR_CDHOME		"HOME not set"
# define ERR_PWD		"getcwd error"
# define ERR_EXPIDEN	"not a valid identifier"
# define ERR_EXPEQ		"missing '='"

#endif
