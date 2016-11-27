#include "ei_widgetclass.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ei_button.h"
#include "ei_frame.h"
#include "ei_toplevel.h"

ei_widgetclass_t *listeClasse = NULL;

void ei_widgetclass_register(ei_widgetclass_t* widgetclass){
    widgetclass->next = listeClasse; // On ajoute en debut de liste
    listeClasse = widgetclass;
    printf("On a ajouté %s a la liste \n", listeClasse->name);
};

ei_widgetclass_t* ei_widgetclass_from_name (ei_widgetclass_name_t name){

    // Notre pointeur courant
    ei_widgetclass_t *courant = listeClasse;
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
};

void   ei_frame_register_class  (){
    ei_widgetclass_t *maFrame = malloc(sizeof(ei_widgetclass_t));

    // L'objet utilisera la fonctionframe_drawfunc pour se dessiner
    maFrame->drawfunc = &frame_drawfunc;
    maFrame->allocfunc = &frame_allocfunc;
    maFrame->releasefunc = &frame_releasefunc;
    maFrame->setdefaultsfunc = &frame_setdefaultsfunc;
    strcpy(maFrame->name, "frame");

    // On l'ajoute au register
    ei_widgetclass_register(maFrame);

}

void   ei_button_register_class  () {
    ei_widgetclass_t* button = malloc(sizeof(ei_widgetclass_t));

    // L'objet utilisera les fonction de ei_button
    button->drawfunc = &button_drawfunc;
    button->allocfunc = &button_allocfunc;
    button->releasefunc = &button_releasefunc;
    button->setdefaultsfunc = &button_setdefaultsfunc;
    strcpy(button->name, "button");

    ei_widgetclass_register(button);
}


void   ei_toplevel_register_class  () {
    // Debut de la fonction
    ei_widgetclass_t* toplevel = malloc(sizeof(ei_widgetclass_t));

    // L'objet utilisera les fonction de ei_toplevel
    toplevel->drawfunc = &toplevel_drawfunc;
    toplevel->allocfunc = &toplevel_allocfunc;
    toplevel->releasefunc = &toplevel_releasefunc;
    toplevel->setdefaultsfunc = &toplevel_setdefaultsfunc;
    strcpy(toplevel->name, "toplevel");

    ei_widgetclass_register(toplevel);
}
