/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:14:09 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/02/21 20:08:47 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/step_1_4.h"

t_environ *new_list(char *envp)
{
    t_environ *new;
    char **tmp;

    new = malloc(sizeof(t_environ));
    if (!new)
        fatal_error("malloc");
    tmp = ft_split(envp, '=');
    new->key = tmp[0];
    new->value = tmp[1];
    new->next = NULL;
    free(tmp);
    return (new);
}

void    list_add_back(t_environ **list, t_environ *new)
{
    t_environ *head;
    
    head = *list;
    while (head->next != NULL)
        head = head->next;
    head->next = new;
}

t_environ *make_environ(char **envp)
{
    t_environ *list;

    t_environ *new;
    new = new_list(*envp++);
    list = new;
    // printf("%s, %d\n", __FILE__, __LINE__);
    // list_add_back(&list, new);
    // printf("%s\n", list->key);
    // printf("%s\n", list->value);
    // printf("%s\n", list->next->key);
    // printf("%s\n", list->next->value);
    // exit(1);

    while (*envp)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        list_add_back(&list, new_list(*envp));
        // printf("%s, %d\n", __FILE__, __LINE__);
        envp++;
        // break ;
    }
    return (list);
}