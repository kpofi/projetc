#include "ei_button.h"
#include "ei_frame.h"
#include "ei_toplevel.h"
#include "ei_draw.h"
#include <stdlib.h>
#include "ei_placeur.h"
#include <ei_types.h>
#include "ei_pofinet.h"
#include "ei_widget.h"
#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"
#include <stdio.h>



// === Evenement écoutant le bouton de fermeture ====
ei_bool_t toplevel_fermer(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
    ei_toplevel_t* toplevel = (ei_toplevel_t*) user_param;
    ei_widget_destroy(toplevel->close);
    ei_widget_destroy(toplevel->redim);
    ei_widget_destroy((ei_widget_t*) toplevel); 

    return EI_FALSE;
}


// === Evenement écoutant le bouton de redimmenssionnement ====
ei_bool_t toplevel_redim_clic(ei_widget_t* widget, ei_event_t* event, void* user_param)
{

    ei_toplevel_t* toplevel = (ei_toplevel_t*) user_param;
    toplevel->being_hold=EI_TRUE;
    return EI_FALSE; 
}


ei_bool_t toplevel_redim_unclic(ei_widget_t* widget, ei_event_t* event, void* user_param)
{ 
    ei_toplevel_t* toplevel = (ei_toplevel_t*) user_param;
    toplevel->being_hold = EI_FALSE;   
    return EI_FALSE; 
}

// === Evenement écoutant la barre d'etat du top level ====
ei_bool_t toplevel_barre_clic(ei_widget_t* widget, ei_event_t* event, void* user_param)
{

    ei_toplevel_t* toplevel = (ei_toplevel_t*) user_param;
    toplevel->being_hold_bandeau = EI_TRUE;
    return EI_FALSE; 
}


ei_bool_t toplevel_barre_unclic(ei_widget_t* widget, ei_event_t* event, void* user_param)
{ 
    ei_toplevel_t* toplevel = (ei_toplevel_t*) user_param;
    toplevel->being_hold_bandeau = EI_FALSE;   
    return EI_FALSE; 
}


// === Evenement écoutant le deplacement ====
ei_bool_t toplevel_redim_move(ei_widget_t* widget, ei_event_t* event, void* user_param)
{

    ei_widget_t* widget_toplevel = (ei_widget_t*) user_param;
    ei_toplevel_t* toplevel = (ei_toplevel_t*) user_param;

    if(toplevel->being_hold && toplevel->resizable != ei_axis_none){


        int nouvelle_width = event->param.mouse.where.x-20;
        int nouvelle_height = event->param.mouse.where.y-5;
        if (nouvelle_width > widget_toplevel->requested_size.width)
        {

            if (nouvelle_height > widget_toplevel->requested_size.height)
            {
                ei_place(widget_toplevel,NULL, NULL, NULL, &(nouvelle_width), &(nouvelle_height), NULL, NULL,NULL, NULL);
            }
            else
            {

                ei_place(widget_toplevel,NULL, NULL, NULL, &(nouvelle_width), NULL, NULL, NULL,NULL, NULL);
            } 
        }
        else if (nouvelle_height > widget_toplevel->requested_size.height)
        {

            ei_place(widget_toplevel,NULL, NULL, NULL, NULL, &(nouvelle_height), NULL, NULL,NULL, NULL);
        }
    }
    else if(toplevel->being_hold_bandeau){ 

        int nouveau_x = event->param.mouse.where.x-20;
        int nouveau_y = event->param.mouse.where.y-5;

        ei_place(widget_toplevel,NULL, &nouveau_x, &nouveau_y, NULL, NULL, NULL, NULL,NULL, NULL);

    }

    return EI_FALSE; 
}


