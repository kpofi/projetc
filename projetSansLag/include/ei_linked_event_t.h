#ifndef EI_LINKED_EVENT_T_H
#define EI_LINKED_EVENT_T_H

#include "ei_event.h"

typedef struct ei_linked_event_t {
    ei_eventtype_t event_type;
    ei_callback_t callback; // pour réagir à un evenement
    void* user_param;
    struct ei_linked_event_t* next;
} ei_linked_event_t;


void delete_linked_list_event(ei_linked_event_t* e);

#endif
