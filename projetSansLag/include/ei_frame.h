#ifndef EI_FRAME_H
#define EI_FRAME_H

#include "ei_widget.h"
#include "ei_linked_event_t.h"


typedef struct ei_frame_t {
    ei_widget_t  widget;
    ei_color_t  color; 
    int exemple; // Une valeur d'exemple de type int
    int border_width;
    ei_relief_t  relief;
    char*   text;
    ei_font_t  text_font;
    ei_color_t  text_color;
    ei_anchor_t  text_anchor;
    ei_surface_t*  img;
    ei_rect_t*  img_rect;
    ei_anchor_t  img_anchor;
    
    ei_linked_event_t* event; // pour réagir à un evenement 
    ei_linked_event_t* queue; // Pointeur du dernier event pour inserer en queue facilement
} ei_frame_t;


void frame_drawfunc (struct ei_widget_t*    widget,
        ei_surface_t           surface,
        ei_surface_t           pick_surface,
        ei_rect_t*             clipper);

void* frame_allocfunc();

void frame_releasefunc(struct ei_widget_t *widget);

void frame_setdefaultsfunc(struct ei_widget_t *widget);

void ajouter_evenement_frame(ei_frame_t* f,ei_callback_t c,ei_eventtype_t e, void* user_param);

void supprimer_evenement_frame(ei_frame_t* f,ei_callback_t c,ei_eventtype_t e, void* u);
#endif
