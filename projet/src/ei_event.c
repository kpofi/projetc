#include "ei_event.h"
#include "ei_button.h"
#include "ei_application.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ei_frame.h"
#include "ei_toplevel.h"
#include "ei_linked_event_t.h"


void bind_tag(ei_widget_t* widget, ei_eventtype_t eventtype, ei_tag_t tag, ei_callback_t callback, void* user_param);

void unbind_tag(ei_widget_t* widget, ei_eventtype_t eventtype, ei_tag_t tag, ei_callback_t callback, void* user_param);


void ajouter_evenement_all(ei_widget_t* widget, ei_callback_t callback, ei_eventtype_t eventtype, void* user_param);

void supprimer_evenement_all(ei_widget_t* widget, ei_callback_t callback, ei_eventtype_t eventtype, void* user_param);

void ei_bind (ei_eventtype_t eventtype,
        ei_widget_t* widget,
        ei_tag_t tag,
        ei_callback_t callback,
        void* user_param){


    //ei_linked_event_t* linked_event = malloc(sizeof(ei_linked_event_t));

    if (widget != NULL){ 

        // On recupere le type du widget 
        if(strcmp(widget->wclass->name, "frame") ==0) {
            //printf("L'objet est bien un frame \n ");
            ei_frame_t* frame = (ei_frame_t*) widget;
            ajouter_evenement_frame(frame,callback,eventtype,user_param);
        }
        else if(strcmp(widget->wclass->name, "button") ==0) {
            //printf("L'objet est bien un bouton \n ");         
            ei_button_t* bouton = (ei_button_t*) widget;
            ajouter_evenement_bouton(bouton,callback,eventtype,user_param);
        }
        else if(strcmp(widget->wclass->name, "toplevel") ==0) {
            ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
            ajouter_evenement_toplevel(toplevel,callback,eventtype,user_param);
        }

    }

    if(tag != NULL) {

        if (strcmp(tag, "all") == 0) { 
            ajouter_evenement_frame((ei_frame_t*) ei_app_root_widget(),callback,eventtype,user_param);

            ajouter_evenement_all(ei_app_root_widget(),callback, eventtype, user_param); 

        }
        else {
            bind_tag(ei_app_root_widget(), eventtype, tag,callback,user_param);
        }

    }


}

void bind_tag(ei_widget_t* widget, ei_eventtype_t eventtype, ei_tag_t tag, ei_callback_t callback, void* user_param) {

    if(widget != NULL){
        if(strcmp(widget->wclass->name,tag) == 0){
            ei_bind (eventtype, widget,NULL,callback, user_param);
        }
        bind_tag(widget->children_head,eventtype, tag, callback, user_param);
        bind_tag(widget->next_sibling,eventtype, tag, callback, user_param);
    }

}

void ajouter_evenement_all(ei_widget_t* widget, ei_callback_t callback, ei_eventtype_t eventtype, void* user_param){
    if (widget != NULL){ 
        // On recupere le type du widget 
        if(strcmp(widget->wclass->name, "frame") ==0) {
            ei_frame_t* frame = (ei_frame_t*) widget;
            ajouter_evenement_frame(frame,callback,eventtype,user_param);
        }
        else if(strcmp(widget->wclass->name, "button") ==0) {
            ei_button_t* bouton = (ei_button_t*) widget;
            ajouter_evenement_bouton(bouton,callback,eventtype,user_param);
        }
        else if(strcmp(widget->wclass->name, "toplevel") ==0) {
            ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
            ajouter_evenement_toplevel(toplevel,callback,eventtype,user_param);
        }


        ajouter_evenement_all(widget->children_head, callback, eventtype, user_param); 
        ajouter_evenement_all(widget->next_sibling, callback, eventtype, user_param);

    }
}

void supprimer_evenement_all(ei_widget_t* widget, ei_callback_t callback, ei_eventtype_t eventtype, void* user_param){
    if (widget != NULL){ 
        // On recupere le type du widget 
        if(strcmp(widget->wclass->name, "frame") ==0) {
            ei_frame_t* frame = (ei_frame_t*) widget;
            supprimer_evenement_frame(frame,callback,eventtype,user_param);
        }
        else if(strcmp(widget->wclass->name, "button") ==0) {
            ei_button_t* bouton = (ei_button_t*) widget;
            supprimer_evenement_bouton(bouton,callback,eventtype,user_param);
        }
        else if(strcmp(widget->wclass->name, "toplevel") ==0) {
            ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
            supprimer_evenement_toplevel(toplevel,callback,eventtype,user_param);
        }


        supprimer_evenement_all(widget->children_head, callback, eventtype, user_param); 
        supprimer_evenement_all(widget->next_sibling, callback, eventtype, user_param);

    }


}



void ei_unbind (ei_eventtype_t eventtype,
        ei_widget_t* widget,
        ei_tag_t tag,
        ei_callback_t callback,
        void* user_param)
{



    if (widget != NULL){ 

        // On recupere le type du widget 
        if(strcmp(widget->wclass->name, "frame") ==0) {
            ei_frame_t* frame = (ei_frame_t*) widget;
            supprimer_evenement_frame(frame,callback,eventtype,user_param);
        }
        else if(strcmp(widget->wclass->name, "button") ==0) {
            ei_button_t* bouton = (ei_button_t*) widget;
            supprimer_evenement_bouton(bouton,callback,eventtype,user_param);
        }
        else if(strcmp(widget->wclass->name, "toplevel") ==0) {
            ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
            supprimer_evenement_toplevel(toplevel,callback,eventtype,user_param);
        }


    }

    // Vaut NULL si 1 seul bouton, definie les autres sinon
    if(tag != NULL) {

        if (strcmp(tag, "all") == 0) {  
            supprimer_evenement_frame((ei_frame_t*) ei_app_root_widget(),callback,eventtype,user_param);
            supprimer_evenement_all(ei_app_root_widget(),callback, eventtype, user_param); 


        }

        else {
            unbind_tag(ei_app_root_widget(), eventtype, tag,callback,user_param);
        }

    }

    return;
}


void unbind_tag(ei_widget_t* widget, ei_eventtype_t eventtype, ei_tag_t tag, ei_callback_t callback, void* user_param) {

    if(widget != NULL){
        if(strcmp(widget->wclass->name,tag) == 0){
            ei_unbind (eventtype, widget,NULL,callback, user_param);
        }
        unbind_tag(widget->children_head,eventtype, tag, callback, user_param);
        unbind_tag(widget->next_sibling,eventtype, tag, callback, user_param);
    }

}



