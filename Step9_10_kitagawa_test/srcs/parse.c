/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 17:46:55 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/18 01:38:18 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/step_9_10.h"

t_node *parse(t_token *token)
{
    return (pipeline(&token, token));
}

t_node *pipeline(t_token **rest, t_token *token)
{
    t_node *node;

    node = new_node(ND_PIPELINE);
    node->inpipe[0] = STDIN_FILENO;
    node->inpipe[1] = -1;
    node->outpipe[0] = -1;
    node->outpipe[1] = STDOUT_FILENO;
    node->command = simple_command(&token, token);
    if (equal_op(token, "|"))
        node->next = pipeline(&token, token->next);
    *rest = token;
    return (node);
}

bool is_control_operator(t_token *token)
{
    static char *const operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
    size_t  i = 0;

    while (i < sizeof(operators) / sizeof(*operators))
    {
        if (startswith(token->word, operators[i]))
            return (true);
        i++;
    }
    return (false);
}
/*
bool is_redirect_operator(t_token *token)
{
    static char *const operators[] = {">", "<", ">>", "<<"};
    size_t  i = 0;

    while (i < sizeof(operators) / sizeof(*operators))
    {
        if (startswith(token->word, operators[i]))
            return (true);
        i++;
    }
    return (false);
}
*/

t_node	*simple_command(t_token **rest, t_token *token)
{
    t_node *node;

    node = new_node(ND_SIMPLE_CMD);
    append_command_element(node, &token, token);
    while (token != NULL && at_eof(token) == false && is_control_operator(token) == false)
        append_command_element(node, &token, token);
    *rest = token;
    return(node);
}

t_node	*redirect_out(t_token **rest, t_token *token)
{
	t_node	*node;

	node = new_node(ND_REDIR_OUT);
	node->filename = tokendup(token->next);
	node->targetfd = STDOUT_FILENO;
	*rest = token->next->next;
	return (node);
}

t_node	*redirect_in(t_token **rest, t_token *token)
{
	t_node	*node;

	node = new_node(ND_REDIR_IN);
	node->filename = tokendup(token->next);
	node->targetfd = STDIN_FILENO;
	*rest = token->next->next;
	return (node);
}

t_node	*redirect_append(t_token **rest, t_token *token)
{
	t_node	*node;
    
	node = new_node(ND_REDIR_APPEND);
	node->filename = tokendup(token->next);
	node->targetfd = STDOUT_FILENO;
	*rest = token->next->next;
	return (node);
}

t_node *redirect_heredoc(t_token **rest, t_token *token)
{
    t_node *node;

    node = new_node(ND_REDIR_HEREDOC);
    node->delimiter = tokendup(token->next);
    node->targetfd = STDIN_FILENO;
    *rest = token->next->next;
    return (node);
}

void append_command_element(t_node *command, t_token **rest, t_token *token)
{
    if (token->kind == TK_WORD)
	{
        append_token(&command->args, tokendup(token));
		token = token->next;
    }
    else if (equal_op(token, ">") && token->next->kind == TK_WORD)
        append_node(&command->redirects, redirect_out(&token, token));
    else if (equal_op(token, "<") && token->next->kind == TK_WORD)
        append_node(&command->redirects, redirect_in(&token, token));
    else if (equal_op(token, ">>") && token->next->kind == TK_WORD)
        append_node(&command->redirects, redirect_append(&token, token));
    else if (equal_op(token, "<<") && token->next->kind == TK_WORD)
        append_node(&command->redirects, redirect_heredoc(&token, token));
    else
		todo("append_command_element");
    *rest = token;
}

bool	at_eof(t_token *token)
{
	return (token->kind == TK_EOF);
}

bool	equal_op(t_token *tok, char *op)
{
	if (tok->kind != TK_OP)
		return (false);
	return (strcmp(tok->word, op) == 0);
}

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = calloc(1, sizeof(*node));
	if (node == NULL)
		fatal_error("calloc");
	node->kind = kind;
	return (node);
}

t_token *tokendup(t_token *token)
{
    char *word;
    
    word = strdup(token->word);
    if (word == NULL)
        fatal_error("strdup");
    return (new_token(word, token->kind));
}

void append_token(t_token **tokens, t_token *token)
{
    if (*tokens == NULL)
    {
        *tokens = token;
        return ;
    }
    append_token(&(*tokens)->next, token);
}

void append_node(t_node **node, t_node *elm)
{
    if (*node == NULL)
    {
        *node = elm;
        return ;
    }
    append_node(&(*node)->next, elm);
}

