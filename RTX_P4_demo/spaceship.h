#include "types.h"

void draw_ship(ship_t);

void set_position(ship_t* ship_p, pixel_t x, pixel_t y);
void set_speed(ship_t* ship_p, speed_t x, speed_t y);
void update_ship_position(ship_t* ship_p);

void spawn_ship(pixel_t x, pixel_t y);
void rotate_ship(int degrees);
void accelerate_ship(ship_t* ship_p, unsigned int acceleration);
void destroy_ship(void);


  