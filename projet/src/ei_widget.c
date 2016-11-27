#include "ei_linked_event_t.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ei_frame.h"
#include "ei_button.h"
#include "ei_toplevel.h"


void  ei_widget_destroy_rec  (ei_widget_t*  widget);

uint32_t idCourant = 1 ; // L'id courant du widget 

ei_widget_t* ei_widget_create  (ei_widgetclass_name_t class_name, ei_widget_t*  parent){

    // Cette fonction commence par vérifier que la classe dont le nom a été passé en paramètre est connue par la bibliothèque.

    // On reserve l'emplacement mémoire pour contenir l'objet

    // On cast en widget
    ei_widget_t* widget = (ei_widget_t*)ei_widgetclass_from_name(class_name)->allocfunc();

    // On créé un id pour le widget
    widget->pick_id =  idCourant;

    ei_color_t *couleur = malloc(sizeof(ei_color_t));
    /*
       couleur->red = (int8_t) idCourant ; 
       couleur->green = (int8_t) idCourant >> 8;
       couleur->blue =(int8_t) idCourant >> 16; 
       */
    couleur->red = (int8_t) idCourant ; 
    couleur->green = (int8_t) (idCourant /255);
    couleur->blue =(int8_t) (idCourant / (255*255)); 

    couleur->alpha = 0xFF;


    widget->pick_color = couleur;

    idCourant++;

    // On fixe l'attribut wclass à frame
    widget->wclass = ei_widgetclass_from_name(class_name);
    // On fixe son parent
    widget->parent=parent;
    // On ne sait pas encore s'il est suivi d'autres widgets
    widget->next_sibling = NULL; 

    // On ajoute un nouveau fils au parent 
    if(parent != NULL) {    
        // Cas ou le parent ne possede pas encore de fils
        if(parent->children_head == NULL && parent->children_tail == NULL){
            parent->children_head = widget;
            parent->children_tail = widget;
        }
        else {
            // On ajoute en tete
            //widget->next_sibling = parent->children_head;
            //parent->children_head = widget;

            //on ajoute en queue
            (parent->children_tail->next_sibling)=widget;
            parent->children_tail=widget;
        }
    }


    widget->wclass->setdefaultsfunc(widget);

    return widget;

}

void  ei_widget_destroy  (ei_widget_t*  widget){

    if(widget == NULL)
        return;

    // On détache le widget de son parent
    if(widget->parent != NULL){
        ei_widget_t* courant = widget->parent->children_head;

        // Si le parent possede un seul fils
        if(widget->parent->children_head==widget->parent->children_tail){
            widget->parent->children_head=NULL;
            widget->parent->children_tail=NULL;
        }

        // S'il s'agit du premier widget 
        else if(courant == widget){
            widget->parent->children_head = widget->next_sibling;
        }

        // Sinon cas general
        else {
            while(courant->next_sibling != widget){
                courant=courant->next_sibling;
            }
            // Ici on a donc courant->next_sibling qui pointe sur le widget a supprimer, on a alors juste à joindre au prochain
            courant->next_sibling = widget->next_sibling;

            // Enfin on vérifie qu'il ne s'agissait pas du widget de queue
            if(widget->parent->children_tail==widget){
                widget->parent->children_tail=courant;
            }
        }
    }


    // Fonction recursive pour detruire les fils 
    ei_widget_destroy_rec(widget->children_head);

    // On release les attributs de widget
    free(widget->pick_color);
    widget->parent=NULL; 
    widget->children_head=NULL; 
    widget->children_tail=NULL; 
    widget->next_sibling=NULL; 
    widget->geom_params=NULL;
    widget->content_rect=NULL;


    // On release la classe du widget 
    widget->wclass->releasefunc(widget);
    widget=NULL;

}

void  ei_widget_destroy_rec  (ei_widget_t*  widget){

    if(widget == NULL)
        return;

    ei_widget_destroy_rec(widget->children_head);
    ei_widget_destroy_rec(widget->next_sibling);    

    // On release les attributs de widget
    free(widget->pick_color);
    widget->parent=NULL; 
    widget->children_head=NULL; 
    widget->children_tail=NULL; 
    widget->next_sibling=NULL; 
    widget->geom_params=NULL;
    widget->content_rect=NULL;


    // On release la classe du widget 
    widget->wclass->releasefunc(widget);
    widget=NULL;
}



