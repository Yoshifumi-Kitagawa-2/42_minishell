/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8_error.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:33:36 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/13 17:01:18 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_count_and_putstr(int fd, char *s)
{
	if (s == NULL)
		s = "(null)";
	write(fd, s, ft_strlen(s));
	return (ft_strlen(s));
}

int	ft_count_and_output(int fd, va_list ap, int fmt)
{
	int	add_count;

	add_count = 0;
	if (fmt == 's')
		add_count = ft_count_and_putstr(fd, (char *)va_arg(ap, char *));
	return (add_count);
}

int	count(int fd, va_list ap, const char *fmt)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	while (fmt[i] != '\0')
	{
		if (fmt[i] != '%')
		{
			write(fd, &fmt[i], 1);
			count++;
		}
		else if (fmt[i] == '%')
		{
			count += ft_count_and_output(fd, ap, fmt[i + 1]);
			i++;
		}
		i++;
	}
	return (count);
}

int	my_dprintf(int fd, const char *fmt, ...)
{
	va_list	ap;
	int		length;

	va_start(ap, fmt);
	length = count(fd, ap, fmt);
	va_end(ap);
	return (length);
}

void	function_error(char *function_name)
{
	ft_putstr_fd("function error: ", STDERR_FILENO);
	ft_putendl_fd(function_name, STDERR_FILENO);
	exit(1);
}

void	tokenize_error(char *message, char **rest, char *line)
{
	ft_putstr_fd("minishell: syntax error near ", STDERR_FILENO);
	ft_putendl_fd(message, STDERR_FILENO);
	while (*line)
		line++;
	*rest = line;
	g_signal.status = 258;
	g_signal.status = TRUE;
}

void	tokenize_error_2(char *message, char *word)
{
	ft_putstr_fd("minishell: syntax error near ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd(" `", STDERR_FILENO);
	ft_putstr_fd(word, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	g_signal.status = 258;
	g_signal.other_code = TRUE;
}

void	assert_error(const char *msg)
{
	my_dprintf(STDERR_FILENO, "Assert Error: %s\n", msg);
	exit(255);
}

void	err_exit(const char *location, const char *msg)
{
	my_dprintf(STDERR_FILENO, "minishell: %s: %s\n", location, msg);
	exit(127);
}

void	command_not_found(const char *location)
{
	my_dprintf(STDERR_FILENO, "minishell: %s: command not found\n", location);
	exit(127);
}

void	file_not_found(const char *filename)
{
	my_dprintf(STDERR_FILENO, "minishell: %s: \
		No such file or directory\n", filename);
}

void	ambiguous_redirect(char *filename)
{
	my_dprintf(STDERR_FILENO, "minishell: %s: ambiguous redirect\n", filename);
}
