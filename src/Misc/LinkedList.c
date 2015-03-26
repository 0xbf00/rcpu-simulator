#include "LinkedList.h"

#include <stdlib.h>

#ifdef __linux__
#include <malloc.h>
#endif

/**
    @abstract
        Prepend an element to a linked list.

    @param list 
        The linked list to which to prepend an element. This list
        can be NULL
    @param elem
        The actual element that should be prepended

    @return
        A new linked list that contains the new element at the first
        position and the old list following this element.
*/
linked_list_t * ll_prepend_element(linked_list_t * list, void * elem)
{
    linked_list_t * node = malloc(sizeof(linked_list_t));
    if (node == NULL)
        return NULL;

    node->element = elem;
    node->next = list;

    return node;
}

/*!
    @abstract
        Append an element to a linked list.
    @discussion
        This is a slow operation, of complexity O(n)

    @param list
        The linked list to which to append the element. This list
        can be NULL
    @param elem
        The actual element that should be appended

    @return 
        The linked list with the specified element appended.
*/
linked_list_t * ll_append_element(linked_list_t * list, void * elem)
{
    if (list == NULL) {
        linked_list_t * node = malloc(sizeof(linked_list_t));
        if (node == NULL)
            return NULL;

        node->element = elem;
        node->next = NULL;

        return node;
    } else {
        list->next = ll_append_element(list->next, elem);
        return list;
    }
}


/*!
    @abstract
        Map a function over every element
    @discussion
        Calls a user-supplied function on every object in the list.
        This can be used to log items, or to modify the elements of a list.
        It cannot be used to change the underlying linked list!

    @param list 
        The linked list to iterate over
    @param f 
        The user-supplied function that is called with every element of the list
    @param backwards 
        Supply a value != 0 to have the output generated backwards.
        This could result in a stack overflow if the list is too long, because tail call optimizations
        are not possible in this scenario.
*/
void ll_map(linked_list_t * list, void (*f)(const void *), int backwards)
{
    if (list == NULL)
        return;

    if (backwards) {
        ll_map(list->next, f, backwards);
        f(list->element);
    } else {
        f(list->element);
        ll_map(list->next, f, backwards);
    }
}

#if __has_extension(blocks)

/**
    Pretty much the same as ll_map, just using blocks instead of function pointers.

    @see ll_map
*/
void ll_map_block(linked_list_t * list, void (^f)(const void *), int backwards)
{
    if (list == NULL)
        return;

    if (backwards) {
        ll_map_block(list->next, f, backwards);
        f(list->element);
    } else {
        f(list->element);
        ll_map_block(list->next, f, backwards);
    }
}

#endif

/*!
    @abstract
        Release the memory for a linked list.

    @param list
        The list to work on.
    @param deep
        Supply a value != 0 iff the element should be free'd aswell.
*/
void ll_free(linked_list_t * list, int deep)
{
    if (list == NULL)
        return;

    ll_free(list->next, deep);
    if (deep)
        free(list->element);
    free(list);
}