void toplevel_drawfunc (struct ei_widget_t*    widget,
        ei_surface_t           surface,
        ei_surface_t           pick_surface,
        ei_rect_t*             clipper) {

    // Debut de la fonction
    ei_toplevel_t* toplevel = (ei_toplevel_t*)widget;
    ei_rect_t screen_location = widget->screen_location;
    ei_color_t color_ext = (ei_color_t){0x77, 0x77, 0x77, 0xff};
    ei_color_t color_int = toplevel->color;
    ei_size_t title_size;
    char* title = toplevel->title;
    ei_font_t title_font = ei_default_font;
    hw_text_compute_size(title,title_font,&(title_size.width),&(title_size.height));

    ei_color_t title_color = (ei_color_t){0xff, 0xff, 0xff, 0xff};
    int border_width = toplevel->border_width;

    // surface a mettre à jour
    ei_rect_t rect_surface = hw_surface_get_rect(surface);
    ei_linked_rect_t rects;
    rects.next = NULL;
    rects.rect = rect_intersection(screen_location,*clipper);
    rects.rect = rect_intersection(rect_surface,rects.rect);

    // taille de l'inteieur
    ei_size_t int_size;
    int_size.width = screen_location.size.width - 2*border_width;
    int_size.height = screen_location.size.height - border_width - title_size.height;

    ei_point_t centre;
    centre.x = screen_location.top_left.x + screen_location.size.width - (title_size.height)/2;
    centre.y = screen_location.top_left.y + (title_size.height)/2;
    int rayon = (title_size.height)/2;
    ei_linked_point_t* point_ext = arc(centre,rayon,0,90);
    ei_linked_point_t* cour = point_ext;
    while (cour->next != NULL) {
        cour = cour->next;
    }

    centre = ei_point_add(screen_location.top_left, ei_point(title_size.height/2,title_size.height/2));

    cour->next = arc(centre,rayon,90,180);

    while (cour->next != NULL) {
        cour = cour->next;
    }

    // Point4
    cour->next = calloc(1,sizeof(ei_linked_point_t));

    cour = cour->next;
    cour->point = ei_point_add(screen_location.top_left, ei_point(0,title_size.height));

    // point3
    cour->next = calloc(1,sizeof(ei_linked_point_t));

    cour = cour->next;
    cour->point = ei_point_add(screen_location.top_left, ei_point(screen_location.size.width,title_size.height));

    // cadre
    ei_linked_point_t* point_cadre;

    // cadre1
    point_cadre = calloc(1,sizeof(ei_linked_point_t));
    cour = point_cadre;
    cour->point = ei_point_add(screen_location.top_left, ei_point(0,title_size.height));

    // cadre2
    cour->next = calloc(1,sizeof(ei_linked_point_t));
    cour = cour->next;

    cour->point = ei_point_add(screen_location.top_left, ei_point(0,screen_location.size.height));

    // cadre3
    cour->next = calloc(1,sizeof(ei_linked_point_t));
    cour = cour->next;

    cour->point = ei_point_add(screen_location.top_left, ei_point(screen_location.size.width,screen_location.size.height));

    // cadre4
    cour->next = calloc(1,sizeof(ei_linked_point_t));
    cour = cour->next;

    cour->point = ei_point_add(screen_location.top_left, ei_point(screen_location.size.width,title_size.height));

    // cadre5
    cour->next = calloc(1,sizeof(ei_linked_point_t));
    cour = cour->next;

    cour->point = ei_point_add(screen_location.top_left, ei_point(screen_location.size.width - border_width,title_size.height));

    // cadre6
    ei_linked_point_t* prec = cour;
    cour->next = calloc(1,sizeof(ei_linked_point_t));
    cour = cour->next;

    cour->point = ei_point_add(prec->point, ei_point(0,int_size.height));

    // cadre7
    prec = cour;
    cour->next = calloc(1,sizeof(ei_linked_point_t));
    cour = cour->next;

    cour->point.x = screen_location.top_left.x + border_width;
    cour->point.y = prec->point.y;

    // cadre8
    prec = cour;
    cour->next = calloc(1,sizeof(ei_linked_point_t));
    cour = cour->next;

    cour->point.x = prec->point.x;
    cour->point.y = screen_location.top_left.y + title_size.height;

    ei_linked_point_t point_int1;
    ei_linked_point_t point_int2;
    ei_linked_point_t point_int3;
    ei_linked_point_t point_int4;
    point_int1.next = &point_int2;
    point_int2.next = &point_int3;
    point_int3.next = &point_int4;
    point_int4.next = NULL;

    point_int1.point = ei_point_add(screen_location.top_left, ei_point(border_width,title_size.height));

    point_int2.point = ei_point_add(point_int1.point, ei_point(int_size.width,0));

    point_int3.point = ei_point_add(point_int2.point, ei_point(0,int_size.height));

    point_int4.point = ei_point_sub(point_int3.point, ei_point(int_size.width,0));


    hw_surface_lock(surface);
    hw_surface_lock(pick_surface);

    // interieur
    ei_draw_polygon(surface,&point_int1,color_int,clipper);

    ei_draw_polygon(surface,point_ext,color_ext,clipper);

    ei_rect_t* contain_rect;

    if(widget->content_rect==NULL)
    {
        contain_rect=malloc(sizeof(ei_rect_t));
    }
    else
    {
        contain_rect= widget->content_rect;
    }

    contain_rect->top_left=point_int1.point;
    contain_rect->size=int_size;
    widget->content_rect=contain_rect;
    //cadre
    ei_draw_polygon(surface,point_cadre,color_ext,clipper);

    free_linked_point(point_cadre);
    free_linked_point(point_ext);

    ei_point_t where;
    where.x = screen_location.top_left.x + title_size.height;
    where.y = screen_location.top_left.y;
    ei_draw_text(surface,&where,title,title_font,&title_color,clipper);

    hw_surface_unlock(surface);
    hw_surface_unlock(pick_surface);

    //hw_surface_update_rects(surface, &rects);
    //hw_surface_update_rects(pick_surface, &rects);

    // bouton de fermeture
    ei_widget_t* button = toplevel->close;

    // placeur
    ei_size_t text_size;
    hw_text_compute_size(toplevel->title,ei_default_font,&(text_size.width),&(text_size.height));
    ei_anchor_t anchor = ei_anc_center;
    int x = screen_location.top_left.x + text_size.height/2;
    int y = screen_location.top_left.y + text_size.height/2;
    float rel_x = 0.0;
    float rel_y = 0.0;
    ei_place(button,&anchor,&x,&y,NULL,NULL,&rel_x,&rel_y,NULL,NULL);

    // petit carré
    ei_widget_t* carre = toplevel->redim;
    anchor = ei_anc_southeast;
    x = screen_location.top_left.x + screen_location.size.width;
    y = screen_location.top_left.y + screen_location.size.height;
    rel_x = 0.0;
    rel_y = 0.0;
    ei_place(carre,&anchor,&x,&y,NULL,NULL,&rel_x,&rel_y,NULL,NULL);

    // bandeau
    ei_widget_t* bandeau = toplevel->bandeau;


    // configuration
    ei_size_t b_requested_size = {screen_location.size.width, text_size.height};
    ei_color_t b_color = {0x00, 0x00, 0x00, 0x00};
    int b_border_width = 0;
    ei_relief_t b_relief = ei_relief_sunken;
    ei_frame_configure(bandeau,&b_requested_size,&b_color,&b_border_width,&b_relief,
            NULL,NULL,NULL,NULL,NULL,NULL,NULL);

    // positionnement
    anchor = ei_anc_northwest;
    x = screen_location.top_left.x;
    y = screen_location.top_left.y;
    rel_x = 0.0;
    rel_y = 0.0;
    ei_place(bandeau,&anchor,&x,&y,NULL,NULL,&rel_x,&rel_y,NULL,NULL);

}




