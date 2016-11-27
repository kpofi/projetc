#ifndef EI_BUTTON_H
#define EI_BUTTON_H

#include "ei_widget.h"
#include "ei_linked_event_t.h"



typedef struct ei_button_t {
  ei_widget_t  widget;
  
  int border_width;
  ei_color_t  color;
  int corner_radius;
  ei_relief_t  relief;
  char*   text;
  ei_font_t  text_font;
  ei_color_t  text_color;
  ei_anchor_t  text_anchor;
  ei_surface_t*  img; //on met un pointeur ici car surface est 'grand'
  ei_rect_t*  img_rect;
  ei_anchor_t  img_anchor;
  ei_bool_t pressed; // Si le bouton est pressé ou pas 

  ei_linked_event_t* event; // pour réagir à un evenement
    ei_linked_event_t* queue; // Pointeur du dernier event pour inserer en queue facilement
} ei_button_t;


void button_drawfunc (struct ei_widget_t*    widget,
		      ei_surface_t           surface,
		      ei_surface_t           pick_surface,
		      ei_rect_t*             clipper);

void* button_allocfunc();

void button_releasefunc(struct ei_widget_t *widget);

void button_setdefaultsfunc(struct ei_widget_t *widget);

void ajouter_evenement_bouton(ei_button_t* b,ei_callback_t c,ei_eventtype_t e, void** user_param);
 
void supprimer_evenement_bouton(ei_button_t* b,ei_callback_t c,ei_eventtype_t e, void** u);
#endif
