#include "ei_application.h"
#include <stdio.h>
#include <stdlib.h>
#include "ei_frame.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_widgetclass.h"
#include <string.h>
#include "ei_types.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"

// Le widget root
ei_widget_t* root_widget = NULL;
ei_surface_t root_surface;
ei_surface_t root_offscreen;
ei_size_t window_size;
ei_bool_t keep_running = EI_TRUE;
//ei_draw_polygon(pick_surface,&first_point,*laCouleur,clipper);

uint32_t colorToId(ei_color_t color){
    return (color.red + (color.blue << 8) + (color.green << 16));    
}

ei_widget_t* get_widget_with_id(ei_widget_t* widget,uint32_t id) {

    if (widget == NULL) 
        return NULL;

    if(widget->pick_id == id){
        return widget;     
    }    

    else {
        ei_widget_t* trouve;

        trouve = get_widget_with_id(widget->children_head, id);

        if( trouve == NULL){
            trouve = get_widget_with_id(widget->next_sibling, id); 
            return trouve;
        }
        else {
            return trouve;
        }

    }

}


void dessiner_racine() {
    // ===== DEBUT DU DESSIN DE ROOT ===== */y

    if(root_surface == NULL) return;
    root_widget->wclass->drawfunc(root_widget,root_surface,root_offscreen,NULL);
}

uint32_t get_couleur_pixel(ei_event_t event) { 
    // Renvoie la couleur sous forme d'entier du pixel pointé par la souris
    int8_t* pixel;
    uint32_t* origine_32;
    uint8_t* couleur_4c;

    int r;
    int g;
    int b;
    int a;


    //printf(" X: %d ,Y: %d  \n", event.param.mouse.where.x, event.param.mouse.where.y);


    hw_surface_lock(root_offscreen); 
    pixel = (int8_t*) hw_surface_get_buffer(root_offscreen);
    origine_32 = (uint32_t*) pixel;
    hw_surface_get_channel_indices(root_offscreen, &r,&g,&b,&a); 

    origine_32 = origine_32+ event.param.mouse.where.y*window_size.width + event.param.mouse.where.x; 

    couleur_4c = (uint8_t*) origine_32;        
    // hw_surface_set_origin(root_surface,origine);
    hw_surface_unlock(root_offscreen);  

    //printf("Couleur pixel RGBA: %x %x %x %x \n", couleur_4c[r], couleur_4c[g], couleur_4c[b], couleur_4c[a]);    

    // printf("id renvoyé: %u \n ", (couleur_4c[r] + (couleur_4c[g] << 8) + (couleur_4c[b] << 16)));    

    return (couleur_4c[r] + (couleur_4c[g] << 8) + (couleur_4c[b] << 16));    
}

void parcours_profondeur(ei_widget_t *courant);

void executer_callback_widget(ei_event_t event){ 

    if(event.type == ei_ev_mouse_buttondown ||event.type == ei_ev_mouse_buttonup || event.type == ei_ev_mouse_move){
        // On verifie le widget pointé par le curseur

        uint32_t idWidget;
        idWidget = get_couleur_pixel(event);
        ei_widget_t* courant = root_widget;
        ei_widget_t* widget_with_id;

        widget_with_id =get_widget_with_id(courant, idWidget);

        if( widget_with_id != NULL){


            // Si le callback est consumant ou pas
            ei_bool_t loop_callback = EI_FALSE; 
            ei_linked_event_t* l_e;

            if(strcmp(widget_with_id->wclass->name,"frame") == 0) 
            {
                l_e = ((ei_frame_t*) widget_with_id)->event;
            }
            else if(strcmp(widget_with_id->wclass->name,"button") == 0)
            {
                l_e = ((ei_button_t*) widget_with_id)->event;

            }
            // if(strcmp(widget_with_id->wclass->name,"frame") == 0) ei_frame_t* objet = widget_with_id;



            while(loop_callback == EI_FALSE && l_e != NULL){

                if(l_e->event_type == event.type){


                    loop_callback = l_e->callback(widget_with_id, &event,l_e->user_param);
                    dessiner_racine(); 
                    parcours_profondeur(root_widget);

                    hw_surface_update_rects(root_surface, NULL); // Mettre les rects ici
                    hw_surface_update_rects(root_offscreen, NULL); // linked rects




                }
                l_e = l_e->next;


            }

        }
    }

    else if(event.type == ei_ev_keydown || event.type == ei_ev_keyup ){
        // On n'a pas encore implementé le focus donc on considere que l'evenement à été codé sur le tag "all", on evalue alors seulement la racine 

        ei_frame_t* frame;
        frame = (ei_frame_t*) root_widget;

        ei_linked_event_t* l_e;
        l_e = frame->event;

        // Tant que la racine possede des evenements 
        while(l_e != NULL)
        {
            // Si c'est le bon evenement
            if(l_e->event_type == event.type){
                // Petite astuce pour arreter la recherche de callback si le callback vaut true
                if(l_e->callback(root_widget, &event,l_e->user_param))
                    return;
            }
            l_e = l_e->next;
        }



    } 
}

