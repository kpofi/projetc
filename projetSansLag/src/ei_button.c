#include "ei_button.h"
#include "ei_draw.h"
#include <stdlib.h>
#include "ei_placeur.h"
#include <ei_types.h>
#include "ei_pofinet.h"
#include "ei_utils.h"


// Fonctions interne
ei_bool_t intern_button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
    ei_button_t* bouton = (ei_button_t*) widget;
    bouton->pressed = EI_TRUE;
    return EI_FALSE; // Car cas par defaut
}

ei_bool_t intern_button_unpress(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
    ei_button_t* bouton = (ei_button_t*) widget;
    bouton->pressed = EI_FALSE;
    return EI_FALSE; // Car cas par defaut
}

ei_bool_t intern_button_mouse_leave(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
    ei_button_t* bouton = (ei_button_t*) widget;
    if(event->param.mouse.where.x < widget->screen_location.top_left.x 
            || event->param.mouse.where.y < widget->screen_location.top_left.y 
            || event->param.mouse.where.x > widget->screen_location.top_left.x+widget->screen_location.size.width 
            || event->param.mouse.where.y > widget->screen_location.top_left.y+widget->screen_location.size.height){ 
        bouton->pressed = EI_FALSE;
    }

    //printf("mouse move \n");
    return EI_FALSE; // Car cas par defaut
}


void ajouter_evenement_bouton(ei_button_t* b,ei_callback_t c,ei_eventtype_t e, void** u){


    ei_linked_event_t* linked_event = malloc(sizeof(ei_linked_event_t));
    linked_event->callback = c;
    linked_event->event_type = e;
    linked_event->next = NULL;
    if(u==NULL)
        linked_event->user_param = NULL;
    else
        linked_event->user_param = *u;

    if(b->queue == NULL){
        b->queue = linked_event;
        b->event = linked_event;
    }
    else {
        b->queue->next = linked_event;
        b->queue = linked_event;
    }
}

void button_drawfunc (struct ei_widget_t*    widget,
        ei_surface_t           surface,
        ei_surface_t           pick_surface,
        ei_rect_t*             clipper) {

    // Debut de la fonction
    ei_button_t* button = (ei_button_t*)widget;
    ei_color_t color = button->color;
    ei_color_t* pick_color = widget->pick_color; // couleur pour l'offscreen
    ei_rect_t screen_location = widget->screen_location; // postion du widget (button)
    ei_surface_t* img = button->img;

    // Cadre dans lequel on met le text
    ei_rect_t text_location;
    text_location.top_left.x = screen_location.top_left.x + button->corner_radius;
    text_location.top_left.y = screen_location.top_left.y + button->corner_radius;
    text_location.size.width = screen_location.size.width - 2*(button->corner_radius);
    text_location.size.height = screen_location.size.height - 2*(button->corner_radius);

    char* text = button->text;
    ei_font_t text_font = button->text_font;
    ei_color_t text_color = button->text_color;
    ei_anchor_t text_anchor = button->text_anchor;
    ei_point_t where;
    ei_size_t size_text;

    // surface a mettre a jour
    ei_linked_rect_t rects_text;
    //rects_text.rect = hw_surface_get_rect(surface);
    rects_text.next = NULL;

    // surface de dessin à mettre à jour
    ei_linked_rect_t rects_img;
    rects_img.next = NULL;

    // Dessine button sans text ni image
    draw_button(surface,clipper,screen_location,color, button->pressed,button->corner_radius,button->border_width,button->relief);

    // Dessiner l'image s'il y'en a
    if (img != NULL) {



        ei_rect_t* src_rect = button->img_rect;
        ei_rect_t* dest_rect = malloc(sizeof(ei_rect_t));

        dest_rect->top_left = widget->screen_location.top_left; 
        dest_rect->size = button->img_rect->size;

        hw_surface_lock(surface);
        hw_surface_lock(*img);

        // CA MARCHE MAIS PUZZLE ENTIER ?
        //ei_copy_surface(surface,&screen_location,*img,&screen_location,EI_TRUE);

        ei_copy_surface(surface,dest_rect,*img,src_rect,EI_TRUE);

        hw_surface_unlock(*img);
        hw_surface_unlock(surface);

        rects_img.rect = rect_intersection(*clipper,screen_location);

        //hw_surface_update_rects(surface,&rects_img);

        free(dest_rect);
    }


    // ecire le text s'il y'en a
    if (text != NULL) {
        // Initialisation de size_text
        hw_text_compute_size(text,text_font,&(size_text.width),&(size_text.height));

        where = trans_anchor_point(text_anchor,text_location,size_text);

        rects_text.rect.top_left = where;
        rects_text.rect.size = size_text;

        hw_surface_lock(surface);

        ei_draw_text(surface,&where,text,text_font,&text_color,&screen_location);

        hw_surface_unlock(surface);

        //hw_surface_update_rects(surface,&rects_text);
    }


    // Dessin de button sur le offscreen
    ei_linked_point_t* liste_point;
    liste_point = round_frame(screen_location,button->corner_radius,EI_TRUE,EI_FALSE);

    hw_surface_lock(pick_surface);

    ei_draw_polygon(pick_surface,liste_point,*pick_color,clipper);

    hw_surface_unlock(pick_surface);

    // on m'a pas besoin de mettre a jour pick_surface
    // car elle n'est pas affiché à l'ecran

    // on libere les points
    free_linked_point(liste_point);

}



