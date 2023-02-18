/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 17:33:58 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/18 01:26:04 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/step_9_10.h"

t_token *new_token(char *word, t_token_kind kind)
{
    t_token *token;

    token = calloc (1, sizeof(*token));
    if (token == NULL)
        fatal_error("calloc");
    token->word = word;
    token->kind = kind;
    return (token);
}

bool    is_blank(char c)
{
    if (c == ' ' || c == '\t' || c == '\n')
        return (true);
    else
        return (false);
}

bool    consume_blank(char **rest, char *line)
{
    if (is_blank(*line) == true)
    {
        while (*line != '\0' && is_blank(*line) == true)
            line++;
        *rest = line;
        return (true);
    }
    *rest = line;
    return (false);
}

/*
この関数は、文字列 s が文字列 keyword で始まっているかどうかを判定するものです。
まず、 strlen(keyword) 関数を使用して、文字列 keyword の長さを取得します。
次に、 memcmp 関数を使用して、文字列 s の先頭 strlen(keyword) 文字と文字列 keyword の内容を比較します。
もし、この比較結果が 0 であれば、文字列 s は文字列 keyword で始まっていると判定されます。そのため、この関数は true を返します。
一方、もし、この比較結果が 0 以外であれば、文字列 s は文字列 keyword で始まっていないと判定されます。そのため、この関数は false を返します。
*/
bool startswith(const char *s, const char *keyword)
{
    if (memcmp(s, keyword, strlen(keyword)) == 0)
        return (true);
    else
        return (false);
}

/*
metacharacter
	  A character that, when unquoted, separates words.  One of the following:
	  |  & ; ( ) < > space tab
control operator
	  A token that performs a control function.  It is one of the following symbols:
	  || & && ; ;; ( ) | <newline>
*/

/*
DEFINITIONS
       The following definitions are used throughout the rest of this document.
       blank  A space or tab.
       word   A sequence of characters considered as a single unit by the shell.  Also known as a token.
       name   A word consisting only of alphanumeric characters and underscores, and beginning with an alphabetic
              character or an underscore.  Also referred to as an identifier.
       metacharacter
              A character that, when unquoted, separates words.  One of the following:
              |  & ; ( ) < > space tab
       control operator
              A token that performs a control function.  It is one of the following symbols:
              || & && ; ;; ( ) | <newline>
*/

/*
この関数は、文字 c がメタ文字であるかどうかを判定するものです。
まず、 c が 0 以外の文字であるかどうかを判定します。
次に、 strchr 関数を使用して、文字 c が "|&;()<> \t\n" のいずれかの文字であるかどうかを判定します。
もし、 c が 0 以外であり、かつ strchr 関数が返す値が非 NULL であれば、文字 c はメタ文字と判定されます。そのため、この関数は true を返します。
一方、もし、 c が 0 以外でありながら、かつ strchr 関数が返す値が NULL であれば、文字 c はメタ文字でないと判定されます。そのため、この関数は false を返します。
*/
bool is_metacharacter(char c)
{
    if (c != 0 && strchr("|&;()<> \t\n", c) != NULL)
        return (true);
    else
        return (false);
}

/*
この関数は、文字列 s が単語であるかどうかを判定するものです。
まず、文字列 s の先頭文字が 0 以外であるかどうかを判定します。
次に、先頭文字 *s がメタ文字であるかどうかを判定するために、 is_metacharacter 関数を呼び出します。
もし、文字列 s の先頭文字が 0 以外であり、かつ is_metacharacter 関数が false を返した場合、文字列 s は単語と判定されます。そのため、この関数は true を返します。
一方、もし、文字列 s の先頭文字が 0 であったり、is_metacharacter 関数が true を返した場合、文字列 s は単語でないと判定されます。そのため、この関数は false を返します。
*/
bool is_word(const char *s)
{
    if (*s != '\0' && is_metacharacter(*s) == false)
        return (true);
    else
        return (false);
}

/*
この関数は、文字列 line の中から演算子（オペレータ）を検索し、そのオペレータを返すものです。
まず、配列 operators には、オペレータとして認識する文字列が定義されています。
次に、配列 operators の各要素に対して、文字列 line がその要素で始まっているかどうかを確認するために、 startswith 関数を呼び出します。
もし、文字列 line が配列 operators のいずれかの要素で始まっていた場合、オペレータ文字列は strdup 関数を使って複製されます。
これにより、文字列 line の先頭からオペレータ文字列までの部分が切り出されます。
最後に、文字列 line のポインタ（アドレス）を更新し、 new_token 関数を呼び出して、オペレータ文字列を含むトークンを返します。
一方、配列 operators の全ての要素に対して、文字列 line がその要素で始まっていない場合、予期しないオペレータであると判定され、 assert_error 関数が呼び出されます。
*/
t_token *operator(char **rest, char *line)
{
    static char *const operators[] = {">>", "<<", ">", "<", "||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
    size_t  i = 0;
    char    *op;

    while (i < sizeof(operators) / sizeof(*operators))
    {
        if(startswith(line, operators[i]) == true)
        {
            op = strdup(operators[i]);
            if (op == NULL)
                fatal_error("strdup");
            *rest = line + strlen(op);
            return (new_token(op, TK_OP));
        }
        i++;
    }
    assert_error("Unexpected operator");
}

t_token *word(char **rest, char *line)
{
    const char  *start = line;
    char        *word;

    while (*line != '\0' && is_metacharacter(*line) == false)
    {
		if (*line == SINGLE_QUOTE)
		{
			// skip quote
			line++;
			while (*line && *line != SINGLE_QUOTE)
				line++;
			if (*line == '\0')
			{
				tokenize_error("Unclosed single quote", &line, line);
				break ;
			}
			// skip quote
            else
			    line++;
		}
        else if (*line == DOUBLE_QUOTE)
		{
			// skip quote
			line++;
			while (*line && *line != DOUBLE_QUOTE)
				line++;
			if (*line == '\0')
			{
				tokenize_error("Unclosed double quote", &line, line);
				break ;
			}
			// skip quote
            else
			    line++;
		}
		else
			line++;
	}
    word = strndup(start, line - start);
    if (word == NULL)
        fatal_error("strndup");
    *rest = line;
    return (new_token(word, TK_WORD));
}

t_token *tokenize(char *line)
{
    t_token head;
    t_token *token;

    syntax_error = false;
    head.next = NULL;
    token = &head;
    while(*line != '\0')
    {
        if (consume_blank(&line, line) == true)
            continue;
        else if (is_metacharacter(*line) == true)
            token = token->next = operator(&line, line);
        else if (is_word(line) == true)
            token = token->next = word(&line, line);
        else
            tokenize_error("Unexpected Token", &line, line);
    }
    token->next = new_token(NULL, TK_EOF);
    return (head.next);
}

char	**tail_recursive(t_token *token, int nargs, char **argv)
{
	if (token == NULL || token->kind == TK_EOF)
		return (argv);
	argv = reallocf(argv, (nargs + 2) * sizeof(char *));
	argv[nargs] = strdup(token->word);
	if (argv[nargs] == NULL)
		fatal_error("strdup");
	argv[nargs + 1] = NULL;
	return (tail_recursive(token->next, nargs + 1, argv));
}

char	**token_list_to_argv(t_token *token)
{
	char	**argv;

	argv = calloc(1, sizeof(char *));
	if (argv == NULL)
		fatal_error("calloc");
	return (tail_recursive(token, 0, argv));
}
