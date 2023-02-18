/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 11:55:39 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/18 18:21:00 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/step_9_10.h"

/*
<program> = <simple_command>
<simple_command> = <simple_command_element>+
<simple_command_element> = <redirect>
                         | <word>
<redirect> = <redirect_out>
           | <redirect_in>
           | <redirect_append>
           | <redirect_heredoc>

<redirect_out> = '>' <word>
<redirect_in> = '<' <word>
<redirect_append> = '>>' <word>
<redirect_heredoc> = '<<' <word>
*/

int stashfd(int fd)
{
    int stashfd;
    
    stashfd = fcntl(fd, F_DUPFD, 10);
    if (stashfd < 0)
        fatal_error("fcntl");
    if (close(fd) < 0)
        fatal_error("close");
    return (stashfd);
}

int		read_heredoc(const char *delimiter)
{
	char	*line;
	int		pfd[2];
	
	if (pipe(pfd) < 0)
		fatal_error("pipe");
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break;
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		dprintf(pfd[1], "%s\n", line);
		free(line);
	}
	close(pfd[1]);
	return (pfd[0]);
}

int     open_redir_file(t_node *node)
{
	if (node == NULL)
		return (0);
	if (node->kind == ND_PIPELINE)
	{
		if (open_redir_file(node->command) < 0)
			return (-1);
		if (open_redir_file(node->next) < 0)
			return (-1);
		return (0);
	}
	else if (node->kind == ND_SIMPLE_CMD)
		return (open_redir_file(node->redirects));
	else if (node->kind == ND_REDIR_OUT)
		node->filefd = open(node->filename->word, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    else if (node->kind == ND_REDIR_IN)
        node->filefd = open(node->filename->word, O_RDONLY);
    else if (node->kind == ND_REDIR_APPEND)
        node->filefd = open(node->filename->word, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (node->kind == ND_REDIR_HEREDOC)
		node->filefd = read_heredoc(node->delimiter->word);
	else
		assert_error("open_redir_file");
    if (node->filefd < 0)
	{
		xperror(node->filename->word);
		return (-1);
	}
	node->filefd = stashfd(node->filefd);
	return (open_redir_file(node->next));
}

bool is_redirect(t_node *node)
{
	if (node->kind == ND_REDIR_OUT)
		return (true);
	else if (node->kind == ND_REDIR_IN)
		return (true);
	else if (node->kind == ND_REDIR_APPEND)
		return (true);
	else if (node->kind == ND_REDIR_HEREDOC)
		return (true);
	return (false);
}

void	do_redirect(t_node *redir)
{
	if (redir == NULL)
		return ;
	if (is_redirect(redir))
	{
		redir->stashed_targetfd = stashfd(redir->targetfd);
		dup2(redir->filefd, redir->targetfd);
	}
    else
        assert_error("do_redirect");
	do_redirect(redir->next);
}

void	reset_redirect(t_node *redir)
{
	if (redir == NULL)
		return ;
	reset_redirect(redir->next);
	if (is_redirect(redir))
	{
		close(redir->filefd);
		close(redir->targetfd);
		dup2(redir->stashed_targetfd, redir->targetfd);
	}
    else
        assert_error("reset_redirect");
}