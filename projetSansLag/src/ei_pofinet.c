#include "ei_pofinet.h"
#include "math.h"
#include "ei_application.h"
#include <stdio.h>
#include "ei_frame.h"
#include "ei_widgetclass.h"
#include <string.h>
#include <stdlib.h>
#include "ei_types.h"
#include "ei_event.h"
#include "ei_utils.h"

#define PI 3.14159265


ei_linked_point_t* arc (ei_point_t c, int r, int ang_deb, int ang_fin) {
    ei_linked_point_t* liste_point;
    ei_linked_point_t* cour;
    float pas = PI/20.0;
    float ang = ang_deb;

    liste_point = malloc(sizeof(ei_linked_point_t));

    cour = liste_point;

    while ((int)ang < ang_fin) {
        cour->point = (ei_point_t){c.x + (int)r*cos(ang*PI/180.0),  c.y - (int)r*sin(ang*PI/180.0)};

        ang = ang + pas;

        cour->next = malloc(sizeof(ei_linked_point_t));

        cour = cour->next;
    }

    cour->point = (ei_point_t){c.x + (int)r*cos(ang_fin*PI/180.0),  c.y - (int)r*sin(ang_fin*PI/180.0)};

    cour->next = NULL;

    return liste_point;
}





ei_linked_point_t* round_frame(ei_rect_t rect, int r, ei_bool_t totalite, ei_bool_t haute) {

    ei_linked_point_t* liste_point;
    ei_linked_point_t* cour;
    ei_point_t centre;

    // cas ou on doit dessiner un cercle
    if ((rect.size.height/2 == r) && (rect.size.width/2 == r)) {
        // dessin de la totalité du disque
        centre = ei_point_add(rect.top_left,ei_point(r,r));
        if (totalite) {
            liste_point = arc(centre,r,0,360);
        }
        // dessin de la partie haute
        else if (haute) {
            liste_point = arc(centre,r,45,225);
        }
        // dessinde la partie basse
        else {
            liste_point = arc(centre,r,225,360);
            ei_linked_point_t* cour = liste_point;
            while (cour->next != NULL) {
                cour = cour->next;
            }
            cour->next = arc(centre,r,0,45);
        }

        return liste_point;
    }
    // fin disque

    if (totalite) {
        // premier centre
        centre = ei_point_add(rect.top_left,ei_point(rect.size.width - r,r));
        //centre.x = rect.top_left.x + rect.size.width - r;
        //centre.y = rect.top_left.y + r;

        liste_point = arc(centre,r,0,90);
        cour = liste_point;

        while (cour->next != NULL) {
            cour = cour->next;
        }

        // deuiexme arc
        centre = ei_point_add(rect.top_left, ei_point(r,r));
        //centre.x = rect.top_left.x + r;
        //centre.y = rect.top_left.y + r;

        cour->next = arc(centre,r,90,180);

        while (cour->next != NULL) {
            cour = cour->next;
        }

        // troisime arc
        centre = ei_point_add(rect.top_left, ei_point(r,rect.size.height -r));
        //centre.x = rect.top_left.x + r;
        //centre.y = rect.top_left.y + rect.size.height - r;

        cour->next = arc(centre,r,180,270);

        while (cour->next != NULL) {
            cour = cour->next;
        }

        // quatrieme arc
        centre = ei_point_add(rect.top_left, ei_point(rect.size.width - r,rect.size.height - r));
        //centre.x = rect.top_left.x + rect.size.width - r;
        //centre.y = rect.top_left.y + rect.size.height - r;

        cour->next = arc(centre,r,270,360);

        while (cour->next != NULL) {
            cour = cour->next;
        }
    }
    else if (haute) {
        // premier centre
        centre.x = rect.top_left.x + rect.size.width - r;
        centre.y = rect.top_left.y + r;

        liste_point = arc(centre,r,45,90);
        cour = liste_point;

        while (cour->next != NULL) {
            cour = cour->next;
        }

        // deuiexme arc
        centre.x = rect.top_left.x + r;
        centre.y = rect.top_left.y + r;

        cour->next = arc(centre,r,90,180);

        while (cour->next != NULL) {
            cour = cour->next;
        }

        // troisime arc
        centre.x = rect.top_left.x + r;
        centre.y = rect.top_left.y + rect.size.height - r;

        cour->next = arc(centre,r,180,225);

        while (cour->next != NULL) {
            cour = cour->next;
        }

        // point intermediare
        cour->next = malloc(sizeof(ei_linked_point_t));

        cour = cour->next;
        cour->point = (ei_point_t){rect.top_left.x + rect.size.height/2, rect.top_left.y + rect.size.height/2};

        cour->next = malloc(sizeof(ei_linked_point_t));

        cour = cour->next;
        cour->point = (ei_point_t){rect.top_left.x + rect.size.width - rect.size.height/2, rect.top_left.y + rect.size.height/2};

        cour->next = NULL;

    }
    else {
        // premier centre
        centre.x = rect.top_left.x + rect.size.width - r;
        centre.y = rect.top_left.y + r;

        liste_point = arc(centre,r,0,45);
        cour = liste_point;

        while (cour->next != NULL) {
            cour = cour->next;
        }

        // Points intermediare
        cour->next = malloc(sizeof(ei_linked_point_t));

        cour = cour->next;
        cour->point = (ei_point_t){rect.top_left.x + rect.size.width - rect.size.height/2, rect.top_left.y + rect.size.height/2};

        cour->next = malloc(sizeof(ei_linked_point_t));

        cour = cour->next;
        cour->point = (ei_point_t){rect.top_left.x + rect.size.height/2, rect.top_left.y + rect.size.height/2};

        // troisime arc
        centre.x = rect.top_left.x + r;
        centre.y = rect.top_left.y + rect.size.height - r;

        cour->next = arc(centre,r,225,270);

        while (cour->next != NULL) {
            cour = cour->next;
        }

        // quatrieme arc
        centre.x = rect.top_left.x + rect.size.width - r;
        centre.y = rect.top_left.y + rect.size.height - r;

        cour->next = arc(centre,r,270,360);

        while (cour->next != NULL) {
            cour = cour->next;
        }

    }

    return liste_point;
}



