#include "types.h"

/**
 * INTERFACE FUNCTIONS (ACCESSIBLE BY MAIN.C)
 */
void update_asteroids_in_frame(void);
void spawn_asteroid(void);
unsigned short get_num_asteroids(void);
linked_list_t *get_asteroid_list(void);