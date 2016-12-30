#include <cmath>
#include "collisions.h"
#include "bullet.h"
#include "asteroid.h"

#define SHIP_ASTEROID_COLLISION_DIST 39
#define BULLET_ASTEROID_COLLISION_DIST 20

#define X_DISTANCE(obj1, obj2) (obj2->x_position - obj1->x_position)
#define Y_DISTANCE(obj1, obj2) (obj2->y_position - obj1->y_position)

#define DISTANCE_BETWEEN(obj1, obj2) (hypot(X_DISTANCE(obj1, obj2), Y_DISTANCE(obj1, obj2)))

unsigned char COLLISION_DETECTED;

void handle_collisions(ship_t* ship_p, linked_list_t *bullet_list, linked_list_t *asteroid_list) {

	list_node_t *asteroid_node, *bullet_node;

	asteroid_t *asteroid_p;
	bullet_t *bullet_p;

	// CHECK FOR COLLISIONS BETWEEN THE SHIP AND ASTEROIDS
	asteroid_node = asteroid_list->head;
	COLLISION_DETECTED = 0;
	while (asteroid_node) 
	{
		// Obtain the current asteroid
		asteroid_p = (asteroid_t*)(asteroid_node->data_p);

		if (DISTANCE_BETWEEN(asteroid_p, ship_p) < SHIP_ASTEROID_COLLISION_DIST) 
		{
			// asteroid ship collision
			COLLISION_DETECTED = 1;
		}

		bullet_node = bullet_list->head;

		while (bullet_node) 
		{
			if (DISTANCE_BETWEEN(asteroid_p, bullet_p) < BULLET_ASTEROID_COLLISION_DIST) 
			{
				COLLISION_DETECTED = 1;
				delete_bullet(bullet_node);
				delete_asteroid(asteroid_node);
			}
			bullet_node = bullet_node->next;
		}
		asteroid_node = asteroid_node->next;
	}

	// CHECK FOR COLLISIONS BETWEEN BULLETS AND ASTEROIDS
}

unsigned char is_collision_detected(void) {
	return COLLISION_DETECTED;
}