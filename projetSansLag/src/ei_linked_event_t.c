#include "ei_linked_event_t.h"
#include <stdlib.h>

void delete_linked_list_event(ei_linked_event_t* e){

    if(e==NULL)
        return;

    delete_linked_list_event(e->next);
    free(e);

}
