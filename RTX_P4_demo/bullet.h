#ifndef __BULLET_H__
#define __BULLET_H__

#include "types.h"

void shoot(ship_t* ship_p);
void update_bullets_in_frame(void);
unsigned short get_num_bullets(void);
linked_list_t *get_bullet_list(void);
#endif