// ei_widget_pick est en fait une combinaison des fonctions get_couleur_pixel + get_widget_with_id définies dans ei_application.c  
ei_widget_t* ei_widget_pick   (ei_point_t*  where);



void  ei_frame_configure  (ei_widget_t*  widget,
        ei_size_t*  requested_size,
        const ei_color_t* color,
        int*   border_width,
        ei_relief_t*  relief,
        char**   text,
        ei_font_t*  text_font,
        ei_color_t*  text_color,
        ei_anchor_t*  text_anchor,
        ei_surface_t*  img,
        ei_rect_t**  img_rect,
        ei_anchor_t*  img_anchor) {

    if(widget == NULL)
        return;
    // printf("max22\n");
    // Debut de la fonction
    // On associe notre widget à un type frame
    ei_frame_t* frame = (ei_frame_t*)widget;


    // param ei_size_t* requested_size
    // plus judicieux de le mettre à la fin

    // Param const ei_color_t* color
    if (color != NULL) {
        frame->color = *color;
    }

    // param int* border width
    if (border_width != NULL) {
        frame->border_width = *border_width;
    }

    // Param ei_relief_t* relief
    if (relief != NULL) {
        frame->relief = *relief;

    }


    // Param char**  text
    if(text==NULL) {
        frame->text=NULL;
    }
    if (text != NULL && *text != NULL) {
        char* text2= malloc(strlen(*text)+1);
        strcpy(text2,*text);        
        frame->text=text2;
    }

    // Param ei_font_t*  text_font
    if (text_font != NULL) {
        frame->text_font = *text_font;
    }

    // Param ei_color_t*  text_color
    if (text_color != NULL) {
        frame->text_color = *text_color;
    }

    // Param ei_anchor_t*  text_anchor
    if (text_anchor != NULL) {
        frame->text_anchor = *text_anchor;
    }

    // Param ei_surface_t* img
    if (img == NULL) {

        frame->img = NULL;

    }
    if (img != NULL && *img != NULL){
        frame->img = img;
    }

    // Param ei_rect_t**  img_rect
    if (img_rect != NULL && img_rect != NULL) {
        frame->img_rect = *img_rect;
    }

    // Param ei_anchor_t*  img_anchor
    if (img_anchor != NULL) {
        frame->img_anchor = *img_anchor;
    }


    // param ei_size_t* requested_size
    if(requested_size == NULL) {    
        // On ecrit les valeurs par defaut
        if (widget->requested_size.width == 0 && widget->requested_size.height == 0) {
            if (img != NULL && *img!=NULL) {
                widget->requested_size = hw_surface_get_size(*img);
            }
            else if (text != NULL && *text!=NULL) {
                widget->requested_size = hw_surface_get_size(hw_text_create_surface(*text,frame->text_font,&(frame->text_color)));
            }
        }

    }
    else {
        widget->requested_size = *requested_size;
    }

}