void free_linked_point (ei_linked_point_t* liste_point) {
    ei_linked_point_t* cour;
    while (liste_point != NULL) {
        cour = liste_point;
        liste_point = liste_point->next;
        free(cour);
    }
}


// si l'intersection est nul on renvoi width = 0 et heigth = 0
// dans ce cas ne pas mettre a jour la surface

ei_rect_t rect_intersection (ei_rect_t rect1, ei_rect_t rect2) {
    // Debut de la fonction
    ei_rect_t rect;
    ei_size_t size;

    // cas ou l'intersection est nul
    // abcisse
    if (rect1.top_left.x + rect1.size.width < rect2.top_left.x) {
        rect = rect1;
        rect.size = (ei_size_t){0, 0};
        return rect;
    }
    if (rect2.top_left.x + rect2.size.width < rect1.top_left.x) {
        rect = rect2;
        rect.size = (ei_size_t){0, 0};
        return rect;
    }

    // ordonee (cas ou l'intersection est nul)
    if (rect1.top_left.y + rect1.size.height < rect2.top_left.y) {
        rect = rect1;
        rect.size = (ei_size_t){0, 0};
        return rect;
    }
    if (rect2.top_left.y + rect2.size.height < rect1.top_left.y) {
        rect = rect2;
        rect.size = (ei_size_t){0, 0};
        return rect;
    }

    // cas ou l'intersection n'est pas nul
    // abcisse
    if (rect1.top_left.x > rect2.top_left.x) {
        rect.top_left.x = rect1.top_left.x;
        // on sur size.width >= 0 car l'intersection est non nul
        size.width = rect2.size.width - (rect1.top_left.x - rect2.top_left.x);

        // configuration de width
        if (size.width < rect1.size.width) {
            rect.size.width = size.width;
        }
        else {
            rect.size.width = rect1.size.width;
        }
    }
    else {
        rect.top_left.x = rect2.top_left.x;
        // on sur size.width >= 0 car l'intersection est non nul
        size.width = rect1.size.width - (rect2.top_left.x - rect1.top_left.x);

        // configuration de width
        if (size.width < rect2.size.width) {
            rect.size.width = size.width;
        }
        else {
            rect.size.width = rect2.size.width;
        }
    }

    // ordonnée
    if (rect1.top_left.y > rect2.top_left.y) {
        rect.top_left.y = rect1.top_left.y;

        // on sur size.height >= 0 car l'intersection est non nul
        size.height = rect2.size.height - (rect1.top_left.y - rect2.top_left.y);

        // configuration de width
        if (size.height < rect1.size.height) {
            rect.size.height = size.height;
        }
        else {
            rect.size.height = rect1.size.height;
        }

    }
    else {
        rect.top_left.y = rect2.top_left.y;

        // on sur size.height >= 0 car l'intersection est non nul
        size.height = rect1.size.height - (rect2.top_left.y - rect1.top_left.y);

        // configuration de width
        if (size.height < rect2.size.height) {
            rect.size.height = size.height;
        }
        else {
            rect.size.height = rect2.size.height;
        }

    }

    return rect;
}





