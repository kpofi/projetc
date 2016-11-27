#ifndef EI_POFINET_H
#define EI_POFINET_H

#include "ei_widget.h"
#include "ei_draw.h"


ei_linked_point_t* arc (ei_point_t c, int r, int ang_deb, int ang_fin);

ei_linked_point_t* round_frame(ei_rect_t rect, int r, ei_bool_t totalite, ei_bool_t haute);

void free_linked_point (ei_linked_point_t* liste_point);

ei_rect_t rect_intersection (ei_rect_t rect1, ei_rect_t rect2);

ei_point_t trans_anchor_point(ei_anchor_t anchor,ei_rect_t rect, ei_size_t size);

void draw_button (ei_surface_t surface,
		  ei_rect_t* clipper,
		  ei_rect_t screen_location,
		  ei_color_t color,
		  ei_bool_t pressed,
          int corner_radius,
		  int border_width,
		  ei_relief_t relief);

#endif
