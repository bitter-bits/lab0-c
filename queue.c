#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

static int smaller(int a, int b)
{
    return a < b ? a : b;
}

static bool copy_str_and_attach(list_ele_t *e, char *s)
{
    if (!e)
        return false;

    char *p = malloc(strlen(s) + 1);
    if (!p)
        return false;

    strncpy(p, s, strlen(s));
    p[strlen(s)] = 0;

    e->value = p;

    return true;
}

static list_ele_t *create_element()
{
    list_ele_t *new_e = NULL;
    new_e = malloc(sizeof(list_ele_t));

    if (!new_e)
        return NULL;

    new_e->next = NULL;
    new_e->value = NULL;

    return new_e;
}

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;

    list_ele_t *e = q->head;
    while (e) {
        /* Delete string */
        if (e->value)
            free(e->value);

        /* Delete element */
        list_ele_t *old = e;
        e = e->next;
        free(old);
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh = create_element();
    if (!newh)
        return false;

    if (!copy_str_and_attach(newh, s))
        return false;

    newh->next = q->head;
    q->head = newh;
    q->size += 1;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *current = NULL;

    /* Find the last element */
    if (!q->tail) {
        current = q->head;
        while (current->next) {
            current = current->next;
        }
    } else {
        current = q->tail;
    }

    /* Create new element */
    list_ele_t *new_e = create_element();
    if (!new_e)
        return false;

    if (!copy_str_and_attach(new_e, s))
        return false;

    current->next = new_e;

    /* Backup the last element */
    q->tail = new_e;
    q->size += 1;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || q->size == 0)
        return false;

    list_ele_t *head = q->head;
    if (sp) {
        int str_sz = smaller(bufsize - 1, strlen(head->value));
        strncpy(sp, head->value, str_sz);
        sp[str_sz] = 0;
    }

    free(head->value);

    q->head = head->next;
    free(head);

    q->size -= 1;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return (q && q->size > 0 ? q->size : 0);
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
}
