#ifndef EI_TOPLEVEL_H
#define EI_TOPLEVEL_H

#include "ei_widget.h"
#include "ei_linked_event_t.h"


typedef struct ei_toplevel_t {
  ei_widget_t  widget;
  ei_color_t  color;
  int border_width;
  char*   title;
  ei_bool_t closable;
  ei_axis_set_t resizable;
  ei_size_t* min_size;
  ei_linked_event_t* event; // pour réagir à un evenement
  ei_linked_event_t* queue; // pour réagir à un evenement 
  ei_bool_t being_hold;
  ei_bool_t being_hold_bandeau;

  // Boutons speciaux
  ei_widget_t* close; 
  ei_widget_t* bandeau;
  ei_widget_t* redim; 
} ei_toplevel_t;


void toplevel_drawfunc (struct ei_widget_t*    widget,
			ei_surface_t           surface,
			ei_surface_t           pick_surface,
			ei_rect_t*             clipper);

void* toplevel_allocfunc();

void toplevel_releasefunc(struct ei_widget_t *widget);

void toplevel_setdefaultsfunc(struct ei_widget_t *widget);

void ajouter_evenement_toplevel(ei_toplevel_t* t,ei_callback_t c,ei_eventtype_t e, void* user_param);

void supprimer_evenement_toplevel(ei_toplevel_t* t,ei_callback_t c,ei_eventtype_t e, void* u);

#endif
