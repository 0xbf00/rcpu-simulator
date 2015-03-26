/*!
    @header Linked List
    Data definitions for a linked list, as well as function
    that can be used to interact with said type.

    @language c
    @author Jakob Rieck
*/
#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

// Workaround to use __has_extension regardless of the compiler
// taken from http://stackoverflow.com/questions/7419167/determine-llvm-versus-gcc-at-compile-time
#ifndef __has_feature         // Optional of course.
  #define __has_feature(x) 0  // Compatibility with non-clang compilers.
#endif
#ifndef __has_extension
  #define __has_extension __has_feature // Compatibility with pre-3.0 compilers.
#endif

// linked list
struct linked_list_s {
    void * element;
    struct linked_list_s * next;
};

/*!
    @abstract
        Type of a node of a linked list 
*/
typedef struct linked_list_s linked_list_t;

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
linked_list_t * ll_prepend_element(linked_list_t * list, void * elem);


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
linked_list_t * ll_append_element(linked_list_t * list, void * elem);

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
void ll_map(linked_list_t * list, void (*f)(const void *), int backwards);


#if __has_extension(blocks)

/*!
    @abstract
        Log the linked to list to stdout, using a user-supplied block.

    @see ll_map
*/
void ll_map_block(linked_list_t * list, void (^f)(const void *), int backwards);

#endif

/*!
    @abstract
        Release the memory for a linked list.

    @param list
        The list to work on.
    @param deep
        Supply a value != 0 iff the element should be free'd aswell.
*/
void ll_free(linked_list_t * list, int deep);

#endif // _LINKED_LIST_H
