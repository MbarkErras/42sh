#include "simplist.h"

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

size_t	list_size(t_list *head)
{
	size_t	size;

	size = 0;
	while (head && ++size)
		head = head->next;
	return (size);
}

/*
 * deletes a node in a list and relink accordinly, if head != NULL it returns head address after deletion.
 */

t_list	*list_delete_node(t_list *head, t_list **node, void(*delete_node)(void))
{
	t_list *prev;
	t_list *next;

	if (!node || !*node)
		return (NULL);
	if (head == *node)
	{
		head = *node->next;
		delete_node(*node->content);
		free(*node);
		*node = NULL;
		return (head);
	}
	if (*node->next)
		*node->next->prev = *node->prev;
	if (*node->prev)
		*node->prev->next = *node->next;
	delete_node(*node->content);
	free(*node);
	*node = NULL;
	return (head);
}

void	list_delete(t_list *head, void(*delete_node)(void *))
{
	while ((head = list_delete_node(head, &head, delete_node)))
		continue;
}

/*
 * delete a portion of a list delimited by the nodes *from and *to inclusive.
 */

t_list	*list_delete_range(t_list *head, t_list **from, t_list **to, void(*delete_node)(void *))
{
	t_list	*sentinel;

	if (!from || !*from || !to)
	sentinel = *to ? NULL : *to->next;
	while (*from->next != sentinel)
		list_delete_node(head, &*from->next, delete_node);
	return (list_delete_node(head, from, delete_node));
}

size_t	list_node_index(t_list *head, *node)
{
	size_t index;

	index = 1;
	if (!head || !node)
		return (0);
	while (head != node)
	{
		index++;
		head = head->next;
	}
	return (index + 1);
}

t_list	list_indexed_node(t_list *head, size_t index)
{
	size_t i;

	i = 1;
	while (head)
	{
		if (i == index)
			return (head);	
		i++;
		head->next;
	}
	return (0);
}
