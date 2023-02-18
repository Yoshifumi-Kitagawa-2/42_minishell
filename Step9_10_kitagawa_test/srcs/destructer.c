/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 17:33:46 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/17 22:32:28 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/step_9_10.h"

void	free_node(t_node *node)
{
	if (node == NULL)
		return ;
	free_all_token(node->args);
    free_all_token(node->filename);
    free_all_token(node->delimiter);
	free_node(node->redirects);
	free_node(node->next);
    free_node(node->command);
	free(node);
}

void free_all_token(t_token *token)
{
    if (token == NULL)
        return ;
    if (token->word != NULL)
        free(token->word);
    free_all_token(token->next);
    free(token);
}

void free_argv(char **argv)
{
    size_t  i;

    if (argv == NULL)
        return ;
    i = 0;
    while (argv[i] != NULL)
    {
        free(argv[i]);
        i++;
    }
    free(argv);
}