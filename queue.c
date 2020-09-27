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

static inline void swap_if_larger(list_ele_t *a, list_ele_t *b)
{
    if (strcmp(a->value, b->value) > 0) {
        char *t = a->value;
        a->value = b->value;
        b->value = t;
    }
}

static inline bool is_ascending(list_ele_t *a, list_ele_t *b)
{
    return (strcmp(a->value, b->value) <= 0);
}

// static void show_list(list_ele_t *e, int SZ)
// {
//     int cnt = 0;
//     while (e) {
//         cnt++;
//         printf("%s ", e->value);
//         e = e->next;
//     }

//     if (cnt == SZ)
//         printf(": %d\n", SZ);
//     else
//         printf(": %d or %d ?! ********\n", cnt, SZ);
// }

list_ele_t *q_do_sort(list_ele_t *e, const int SZ)
{
    // printf("\n[IN]\n");
    // show_list(e, SZ);

    if (SZ < 2)
        return e;

    /* sort two elements and return */
    if (SZ == 2) {
        swap_if_larger(e, e->next);

        return e;
    }

    /* split */

    list_ele_t *head_a = e, *tail_a = e;
    for (int i = 0; i < SZ / 2; i++) {
        tail_a = tail_a->next;
    }

    list_ele_t *head_b = tail_a->next;
    tail_a->next = NULL;

    // printf("A: "); show_list(head_a, SZ / 2 + 1);
    // printf("B: "); show_list(head_b, SZ - SZ / 2 - 1);

    /* recur */

    head_a = q_do_sort(head_a, SZ / 2 + 1);
    head_b = q_do_sort(head_b, SZ - SZ / 2 - 1);

    /* combine */

    // printf("\nCOMBINE\n");
    // printf("A: "); show_list(head_a, SZ / 2 + 1);
    // printf("B: "); show_list(head_b, SZ - SZ / 2 - 1);

    // printf("M: {");
    list_ele_t *a = head_a, *b = head_b, *m = NULL, *head_m = NULL;
    if (is_ascending(a, b)) {
        m = a;
        a = a->next;
    } else {
        m = b;
        b = b->next;
    }
    head_m = m;
    // printf("%s ", m->value);
    while (a && b) {
        if (is_ascending(a, b)) {
            m->next = a;
            a = a->next;
        } else {
            m->next = b;
            b = b->next;
        }
        m = m->next;
        // printf("%s ", m->value);
    }
    // printf("; ");

    list_ele_t *other = (a ? a : b);
    while (true) {
        m->next = other;
        m = m->next;
        // printf("%s ", m->value);
        if (!(other = other->next))
            break;
    }
    m->next = NULL;
    // printf("}\n");

    return head_m;
}

void q_sort(queue_t *q)
{
    /* if q has only one element, do nothing */
    if (!q || q->size == 1)
        return;

    list_ele_t *new_head = q_do_sort(q->head, q->size);
    q->head = new_head;

    list_ele_t *new_tail = new_head;
    while (new_tail && new_tail->next) {
        new_tail = new_tail->next;
    }
    q->tail = new_tail;

    // printf("CHECK AFTER q_sort:\n");
    // list_ele_t *k = q->head;
    // int cnt = 0;
    // while (true) {
    //     if (!k->next)
    //         break;
    //     printf("%s:%d ", k->value, cnt);
    //     k = k->next;
    //     cnt++;
    // }
    // printf("\n");

    // printf("q_sort done\n");
}
