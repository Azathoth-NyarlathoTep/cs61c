#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* your code here */
    if(head == NULL) return 0;
    node *tortoise = head->next;
    node *hare = head->next;
    if(hare == NULL) return 0;
    hare = hare->next;

    while(hare != NULL && hare != tortoise)
    {
        tortoise = tortoise->next;
        hare = hare->next;
        if(hare == NULL) break;
        hare = hare->next;
    }
    
    if(hare == tortoise) return 1;

    return 0;
}