void  ei_button_configure  (ei_widget_t*  widget,
        ei_size_t*  requested_size,
        const ei_color_t* color,
        int*   border_width,
        int*   corner_radius,
        ei_relief_t*  relief,
        char**   text,
        ei_font_t*  text_font,
        ei_color_t*  text_color,
        ei_anchor_t*  text_anchor,
        ei_surface_t*  img,
        ei_rect_t**  img_rect,
        ei_anchor_t*  img_anchor,
        ei_callback_t*  callback,
        void**   user_param) {
    // Debut de la fonction 
    // On associe notre widget à un type button
    ei_button_t* button = (ei_button_t*)widget;


    // param ei_size_t*  requested_size
    // plus judicieux de le mettre à la fin


    // param const ei_color_t* color
    if (color != NULL) {
        button->color = *color;
    }


    // param int*   border_width
    if (border_width != NULL) {
        button->border_width = *border_width;
    }


    // param int*   corner_radius
    if (corner_radius != NULL) {
        button->corner_radius = *corner_radius;
    }


    // param ei_relief_t*  relief
    if (relief != NULL) {
        button->relief = *relief;
    }


    // param char**   text
    if (text != NULL && *text != NULL) {
        char* text2 = malloc((strlen(*text)+1)*sizeof(char));
        strcpy(text2,*text);        
        button->text = text2;
    }


    // param ei_font_t*  text_font
    if (text_font != NULL) {
        button->text_font = *text_font;
    }


    // param ei_color_t*  text_color

    if (text_color != NULL) {
        button->text_color = *text_color;
    }


    // param ei_anchor_t*  text_anchor
    if (text_anchor != NULL) {
        button->text_anchor = *text_anchor;
    }


    // param ei_surface_t*  img
    if (img == NULL) {
        button->img = NULL;
    }
    if (img != NULL) {
        button->img = malloc(sizeof(ei_surface_t*));

        *(button->img) = *img;
    }


    // param ei_rect_t**  img_rect
    if (img_rect != NULL && *img_rect != NULL) {
        ei_rect_t* rect = malloc(sizeof(ei_rect_t));

        //button->img_rect = malloc(sizeof(ei_rect_t*));

        *rect = **img_rect;
        button->img_rect = rect;

    }


    // param ei_anchor_t*  img_anchor
    if (img_anchor != NULL) {
        button->img_anchor = *img_anchor;
    }


    // param ei_callback_t*  callback
    if (callback != NULL) {

        ajouter_evenement_bouton(button,*callback,ei_ev_mouse_buttondown, user_param);
    } 
    // param ei_size_t*  requested_size
    if (requested_size == NULL) {
        if (widget->requested_size.width == 0 && widget->requested_size.height == 0) {
            // On ecrit les valeurs par defaut
            if (img != NULL) {
                widget->requested_size = hw_surface_get_size(*img);
            }
            else if (text != NULL) {
                widget->requested_size = hw_surface_get_size(hw_text_create_surface(*text,button->text_font,&(button->text_color)));
            }
        }
    }
    else {
        widget->requested_size = *requested_size;
    }

}





void  ei_toplevel_configure  (ei_widget_t*  widget,
        ei_size_t*  requested_size,
        ei_color_t*  color,
        int*   border_width,
        char**   title,
        ei_bool_t*  closable,
        ei_axis_set_t*  resizable,
        ei_size_t**  min_size) {
    // Debut de la fonction
    // On associe notre widget à un type toplevel
    ei_toplevel_t* toplevel = (ei_toplevel_t*)widget;

    ei_size_t text_size;
    hw_text_compute_size(toplevel->title,ei_default_font,&(text_size.width),&(text_size.height));

    // param ei_size_t* requested_size
    if (requested_size != NULL) {
        widget->requested_size = *requested_size;
    }
    widget->requested_size.height += text_size.height; 

    // param ei_color_t*  color
    if (color != NULL) {
        toplevel->color = *color;
    }

    // param int*   border_width
    if (border_width != NULL) {
        toplevel->border_width = *border_width;
    }

    // param char**   title
    if (title != NULL && *title != NULL) {
        char* text2 = malloc((strlen(*title)+1)*sizeof(char));
        strcpy(text2,*title);        
        toplevel->title = text2;
    }

    // param ei_bool_t*  closable
    if (closable != NULL) {
        toplevel->closable = *closable;
        if(*closable==EI_FALSE){
            if(toplevel->close != NULL){
                ei_widget_destroy(toplevel->close);
                toplevel->close = NULL;
            }
        }
    }

    // param ei_axis_set_t*  resizable
    if (resizable != NULL) {
        toplevel->resizable = *resizable; 
    }

    // param ei_size_t**  min_size
    if (min_size != NULL) {
        toplevel->min_size = calloc(1,sizeof(ei_size_t));
        toplevel->min_size = *min_size;
    }

    toplevel->being_hold = EI_FALSE;
}



