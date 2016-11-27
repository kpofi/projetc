#include <stdio.h>
#include <stdlib.h>

#include "ei_pofinet.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_application.h"


int main(int argc, char** argv) {
  ei_size_t screen_size  = {600, 600};
  ei_color_t root_bgcol  = {0xff, 0xff, 0xff, 0xff};
  //ei_point_t centre = {200, 200};
  //ei_linked_point_t* liste_point;

  ei_color_t couleur =  {0xf, 0x0f, 0xf, 0xff};
  ei_color_t clipper_col = {0xff, 0x00, 0x00, 0xff};

  ei_linked_rect_t rect;
  rect.next = NULL;
  rect.rect.top_left = (ei_point_t){0, 0};
  rect.rect.size = screen_size;

  ei_rect_t clipper;
  clipper.top_left = (ei_point_t){0, 0};
  clipper.size = (ei_size_t){550, 500};

  ei_rect_t bouton;
  bouton.top_left = (ei_point_t){500, 100};
  bouton.size = (ei_size_t){300,200};
  
  
  ei_surface_t root_surface = hw_create_window(&screen_size,EI_FALSE);

  // On met la racine en bgcol
  hw_surface_lock(root_surface);

  ei_fill(root_surface, &root_bgcol, &rect.rect);

  // dessin du clipper
  ei_fill(root_surface,&clipper_col,&clipper);

  hw_surface_unlock(root_surface);

  hw_surface_update_rects(root_surface, &rect);

  // Dessin de polygon
  //liste_point = arc(centre,100,0,90);

  /*liste_point = round_frame(bouton,20,EI_FALSE,EI_FALSE);
  
  hw_surface_lock(root_surface);
  
  ei_draw_polygon(root_surface,liste_point,couleur,NULL);

  hw_surface_unlock(root_surface);
  
  hw_surface_update_rects(root_surface, &rect);*/

  // On vérifie que le bouton ne dépasse pas du clipper
  draw_button(root_surface,&clipper,bouton,couleur,EI_FALSE,20,200,ei_relief_sunken);

  getchar();

  hw_quit();
  
}


