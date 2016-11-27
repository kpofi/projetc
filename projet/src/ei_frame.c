#include "ei_frame.h"
#include "ei_draw.h"
#include <stdlib.h>
#include "ei_placeur.h"
#include <ei_types.h>
#include "ei_pofinet.h"
#include <stdio.h>
#include <string.h>
void frame_drawfunc (struct ei_widget_t*    widget,
        ei_surface_t           surface,
        ei_surface_t           pick_surface,
        ei_rect_t*             clipper)
{

    //printf("On est dans le drawfunc de ir_frame \n");
    ei_frame_t* frame = (ei_frame_t*)widget;
    ei_color_t couleur = frame->color;

    ei_color_t *laCouleur = widget->pick_color;
    ei_color_t CouleurCadre; 
    CouleurCadre=couleur; 

    if(frame->relief!=ei_relief_none) {

        if(frame->relief == ei_relief_raised){
            // On choisis des couleurs pour faire le cadre plus foncé
            (CouleurCadre.red > 205)? (CouleurCadre.red = 255) : (CouleurCadre.red+=50);  
            (CouleurCadre.green > 205)? (CouleurCadre.green = 255) : (CouleurCadre.green+=50);  
            (CouleurCadre.blue > 205)? (CouleurCadre.blue = 255) : (CouleurCadre.blue+=50);  


        }  

        else {
            // On choisis des couleurs pour faire le cadre plus foncé
            (CouleurCadre.red < 30)? (CouleurCadre.red = 0) : (CouleurCadre.red-=30);  
            (CouleurCadre.green< 30)? (CouleurCadre.green = 0) : (CouleurCadre.green-=30);  
            (CouleurCadre.blue < 30)? (CouleurCadre.blue = 0) : (CouleurCadre.blue-=30);  
        } 
    }


    int border_width=frame->border_width;

    //printf(" couleur: %c \n", frame->color.red);


    ei_rect_t parametre = widget->screen_location;

    // Partie à mettre à jour
    ei_linked_rect_t rects;
    rects.rect = widget->screen_location;
    rects.next = NULL;

    int x = (parametre.top_left.x);
    int y = (parametre.top_left.y);
    int width = (parametre.size.width);
    int height = (parametre.size.height);


    ei_linked_point_t first_point;
    ei_linked_point_t second_point;
    ei_linked_point_t third_point;
    ei_linked_point_t fourth_point;


    first_point.next=&second_point;
    second_point.next=&third_point;
    third_point.next=&fourth_point;
    fourth_point.next=NULL;

    //on ajoute les 4 points du frame
    first_point.point.x=x;
    first_point.point.y=y; 

    second_point.point.x=x+width;
    second_point.point.y=y; 

    third_point.point.x=x+width;
    third_point.point.y=y+height; 

    fourth_point.point.x=x;
    fourth_point.point.y=y+height;   


    hw_surface_lock(surface); 
    hw_surface_lock(pick_surface);

    ei_draw_polygon(surface,&first_point,CouleurCadre,clipper); 
    ei_draw_polygon(pick_surface,&first_point,*laCouleur,clipper);

    hw_surface_unlock(surface);
    hw_surface_unlock(pick_surface);

    first_point.point.x+=border_width;
    first_point.point.y+=border_width; 

    second_point.point.x-=border_width;
    second_point.point.y+=border_width; 

    third_point.point.x-=border_width;
    third_point.point.y-=border_width; 

    fourth_point.point.x+=border_width;
    fourth_point.point.y-=border_width;  


    ei_draw_polygon(surface,&first_point,couleur,clipper);

    ei_rect_t screen_location = widget->screen_location;
    ei_surface_t* img = frame->img;
    // Dessin de l'image
    if (img != NULL && *img != NULL) {

        ei_size_t img_size = hw_surface_get_size(*img);
        ei_point_t pos_anchor = trans_anchor_point(frame->img_anchor,screen_location, img_size);
        ei_rect_t img_rect;
        img_rect.top_left = pos_anchor;
        img_rect.size = img_size;
        img_rect = rect_intersection(*clipper,img_rect);
        ei_copy_surface(surface,&img_rect,*img,NULL,EI_TRUE);
    }

    // Ecriture des textes
    // ecire le text s'il y'en a
    // char* text = frame->text;
    //    printf("%s \n",frame->text);
    ei_font_t text_font = frame->text_font;
    ei_anchor_t text_anchor = frame->text_anchor;
    ei_color_t text_color = frame->text_color;
    if (frame->text != NULL) {


        char text[strlen(frame->text)+1];

        strcpy(text,frame->text);

        ei_size_t text_size;
        ei_point_t where;

        // Initialisation de size_text
        hw_text_compute_size(text,
                text_font,
                &(text_size.width),
                &(text_size.height));

        // Cadre dans lequel on met le text
        ei_rect_t text_location;
        text_location.top_left.x = screen_location.top_left.x + frame->border_width;
        text_location.top_left.y = screen_location.top_left.y + frame->border_width;
        text_location.size.width = screen_location.size.width - 2*(frame->border_width);
        text_location.size.height = screen_location.size.height - 2*(frame->border_width);
        where = trans_anchor_point(text_anchor,text_location,text_size);


        // fprintf(stderr,"surface : %p, where : %p , text : %s, text_color: %p", surface,&where,text,&text_color);
        ei_draw_text(surface,&where,text,text_font,&text_color,&screen_location);

        // ei_draw_text(surface,&where,text,NULL,&text_color,NULL);
    }


    // on met à jour que la partie qui a été modifié
    //hw_surface_update_rects(surface, &rects);
    //hw_surface_update_rects(pick_surface, &rects);




}

