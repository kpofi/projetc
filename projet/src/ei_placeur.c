#include "ei_geometrymanager.h"
#include "ei_placeur.h"
#include "ei_widget.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void runfunc_placeur(struct ei_widget_t* widget) {


    //On récupère d'abord les parametre du widget 
    param_placeur* parametre = (param_placeur*) widget->geom_params;

    int x;
    int y;
    //float rel_x;
    //float rel_y;   
    float rel_width;
    float rel_height;


    int width = 0;
    int heigth = 0;



    // CONTAINER DU PARRENT OU SCRREN_LOCATION DU PARRENT ? 
    int height_parent = widget->parent->content_rect->size.height;
    int width_parent = widget->parent->content_rect->size.width;
    int x_parent = widget->parent->content_rect->top_left.x;
    int y_parent = widget->parent->content_rect->top_left.y;

    x = parametre->x + x_parent;

    y = parametre->y + y_parent;

    x = x + (parametre->rel_x) * width_parent;

    y = y + (parametre->rel_y) * height_parent;



    //REQUESTED SIDE .H FUX !!!!

    // MISE A JOUT DU WIDTH
    // remplace null par 0 ou 0.0
    if (parametre->width == 0 && parametre->rel_width == 0.0){
        width = widget->requested_size.width;  
    }
    else {

        if (parametre->width != 0) {
            width = (parametre->width);  
        }  
        else {
            width = 0;
        }

        if (parametre->rel_width != 0.0 ) {
            rel_width = (parametre->rel_width);  
        }   
        else {
            rel_width = 0.0 ;
        }


        width = width + rel_width * (widget->parent->content_rect->size.width) ;

    }



    //MISE A JOUT DU HEIGTH
    // on remplace NULL par 0 ou 0.0
    if (parametre->height == 0 && parametre->rel_height == 0.0) {
        heigth = widget->requested_size.height;  
    }
    else {
        if (parametre->height != 0) {
            heigth= (parametre->height);  
        }  
        else {
            heigth = 0;
        }

        if (parametre->rel_height != 0.0) {
            rel_height = (parametre->rel_height);  
        }   
        else {
            rel_height = 0.0 ;
        }


        heigth = heigth + rel_height * widget->parent->content_rect->size.height ;

    }


    // puis on calcule ou est le nouveau widgjet avec les paramettre par défault si certains paramettres sont à NULL :

    ei_rect_t  screen_location;
    screen_location.size.height = heigth;
    screen_location.size.width = width;




    // en fonction de l'encre on met à jour le x et le y
    // ici on remplace NULL par ei_anc_northwest
    if ((parametre->anchor == ei_anc_northwest) || ((parametre->anchor) == ei_anc_northwest) || ((parametre-> anchor) == ei_anc_none)) {

        screen_location.top_left.x = x;
        screen_location.top_left.y = y;

    }
    else {


        // en fonction de l'encre on met à jour le x 

        if (((parametre->anchor)==ei_anc_west) || ((parametre-> anchor) == ei_anc_southwest)) {

            screen_location.top_left.x = x;
        }

        if ( ((parametre-> anchor) == ei_anc_north) || ((parametre-> anchor) ==ei_anc_center) || ((parametre-> anchor) == ei_anc_south) ) {

            screen_location.top_left.x = x - width/2;
        }

        if ( ((parametre-> anchor) == ei_anc_northeast) || ((parametre-> anchor) == ei_anc_east) || ((parametre-> anchor) == ei_anc_southeast) ) {

            screen_location.top_left.x = x - width;
        }      

        //  on met a jout le y

        if ( ((parametre-> anchor) == ei_anc_north) || ((parametre-> anchor) == ei_anc_northeast) ) {

            screen_location.top_left.y = y;
        }

        if ( ((parametre-> anchor) == ei_anc_west) || ((parametre-> anchor) == ei_anc_center) || ((parametre-> anchor) == ei_anc_east) ) {

            screen_location.top_left.y = y - heigth/2;
        }

        if ( ((parametre->anchor) == ei_anc_southwest) || ((parametre->anchor) == ei_anc_south) || ((parametre->anchor) == ei_anc_southeast) ) {

            screen_location.top_left.y = y - heigth;
        }

    }


    widget->screen_location = screen_location;

    // CAR ON PEUT ECRIRE DANS TOUTE LA SOUS FENETRE
    if(strcmp(widget->wclass->name,"toplevel") != 0) 
    {
        widget->content_rect = &(widget->screen_location);
    } 



}



void releasefunc_placeur(struct ei_widget_t* widget) {

    param_placeur* parametre = (param_placeur*) widget->geom_params;
    free(parametre);
    widget->geom_params=NULL;
    return;
}