void* toplevel_allocfunc(){
    return calloc(1, sizeof(ei_toplevel_t));
}




void toplevel_releasefunc(struct ei_widget_t *widget){

    widget->wclass=NULL;
    ei_toplevel_t* toplevel = (ei_toplevel_t*)widget;
    delete_linked_list_event(toplevel->event);
    toplevel->event=NULL;
    toplevel->queue=NULL;
    free(toplevel);

}


// permet de mettre les valeurs par defaut de toplevel
void toplevel_setdefaultsfunc(struct ei_widget_t *widget) {
    // Debut de la fonction
    // Cast
    ei_toplevel_t* toplevel = (ei_toplevel_t*)widget;

    // requested_size
    widget->requested_size = (ei_size_t){320, 240};

    // color
    toplevel->color = ei_default_background_color;

    // border_width
    toplevel->border_width = 4;

    // *title
    toplevel->title = calloc(8,sizeof(char));
    toplevel->title = "Toplevel";

    // closable
    toplevel->closable = EI_TRUE;

    // resizable
    toplevel->resizable = ei_axis_both;

    // min_size
    toplevel->min_size = calloc(1,sizeof(ei_size_t));
    *(toplevel->min_size) = (ei_size_t){160, 120};

    // callback
    toplevel->event = NULL;


    // taille du  text
    ei_size_t text_size;
    hw_text_compute_size(toplevel->title,ei_default_font,&(text_size.width),&(text_size.height));

    // prametres de configuration
    ei_size_t requested_size;
    ei_color_t color;
    int border_width;
    int corner_radius;
    ei_relief_t relief;

    // creation du bandeau
    ei_widget_t* bandeau = ei_widget_create("frame",widget->parent);
    toplevel->bandeau = bandeau;

    // configuration du bandeau
    // on cofigure le bandeau dans toplevel_drawfunc

    // Creation du bouton de fermeture
    toplevel->being_hold = EI_FALSE;
    toplevel->being_hold = EI_FALSE;

    // Creation du bouton de fermeture
    //ei_widget_t* button = ei_widget_create("button",widget);
    ei_widget_t* button = ei_widget_create("button",widget->parent);
    toplevel->close = button;

    // configuration du button
    requested_size = (ei_size_t){text_size.height/2 + 2, text_size.height/2 + 2};
    color = (ei_color_t){0xee, 0x00, 0x00, 0xff};
    border_width = 2;
    corner_radius = requested_size.width/2;
    relief = ei_relief_raised ;
    ei_button_configure((ei_widget_t*)button,&requested_size,&color,&border_width,&corner_radius,&relief,
            NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

    ajouter_evenement_bouton((ei_button_t*)button,toplevel_fermer,  ei_ev_mouse_buttondown, (void*)&toplevel);  

    // creation du petit carré de redimensionnement
    ei_widget_t* carre = ei_widget_create("frame",widget->parent);
    toplevel->redim = carre;

    // configuration du carre
    requested_size = ei_size(15,15);
    color = (ei_color_t){0x77, 0x77, 0x77, 0xff};
    border_width = 0;
    relief = ei_relief_sunken;
    ei_frame_configure(carre,&requested_size,&color,&border_width,&relief,
            NULL,NULL,NULL,NULL,NULL,NULL,NULL);


    // On ajoute les evenements pour le bouton de redimmension
    ajouter_evenement_frame((ei_frame_t*) carre,toplevel_redim_clic,  ei_ev_mouse_buttondown,(void*) toplevel);  

    ajouter_evenement_frame((ei_frame_t*) carre,toplevel_redim_move,  ei_ev_mouse_move,(void*) toplevel);  

    ajouter_evenement_frame((ei_frame_t*) carre,toplevel_redim_unclic,  ei_ev_mouse_buttonup,(void*) toplevel);  

    // On ajoute les evenements à la barre d'etat
    ajouter_evenement_frame((ei_frame_t*) bandeau,toplevel_barre_clic,  ei_ev_mouse_buttondown,(void*) toplevel);  

    ajouter_evenement_frame((ei_frame_t*) bandeau,toplevel_barre_unclic,  ei_ev_mouse_buttonup,(void*) toplevel);  

    ajouter_evenement_frame((ei_frame_t*) bandeau,toplevel_redim_move,  ei_ev_mouse_move,(void*) toplevel);  
}

void ajouter_evenement_toplevel(ei_toplevel_t* t,ei_callback_t c,ei_eventtype_t e, void* u){

    ei_linked_event_t* linked_event = malloc(sizeof(ei_linked_event_t));
    linked_event->callback = c;
    linked_event->event_type = e;
    linked_event->next = NULL;
    linked_event->user_param = u;

    if(t->queue == NULL){
        t->queue = linked_event;
        t->event = linked_event;
    }
    else {
        t->queue->next = linked_event;
        t->queue = linked_event;
    }
}

void supprimer_evenement_toplevel(ei_toplevel_t* t,ei_callback_t c,ei_eventtype_t e, void* u){


    ei_linked_event_t* prec = NULL;
    ei_linked_event_t* linked_event = t->event;

    while(linked_event != NULL){
        if(linked_event->event_type == e 
                && linked_event->callback == c
                && linked_event->user_param ==u){

            // Si on n'est pas en tete de liste
            if(prec != NULL)
            {
                prec->next = linked_event->next;    

                // S'il s'agit de la queue, on met a jour l'attribut queue
                if(linked_event->next == NULL){
                    t->queue=prec;
                }
            }
            else{
                // Si un seul element
                if(t->queue == t->event)
                {
                    t->queue= NULL;
                }

                t->event=t->event->next;
            }
            free(linked_event);
            return;
        }

        prec = linked_event;
        linked_event = linked_event->next;
    }
}