void parcours_profondeur(ei_widget_t *courant){

    if(root_surface == NULL) return;

    ei_widget_t* children_courant = courant->children_head;

    while(children_courant!=NULL)
    {      

        children_courant->geom_params->manager->runfunc(children_courant);

        children_courant->wclass->drawfunc(children_courant,root_surface,root_offscreen,children_courant->parent->content_rect);

        parcours_profondeur(children_courant);
        children_courant=children_courant->next_sibling;

    }


}
// Fin fonction persos

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen)
{ 

    hw_init();

    // On enregistre les classes
    ei_frame_register_class(); 
    ei_button_register_class(); // on ajoute button
    ei_toplevel_register_class(); // on ajoute toplevel
    ei_register_placer_manager();

    // On enregistre la taille de la fenetre
    window_size = *main_window_size;

    root_surface = hw_create_window(main_window_size,fullscreen);      

    root_offscreen = hw_surface_create(root_surface,main_window_size, EI_TRUE);

    // Creation du widget root associé
    root_widget = ei_widget_create("frame", NULL);


    root_widget->screen_location=hw_surface_get_rect(root_surface);
    root_widget->content_rect=&(root_widget->screen_location);


}

void ei_app_free(){
    // On detruit tous les widgets
    ei_widget_destroy(ei_app_root_widget());

    // Free des classes
    free(ei_widgetclass_from_name("frame"));
    free(ei_widgetclass_from_name("button"));
    free(ei_widgetclass_from_name("toplevel"));


    printf("app free \n");

}

void ei_app_run(){
    printf("app run \n");

    ei_event_t			event;

    dessiner_racine();    
    parcours_profondeur(root_widget);
    hw_surface_update_rects(root_surface, NULL); // Mettre les rects ici
    hw_surface_update_rects(root_offscreen, NULL); // linked rects


#ifdef __APPLE__
    // On osx, the window opens empty and does not perform the first redraw.
    //	Force a redraw after 0.5s
    hw_event_schedule_app(500, (void*)NULL);
#endif



    while(keep_running) {
        // TODO: Pas tout redessiner, appeler invalidate_rect

#ifdef __APPLE__
        // On osx, the window opens empty and does not perform the first redraw.
        //	Force a redraw after 0.5s
        hw_event_schedule_app(500, (void*)NULL);
#endif
        if(root_surface == NULL) return;

        hw_event_wait_next(&event);  

        executer_callback_widget(event);        

    }

}


void ei_app_invalidate_rect(ei_rect_t* rect){

    printf("app_invalidate \n");
}

void ei_app_quit_request(){
    keep_running = EI_FALSE;
    hw_surface_free(root_surface);
    hw_surface_free(root_offscreen); 
    hw_quit();


    printf("ei_app_quit_request \n");
    root_surface = NULL;
    root_offscreen = NULL;
}


ei_widget_t* ei_app_root_widget (){
    return (ei_widget_t*) root_widget;
}

ei_surface_t ei_app_root_surface(){
    return root_surface;
}



