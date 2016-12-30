#include <stdlib.h>
#include "bullet.h"
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
#define OUT_OF_BOUNDS(bullet) (OUT_OF_X(bullet->x_position) || OUT_OF_Y(bullet->y_position))

linked_list_t bullet_list;

/**
 * FUNCTIONS FOR DRAWING AND ERASING A GIVEN BULLET
 */
void draw_bullet(bullet_t* bullet_p) {
	short plot_x, plot_y;

	// Determine where to place the image on the LCD
	plot_x = bullet_p->x_position - GET_OFFSET(bullet_image.width);
	plot_y = bullet_p->y_position - GET_OFFSET(bullet_image.height);

	// Draw the image to the LCD
	GLCD_Bitmap(BITMAP_ARGS(plot_x, plot_y, bullet_image));
}
void erase_bullet(bullet_t* bullet_p) {
	short plot_x, plot_y;

	// Determine where to place the image on the LCD
	plot_x = bullet_p->x_position - GET_OFFSET(clear_bullet_image.width);
	plot_y = bullet_p->y_position - GET_OFFSET(clear_bullet_image.height);

	// Draw the image to the LCD
	GLCD_Bitmap(BITMAP_ARGS(plot_x, plot_y, clear_bullet_image));
}

/**
 * FUNCTIONS FOR DYNAMICALLY CREATING AND DESTROYING BULLETS
 */
bullet_t* create_bullet(pixel_t x, pixel_t y, speed_t x_speed, speed_t y_speed) {

	bullet_t* bullet_p;

	bullet_p = (bullet_t*)malloc(sizeof(bullet_t));
	bullet_p->x_position = x;
	bullet_p->y_position = y;
	bullet_p->x_speed = x_speed;
	bullet_p->y_speed = y_speed;

	return bullet_p;
}
void add_bullet(pixel_t x, pixel_t y, speed_t x_speed, speed_t y_speed) {
	bullet_t* bullet_p = create_bullet(x, y, x_speed, y_speed);
	add_list_node(&bullet_list, bullet_p);
}
void delete_bullet(list_node_t *node) {
	free(node->data_p);
	remove_list_node(&bullet_list, node);
}

/**
 * FUNCTION FOR UPDATING THE POSITION OF A BULLET
 */
void update_bullet(bullet_t* bullet_p) {
	bullet_p->x_position += bullet_p->x_speed;
	bullet_p->y_position += bullet_p->y_speed;
}

/**
 * INTERFACE FUNCTIONS (ACCESSIBLE BY MAIN.C)
 */
void shoot(ship_t* ship_p) {
	
	pixel_t initial_x, initial_y;
	speed_t x_speed, y_speed;

	// Determine the initial position of the bullet
	initial_x = ship_p->x_position;
	initial_y = ship_p->y_position;

	// Determine the speed of the bullet
	x_speed = 5;
	y_speed = 5;

	// Generate the bullet object and store it appropriately
	add_bullet(initial_x, initial_y, x_speed, y_speed);
}
void update_bullets_in_frame() {

	list_node_t *current_node;
	bullet_t *bullet_p;

	current_node = bullet_list.head;

	// Iterate through the list of bullets
	while (current_node) {

		bullet_p = (bullet_t *)(current_node->data_p);

		// Erase the previous frame's image of the bullet
		erase_bullet(bullet_p);

		// Update the position of the bullet
		update_bullet(bullet_p);

		// Re-draw if within bounds of screen, otherwise delete
		if (OUT_OF_BOUNDS(bullet_p)) {
			delete_bullet(current_node);
		} else {
			draw_bullet(bullet_p);
		}

		current_node = current_node->next;
	}
}
unsigned short get_num_bullets() {
	return bullet_list.size;
}
linked_list_t *get_bullet_list(void) {
	return &bullet_list;
}