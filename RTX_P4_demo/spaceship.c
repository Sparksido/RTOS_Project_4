#include "spaceship.h"
#include "bitmaps.h"
#include "GLCD.h"

#define BITMAP_ARGS(x, y, image) x, y, image.width, image.height, (unsigned char*)image.pixel_data
#define GET_OFFSET(length) (length/2)

void draw_ship(ship_t ship) {

	short plot_x, plot_y;

	plot_x = ship.x_position - GET_OFFSET(spaceship_0.width);
	plot_y = ship.y_position - GET_OFFSET(spaceship_0.height);

	GLCD_Bitmap(BITMAP_ARGS(plot_x, plot_y, spaceship_0));
}

void set_position(ship_t* ship_p, pixel_t x, pixel_t y){
	ship_p->x_position = x;
	ship_p->y_position = y;
}
void update_position(ship_t* ship_p) {
	ship_p->x_position += ship_p->x_speed;
	ship_p->y_position += ship_p->y_speed;
}

void set_speed(ship_t* ship_p, speed_t x, speed_t y) {
	ship_p->x_speed = x;
	ship_p->y_speed = y;
}

// void accelerate_ship(ship_t* ship_p, int acceleration) {
// 	ship_p->x_speed += acceleration;
// 	ship_p->y_speed += acceleration;
// }

// void spawn_ship(pixel_t x, pixel_t y);
// void set_ship_position(pixel_t x, pixel_t y);
// void rotate_ship(int degrees);
// void accelerate_ship(unsigned int acceleration);
// void destroy_ship(void);
//   
