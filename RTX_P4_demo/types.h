#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define FOREVER 1

typedef signed short pixel_t;
typedef signed short speed_t;

typedef enum collision_type {
	SHIP_AND_ASTEROID, BULLET_AND_ASTEROID
} collision_type_t;

typedef enum direction {
	UP, DOWN, LEFT, RIGHT
} direction_t;

typedef enum object_type {
	SHIP, ASTEROID, BULLET
} object_type_t;

typedef struct moveable {
	pixel_t x_position;
	pixel_t y_position;
	speed_t x_speed;
	speed_t y_speed;
} moveable_t;

typedef moveable_t ship_t;
typedef moveable_t bullet_t;
typedef moveable_t asteroid_t;

typedef struct list_node {
	struct list_node *prev;
	struct list_node *next;
	void *data_p;
} list_node_t;

typedef struct linked_list {
	list_node_t *head;
	size_t size;
} linked_list_t;

#endif