#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H


typedef struct linknode
{
    struct linknode *pre;
    struct linknode *next;
}TLinkNode;



#define LINK_LIST_HEAD(type) \
struct { \
    struct type *l_first; \
}


#define LINK_LIST_ENTRY(type) \
struct { \
    struct type *pre; \
    struct type *next; \
}


#define LIST_INIT(head) do { \
    (head)->l_first = NULL; \
} while (0)



#define LIST_INSERT_HEAD(head, tmp, field) do { \
    if ((head)->l_first == NULL) {\
        (tmp)->next = (tmp); \
        (tmp)->pre = (tmp); \
    } else { \
        (tmp)->next = (head)->l_first; \
        (tmp)->pre = (head)->l_first->pre; \
        (head)->l_first->pre->next = (tmp); \
        (head)->l_first->pre = (tmp); \
        } \
    (head)->l_first = (tmp); \
}while (0)

#define LIST_REMOVE(head, tmp, field) do {\
    if ((tmp)->field->pre == (tmp)->field->next) { \
        (head)->l_first = NULL; \
    } else { \
        (tmp)->field->pre->next = (tmp)->field->next; \
        (tmp)->field->next->pre = (tmp)->field->pre; \
    } \
}while (0)



extern int quit_flag;
#endif // COMMON_UTILS_H
