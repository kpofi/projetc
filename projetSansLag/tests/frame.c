#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"


/*
 * ei_main --
 *
 * Main function of the application.
 */
ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_sym == SDLK_ESCAPE) {
		ei_app_quit_request();
		return EI_TRUE;
	}
	
	return EI_FALSE;
}

int ei_main(int argc, char** argv)
{
 ei_size_t screen_size  = {600, 600};
 ei_color_t root_bgcol  = {0x52, 0x7f, 0xb4, 0xff};

 ei_widget_t* frame;
 ei_size_t frame_size  = {300,200};
 int  frame_x   = 150;
 int  frame_y   = 200;
 ei_color_t frame_color  = {0x88, 0x88, 0x88, 0xff};
 ei_relief_t frame_relief  = ei_relief_raised;
 int  frame_border_width = 6;



//TROISIEME FENETRE IMBRIQUE avec un cadre :)
 ei_widget_t* frame2;
 ei_size_t frame_size2  = {0,0};
 int  frame_x2   =0;
 int  frame_y2   =0;
 ei_color_t frame_color2  = {0xFF, 0x00, 0x00, 0xff};
 ei_relief_t frame_relief2  = ei_relief_raised;
 int  frame_border_width2 = 6;
 
 float rel_x2 = 0.5 ;
 float rel_y2 = 0.5 ;
 
 float rel_width2 = 0.5 ;
 float rel_height2 = 0.5 ;
 ei_anchor_t anchor2 = ei_anc_center;


 /* Create the application and change the color of the background. */
 ei_app_create(&screen_size, EI_FALSE);

    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

 /* Create, configure and place the frame on screen. */
frame = ei_widget_create("frame", ei_app_root_widget());

    ei_frame_configure(frame, &frame_size, &frame_color,       &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,   NULL, NULL, NULL);
    
    ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );


// Configure the 3eme frame
frame2 = ei_widget_create("frame", frame);

    ei_frame_configure(frame2, &frame_size2, &frame_color2,&frame_border_width2, &frame_relief2, NULL, NULL, NULL, NULL,   NULL, NULL, NULL);
    
    ei_place(frame2, &anchor2, &frame_x2, &frame_y2, NULL, NULL, &rel_x2, &rel_y2,&rel_width2, &rel_height2 );


ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);
 /* Run the application's main loop. */
 ei_app_run();

 /* We just exited from the main loop. Terminate the application (cleanup). */
 ei_app_free();

 return (EXIT_SUCCESS);
}
