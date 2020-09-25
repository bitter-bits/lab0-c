#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/******** Utility Zone ********/

static inline int smaller(int a, int b)
{
    return a < b ? a : b;
}

static inline void increase_size(queue_t *q)
{
    q->size += 1;
}

static inline void decrease_size(queue_t *q)
{
    q->size -= 1;
}

static bool copy_str_and_attach(list_ele_t *e, char *s)
{
    if (!e)
        return false;

    const int SZ = strlen(s);

    char *p = malloc(SZ + 1);
    if (!p)
        return false;

    // strncpy(p, s, strlen(s));
    for (int i = 0; i < SZ; i++) {
        p[i] = s[i];
    }
    p[SZ] = 0;

    e->value = p;

    return true;
}

static list_ele_t *create_element()
{
    list_ele_t *new_e = malloc(sizeof(list_ele_t));
    if (!new_e)
        return NULL;

    new_e->next = NULL;
    new_e->value = NULL;

    return new_e;
}

/******** End of Utility Zone ********/

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    /* Return NULL if could not allocate space */
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
        if (e->value)
            free(e->value);

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
    if (!q)
        return false;

    list_ele_t *e = create_element();
    if (!e)
        return false;

    if (!copy_str_and_attach(e, s)) {
        free(e);

        return false;
    }

    if (q->head) {
        e->next = q->head;
    }

    increase_size(q);
    q->head = e;
    if (q->size == 1)
        q->tail = e;

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
    if (!q)
        return false;

    list_ele_t *e = create_element();
    if (!e)
        return false;

    if (!copy_str_and_attach(e, s)) {
        free(e);

        return false;
    }

    if (q->size > 0)
        q->tail->next = e;

    q->tail = e;
    increase_size(q);
    if (q->size == 1)
        q->head = e;

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
    if (!q || q->size == 0 || !q->head)
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

    decrease_size(q);

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

void swap_head_and_tail(queue_t *q, list_ele_t *head, list_ele_t *tail)
{
    list_ele_t *tmp = tail;
    q->tail = head;
    q->head = tmp;

    q->tail->next = NULL;
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
    if (!q || q->size < 2)
        return;

    list_ele_t *copy_head = q->head, *copy_tail = q->tail;
    list_ele_t *a = q->head, *b = q->head->next;

    a->next = NULL;

    while ((a && b)) {
        list_ele_t *c = b->next;
        /* Revert b to a */
        b->next = a;

        /* Shift a and b */
        a = b;
        b = c;
    }

    /* Swap head and tail */
    swap_head_and_tail(q, copy_head, copy_tail);
}

void q_do_sort(list_ele_t *e);

void q_sort(queue_t *q)
{
    /* if q has only one element, do nothing */
    if (!q || q->size == 1)
        return;

    q_do_sort(q->head);
}

static inline void swap_if_larger(list_ele_t *a, list_ele_t *b)
{
    if (strcmp(a->value, b->value) > 0) {
        char *t = a->value;
        a->value = b->value;
        b->value = t;
    }
}

void q_do_sort(list_ele_t *e)
{
    /* if e is the only one element, do nothing */
    if (!e || !e->next)
        return;

    list_ele_t *head_a, *tail_a, *head_b, *tail_b;
    head_a = tail_a = e;
    head_b = tail_b = e->next;

    char *pivot = tail_b->value;

    /* if a > b, swap values */
    swap_if_larger(tail_a, tail_b);

    if (!e->next->next)
        return;

    list_ele_t *rest = e->next;
    while ((rest = rest->next)) {
        if (strcmp(rest->value, pivot) > 0) {
            tail_b->next = rest;
            tail_b = rest;
        } else {
            tail_a->next = rest;
            tail_a = rest;
        }
    }

    /* split */
    tail_a->next = NULL;
    q_do_sort(head_a);

    tail_b->next = NULL;
    q_do_sort(head_b);

    /* chain */
    tail_a->next = head_b;
}
