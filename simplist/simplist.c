#include "simplist.h"

#include <stdio.h>

t_list	*list_create_node(void *content, size_t content_size)
{
	t_list *node;

	node = malloc(sizeof(t_list));
	node->next = NULL;
	node->prev = NULL;
	node->content_size = content_size;
	node->content = content;
	return (node);
}

t_list	*list_head_tail(t_list *node, int head_or_tail)
{
	if (!node)
		return (NULL);
	while (head_or_tail ? node->next : node->prev)
		node = head_or_tail ? node->next : node->prev;
	return (node);
}

void	list_push(t_list **head, t_list *node, int X)
{
	t_list	*appendto;

	if (!head || !node)
		return ;
	if (!*head)
		*head = node;
	else
	{
		appendto = list_head_tail(*head, X);
		if (X)
		{
			appendto->next = node;
			node->prev = appendto;
			node->next = NULL;
		}
		else
		{
			appendto->prev = node;
			node->next = appendto;
			node->prev = NULL;
		}
	}
}

void	list_push_back(t_list **head, t_list *node)
{
	list_push(head, node, 1);
}

void	list_push_front(t_list **head, t_list *node)
{
	list_push(head, node, 0);
}
