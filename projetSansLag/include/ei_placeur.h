#ifndef EI_PLACEUR_H
#define EI_PLACEUR_H

#include "ei_geometrymanager.h"



void runfunc_placeur(struct ei_widget_t* widget);

void releasefunc_placeur(struct ei_widget_t* widget);

typedef struct param_placeur {
    ei_geometrymanager_t * manager;
    ei_anchor_t anchor;
    int x;
    int y;
    int width;
    int height;
    float rel_x;
    float rel_y;
    float rel_width;
    float rel_height;
} param_placeur;

#endif