void* frame_allocfunc(){
    return calloc(1, sizeof(ei_frame_t));
}

void frame_releasefunc(struct ei_widget_t *widget){
    widget->wclass=NULL;
    ei_frame_t* frame = (ei_frame_t*) widget;
    delete_linked_list_event(frame->event);
    frame->event=NULL;
    frame->queue=NULL;
    free(frame);
}

void frame_setdefaultsfunc(struct ei_widget_t *widget) {
    // Debut de la fonction
    // Cast
    ei_frame_t* frame = (ei_frame_t*)widget;

    // requested_size
    widget->requested_size = (ei_size_t){0, 0};

    // color
    frame->color = ei_default_background_color;

    // border_width
    frame->border_width = 0;

    // relief
    frame->relief = ei_relief_none;

    // *text
    frame->text = NULL;

    // text_font
    frame->text_font = ei_default_font;

    // text_color
    frame->text_color = ei_font_default_color;

    // text_anchor
    frame->text_anchor = ei_anc_center;

    // *img
    frame->img = NULL;

    // *img_rect (penser à liberer l'espace si le pointeur n'est pas null)
    frame->img_rect = NULL;

    // img_anchor
    frame->img_anchor = ei_anc_center;

    // callback
    frame->event = NULL;
}

void ajouter_evenement_frame(ei_frame_t* f,ei_callback_t c,ei_eventtype_t e, void* u){

    ei_linked_event_t* linked_event = malloc(sizeof(ei_linked_event_t));
    linked_event->callback = c;
    linked_event->event_type = e;
    linked_event->next = NULL;
    if(u==NULL)
        linked_event->user_param = NULL;
    else
        linked_event->user_param = u;


    if(f->queue == NULL){
        f->queue = linked_event;
        f->event = linked_event;
    }
    else {
        f->queue->next = linked_event;
        f->queue = linked_event;
    }
}

void supprimer_evenement_frame(ei_frame_t* f,ei_callback_t c,ei_eventtype_t e, void* u){


    ei_linked_event_t* prec = NULL;
    ei_linked_event_t* linked_event = f->event;

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
                    f->queue=prec;
                }
            }
            else{
                // Si un seul element
                if(f->queue == f->event)
                {
                    f->queue= NULL;
                }

                f->event=f->event->next;
            }
            //printf("frame detruite!\n");
            free(linked_event);
            return;
        }

        prec = linked_event;
        linked_event = linked_event->next;
    }
}


