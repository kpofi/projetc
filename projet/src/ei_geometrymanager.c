#include "ei_geometrymanager.h"
#include "ei_placeur.h"
#include "ei_widget.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


ei_geometrymanager_t *liste = NULL;


void ei_geometrymanager_register (ei_geometrymanager_t* geometrymanager)
{

    geometrymanager->next = liste; // On ajoute en debut de liste
    liste = geometrymanager;
    printf("On a ajouté %s a la liste des geometrymanagers \n", liste->name);


}



ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name)
{

    // Notre pointeur courant
    ei_geometrymanager_t *courant = liste;
    //int tourdeboucle= 0;

    while(courant != NULL)
    {
        // printf("tour %d \n", tourdeboucle);
        //printf("Courant non nul, on est sur %s \n", courant->name);
        if(strcmp(courant->name,name) == 0){
            // Les deux noms correspondent, on renvoie la classe courante
            //      printf("%s = %s donc on return courant \n\n", courant->name, name);
            return courant; 
        }
        // printf("%s != %s donc on avance \n", courant->name, name);
        // Sinon on avance dans la liste des classes
        courant = courant->next;
        //tourdeboucle++;
    }
    // Enfin si on n'a pas trouvé la classe, on renvoie null 
    return NULL;  

}


void ei_geometrymanager_unmap(ei_widget_t* widget)
{


}


void ei_register_placer_manager	()
{
    ei_geometrymanager_t *placeur = malloc(sizeof(ei_geometrymanager_t));

    // L'objet utilisera la fonctionframe_drawfunc pour se dessiner
    placeur->runfunc = &runfunc_placeur;
    placeur->releasefunc = &releasefunc_placeur;
    strcpy(placeur->name, "placeur");

    // On l'ajoute au register
    ei_geometrymanager_register(placeur);

}

void ei_place (ei_widget_t*		widget,
        ei_anchor_t*		anchor,
        int*			x,
        int*			y,
        int*			width,
        int*			height,
        float*			rel_x,
        float*			rel_y,
        float*			rel_width,
        float*			rel_height) {
    // Debut de la fonction


    if (widget->geom_params == NULL)
    {
        param_placeur *geom_params = malloc(sizeof(param_placeur));


        //VERFIIER SI LA MEMOIRE EST PLEINE

        //printf("On est dans ei_place\n");

        geom_params->manager = ei_geometrymanager_from_name("placeur");
        if (x == NULL) {

            geom_params->x = 0;
        }
        else {
            geom_params->x = *x;
        }

        if (y == NULL) {
            geom_params->y = 0;
        }
        else {
            geom_params->y = *y;
        }

        if (width == NULL) {
            geom_params->width = 0; 
        }
        else {
            geom_params->width = *width;
        }

        if (height == NULL) {
            geom_params->height = 0;
        }
        else {
            geom_params->height = *height;
        }

        if (rel_x == NULL) {
            geom_params->rel_x = 0.0;
        }
        else {
            geom_params->rel_x = *rel_x;
        }

        if (rel_y == NULL) {
            geom_params->rel_y = 0.0;
        }
        else {
            geom_params->rel_y = *rel_y;
        }

        if (rel_width == NULL) {
            geom_params->rel_width = 0.0;
        }
        else {
            geom_params->rel_width = *rel_width;
        }

        if (rel_height == NULL) {
            geom_params->rel_height = 0.0;
        }
        else {
            geom_params->rel_height = *rel_height;
        }

        if (anchor == NULL) {
            geom_params->anchor = ei_anc_northwest;
        }
        else {
            geom_params->anchor = *anchor;
        }

        widget->geom_params = (ei_geometry_param_t*) geom_params;
    }
    else {

        // param_placeur *geom_params = widget->geom_params;
        param_placeur *geom_params = (param_placeur*) widget->geom_params;


        //VERFIIER SI LA MEMOIRE EST PLEINE

        //printf("On est dans ei_place\n");

        geom_params->manager = ei_geometrymanager_from_name("placeur");
        if (x == NULL) { 
            //  geom_params->x = 0;
        }
        else {
            geom_params->x = *x;
        }

        if (y == NULL) {
            // geom_params->y = 0;
        }
        else {
            geom_params->y = *y;
        }

        if (width == NULL) {
            //  geom_params->width = 0; 
        }
        else {
            geom_params->width = *width;
        }

        if (height == NULL) {
            //  geom_params->height = 0;
        }
        else {
            geom_params->height = *height;
        }

        if (rel_x == NULL) {
            // geom_params->rel_x = 0.0;
        }
        else {
            geom_params->rel_x = *rel_x;
        }

        if (rel_y == NULL) {
            // geom_params->rel_y = 0.0;
        }
        else {
            geom_params->rel_y = *rel_y;
        }

        if (rel_width == NULL) {
            // geom_params->rel_width = 0.0;
        }
        else {
            geom_params->rel_width = *rel_width;
        }

        if (rel_height == NULL) {
            // geom_params->rel_height = 0.0;
        }
        else {
            geom_params->rel_height = *rel_height;
        }

        if (anchor == NULL) {
            //     geom_params->anchor = ei_anc_northwest;
        }
        else {
            geom_params->anchor = *anchor;
        }

        widget->geom_params = (ei_geometry_param_t*) geom_params;


    }   
}



