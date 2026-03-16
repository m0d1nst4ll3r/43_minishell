/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:03:53 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/16 22:40:08 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPE_H
# define PROTOTYPE_H

//
//	Shell setup
//
char			**build_env(char **envp);
int				setup_signal_handlers(void);

//
//	Main loop
//
t_command		*parse(t_minishell *data);
int				execute(t_command *cmd_list);
void			cleanup_line(char *line, t_command *cmd_list);
void			cleanup_env(char **env);

//
//	Parser
//
// Tokenizer
t_token			*tokenize(t_minishell *d);
int				get_word_len(t_minishell *d, size_t i);
void			fill_word(t_minishell *d, size_t *i, char *word);
// Tokenizer Util
t_token_type	get_token_type(char *line);
int				is_end_of_word(char c, t_parse_state state);
void			update_state(char c, t_parse_state *state, size_t *i);
void			cleanup_token_list(t_token *token_list);
// Organizer

//
//	Executor
//

//
//	Util
//
void			*ft_malloc(size_t size);
int				is_envar_char(char c);
void			print_error(char *err_str);
size_t			get_uchar_len(unsigned char u);

#endif
