#include <stdlib.h>
#include "asteroid.h"
#include "bitmaps.h"
#include "linked_list.h"

#define BITMAP_ARGS(x, y, image) x, y, image.width, image.height, (unsigned char*)image.pixel_data
#define GET_OFFSET(length) (length/2)

#define MIN_X 0
#define MAX_X 320
#define MIN_Y 0
#define MAX_Y 240

#define OUT_OF_X(x_position) (x_position < MIN_X || x_position > MAX_X)
#define OUT_OF_Y(y_position) (y_position < MIN_Y || y_position > MAX_Y)
#define OUT_OF_BOUNDS(asteroid) (OUT_OF_X(asteroid->x_position) || OUT_OF_Y(asteroid->y_position))

linked_list_t asteroid_list;

/**
 * FUNCTIONS FOR DRAWING AND ERASING A GIVEN ASTEROID
 */
void draw_asteroid(asteroid_t* asteroid_p) {
	short plot_x, plot_y;

	// Determine where to place the image on the LCD
	plot_x = asteroid_p->x_position - GET_OFFSET(asteroid_image.width);
	plot_y = asteroid_p->y_position - GET_OFFSET(asteroid_image.height);

	// Draw the image to the LCD
	GLCD_Bitmap(BITMAP_ARGS(plot_x, plot_y, asteroid_image));	
}
void erase_asteroid(asteroid_t* asteroid_p) {
	short plot_x, plot_y;

	// Determine where to place the image on the LCD
	plot_x = asteroid_p->x_position - GET_OFFSET(clear_asteroid_image.width);
	plot_y = asteroid_p->y_position - GET_OFFSET(clear_asteroid_image.height);

	// Draw the image to the LCD
	GLCD_Bitmap(BITMAP_ARGS(plot_x, plot_y, clear_asteroid_image));	
}

/**
 * FUNCTIONS FOR DYNAMICALLY CREATING AND DESTROYING ASTEROIDS
 */
asteroid_t *create_asteroid(pixel_t x, pixel_t y, speed_t x_speed, speed_t y_speed) {

	asteroid_t* asteroid_p;

	asteroid_p = (asteroid_t*)malloc(sizeof(asteroid_t));
	asteroid_p->x_position = x;
	asteroid_p->y_position = y;
	asteroid_p->x_speed = x_speed;
	asteroid_p->y_speed = y_speed;

	return asteroid_p;
}
void add_asteroid(pixel_t x, pixel_t y, speed_t x_speed, speed_t y_speed) {
	asteroid_t *asteroid_p = create_asteroid(x, y, x_speed, y_speed);
	add_list_node(&asteroid_list, asteroid_p);
}
void delete_asteroid(list_node_t *node) {
	free(node->data_p);
	remove_list_node(&asteroid_list, node);
}

/**
 *	FUNCTION FOR UPDATING THE POSITION OF THE ASTEROID
 */
void update_asteroid(asteroid_t *asteroid_p) {
	asteroid_p->x_position += asteroid_p->x_speed;
	asteroid_p->y_position += asteroid_p->y_speed;
}

/**
 * UPDATE ALL ASTEROID OBJECTS AND LCD FOR THE NEXT FRAME
 */
void update_asteroids_in_frame() {

	list_node_t *current_node;
	asteroid_t *asteroid_p;

	current_node = asteroid_list.head;

	// Iterate through the list of asteroids
	while (current_node) {

		asteroid_p = (asteroid_t *)(current_node->data_p);

		// Erase the previous frame's image of the asteroid
		// erase_asteroid(asteroid_p);

		// Update the position of the asteroid
		update_asteroid(asteroid_p);

		// Draw if within bounds, otherwise delete 
		if (OUT_OF_BOUNDS(asteroid_p)) {
			delete_asteroid(current_node);
		} else {
			draw_asteroid(asteroid_p);
		}

		current_node = current_node->next;
	}
}

/**
 * INTERFACE FUNCTIONS (ACCESSIBLE BY MAIN.C)
 */
void spawn_asteroid() {
	add_asteroid(0, 0, 2, 2);
}

unsigned short get_num_asteroids() {
	return asteroid_list.size;
}
linked_list_t *get_asteroid_list(void) {
	return &asteroid_list;
}