ei_point_t trans_anchor_point(ei_anchor_t anchor,ei_rect_t rect, ei_size_t size) {
    // Debut de la fonction
    ei_point_t point;
    ei_point_t centre;
    ei_point_t top_left = rect.top_left;
    centre = (ei_point_t){top_left.x + (rect.size.width)/2, top_left.y + (rect.size.height)/2};

    if (anchor == ei_anc_center) {
        // Center
        point = (ei_point_t){centre.x - (size.width)/2, centre.y - (size.height)/2};
    }
    else if (anchor == ei_anc_north) {
        // The top side
        point = (ei_point_t){centre.x - (size.width)/2, top_left.y};
    }
    else if (anchor == ei_anc_northeast) {
        // The top-right corner
        point = (ei_point_t){top_left.x + rect.size.width - size.width, top_left.y};
    }
    else if (anchor == ei_anc_east) {
        // The right side
        point = (ei_point_t){top_left.x + rect.size.width - size.width, centre.y - (size.height)/2};
    }
    else if (anchor == ei_anc_southeast) {
        // The bottom-right corner
        point = (ei_point_t){top_left.x + rect.size.width - size.width, top_left.y + rect.size.height - size.height};
    }
    else if (anchor == ei_anc_south) {
        // The bottom side
        point = (ei_point_t){centre.x - (size.width)/2, top_left.y + rect.size.height - size.height};
    }
    else if (anchor == ei_anc_southwest) {
        // The bottom-left corner
        point = (ei_point_t){top_left.x, top_left.y + rect.size.height - size.height};
    }
    else if (anchor == ei_anc_west) {
        // The left side
        point = (ei_point_t){top_left.x, top_left.y + (rect.size.height - size.height)/2};
    }
    else if (anchor == ei_anc_northwest) {
        // The top-left corner
        point = (ei_point_t){top_left.x, top_left.y};
    }
    else {
        // Default = center
        point = (ei_point_t){centre.x - (size.width)/2, centre.y - (size.height)/2};
    }

    return point;
}


void draw_button (ei_surface_t surface,
        ei_rect_t* clipper,
        ei_rect_t screen_location,
        ei_color_t color,
        ei_bool_t pressed,
        int corner_radius,
        int border_width,
        ei_relief_t relief) {
    // Debut de la fonction
    ei_linked_point_t* liste_point1;
    ei_linked_point_t* liste_point2;
    ei_linked_point_t* liste_point3;
    ei_color_t couleur_bas;
    ei_color_t couleur_haut;
    ei_rect_t  button = screen_location;
    int rayonInt = corner_radius - border_width;
    int bord = border_width;

    ei_linked_rect_t rects;
    // a optimiser
    // pour l'instant on va mettre a jour toute la surface
    rects.rect = hw_surface_get_rect(surface);
    rects.next = NULL;
    // on fait une premiere intersection avec le root_screen aka surface
    rects.rect = rect_intersection(rects.rect,button);
    // on fait une deuxieme intersection avec le clipper
    rects.rect = rect_intersection(rects.rect,*clipper);

    // initialisation des couleurs
    if (relief == ei_relief_sunken) {
        // initialisation des couleurs
        couleur_bas.red = 3*(color.red)/2;
        couleur_bas.green = 3*(color.green)/2;
        couleur_bas.blue = 3*(color.blue)/2;
        couleur_bas.alpha = color.alpha;
        // couleur de la partie d'en haut
        couleur_haut.red = (color.red)/2;
        couleur_haut.green = (color.green)/2;
        couleur_haut.blue = (color.blue)/2;
        couleur_haut.alpha = color.alpha;
    }
    else {
        // initialisation des couleurs
        couleur_haut.red = 3*(color.red)/2;
        couleur_haut.green = 3*(color.green)/2;
        couleur_haut.blue = 3*(color.blue)/2;
        couleur_haut.alpha = color.alpha;
        // couleur de la partie du bas
        couleur_bas.red = (color.red)/2;
        couleur_bas.green = (color.green)/2;
        couleur_bas.blue = (color.blue)/2;
        couleur_bas.alpha = color.alpha;
    }

    // intialisation de la liste de point
    liste_point1 = round_frame(button,corner_radius,EI_FALSE,EI_TRUE);
    liste_point2 = round_frame(button,corner_radius,EI_FALSE,EI_FALSE);

    ei_rect_t rect;
    rect.top_left = (ei_point_t){button.top_left.x + bord, button.top_left.y + bord};
    rect.size = (ei_size_t){button.size.width - 2*bord, button.size.height - 2*bord};
    liste_point3 = round_frame(rect,rayonInt,EI_TRUE,EI_FALSE);

    // Debut du dessin
    // partie d'en haut
    hw_surface_lock(surface);

    if(!pressed){
        ei_draw_polygon(surface,liste_point1,couleur_haut,clipper);
        ei_draw_polygon(surface,liste_point2,couleur_bas,clipper);
    }
    else {
        ei_draw_polygon(surface,liste_point1,couleur_bas,clipper);
        ei_draw_polygon(surface,liste_point2,couleur_haut,clipper);
    }

    // troisieme couche
    ei_draw_polygon(surface,liste_point3,color,clipper);


    hw_surface_unlock(surface);

    if (rects.rect.size.width == 0 && rects.rect.size.height == 0) {
        // rein faire
    }
    else {
        hw_surface_update_rects(surface,&rects);
    }

    // on libere les points
    free_linked_point(liste_point1);
    free_linked_point(liste_point2);
    free_linked_point(liste_point3);

}