void* button_allocfunc() {
    // Allocation d'un bloc intitialisé à 0
    return calloc(1,sizeof(ei_button_t));
}



void button_releasefunc(struct ei_widget_t *widget) {
    // Cast
    widget->wclass=NULL;
    ei_button_t* button = (ei_button_t*)widget;
    delete_linked_list_event(button->event);
    button->event=NULL;
    button->queue=NULL;
    free(button);

}




void button_setdefaultsfunc(struct ei_widget_t *widget) {
    // Cast
    ei_button_t* button = (ei_button_t*)widget;

    // requested_size
    widget->requested_size = (ei_size_t){0, 0};

    // border_width
    button->border_width = k_default_button_border_width;

    // color
    button->color = ei_default_background_color;

    // corner_radius
    button->corner_radius = k_default_button_corner_radius;

    // relief
    button->relief = ei_relief_raised;

    // *text
    button->text = NULL;

    // text_font
    button->text_font = ei_default_font;

    // text_color
    button->text_color = ei_font_default_color;

    // text_anchor
    button->text_anchor = ei_anc_center;

    // *img
    button->img = NULL;

    // *img_rect
    button->img_rect = NULL;

    // img_anchor
    button->img_anchor = ei_anc_center;

    // *callback
    button->event = NULL;
    button->queue = NULL;

    // Pressé ? 
    button->pressed = EI_FALSE;


    // On ajoute maintenant les evenements par defaut
    ajouter_evenement_bouton(button,intern_button_press,  ei_ev_mouse_buttondown, NULL);  

    ajouter_evenement_bouton(button,intern_button_unpress,  ei_ev_mouse_buttonup, NULL);  

    //     ajouter_evenement_bouton(button,intern_button_mouse_leave,  ei_ev_mouse_move, NULL);  
}


void supprimer_evenement_bouton(ei_button_t* b,ei_callback_t c,ei_eventtype_t e, void** u){

    ei_linked_event_t* prec = NULL;
    ei_linked_event_t* linked_event = b->event;

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
                    b->queue=prec;
                }
            }
            else{
                // Si un seul element
                if(b->queue == b->event)
                {
                    b->queue= NULL;
                }

                b->event=b->event->next;
            }
            free(linked_event);
            //printf("unbind+free!\n");
            return;
        }

        prec = linked_event;
        linked_event = linked_event->next;
    }
}

