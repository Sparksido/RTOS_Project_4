// #include "bitmaps.h"
#include <RTL.h>
#include "LPC17xx.H"                    /* LPC17xx definitions               */
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "ADC.h"
#include "spaceship.h"
#include "types.h"
#include "bullet.h"
#include "asteroid.h"
#include "collisions.h"
#include <stdio.h>

#define __FI        1                   /* Font index 16x24                  */

OS_TID t_led;                           /* assigned task id of task: led */
OS_TID t_adc;                           /* assigned task id of task: adc */
OS_TID t_kbd;                           /* assigned task id of task: keyread */
OS_TID t_jst   ;                        /* assigned task id of task: joystick */
OS_TID t_clock;                         /* assigned task id of task: clock   */
OS_TID t_lcd;                           /* assigned task id of task: lcd     */
OS_TID bullet_task;                     /* assigned task id of task: bullet_manager */

OS_MUT mut_GLCD;                        /* Mutex to controll GLCD access     */

unsigned int ADCStat = 0;
unsigned int ADCValue = 0;

#define MAX_LIVE_BULLETS 20
#define FPS 20
#define FRAME_DELAY 100 / FPS
ship_t ship;

/*----------------------------------------------------------------------------
  switch LED on
 *---------------------------------------------------------------------------*/
void LED_on  (unsigned char led) {
    LED_On (led); //turn on the physical LED
    os_mut_wait(mut_GLCD, 0xffff); //Rest of code updates graphics on the LCD
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Green);
    GLCD_DisplayChar(4, 5+led, __FI, 0x80+1); /* Circle Full                   */
    os_mut_release(mut_GLCD);
}

/*----------------------------------------------------------------------------
  switch LED off
 *---------------------------------------------------------------------------*/
void LED_off (unsigned char led) {
    LED_Off(led); //turn off the physical LED
    os_mut_wait(mut_GLCD, 0xffff); //Rest of code updates graphics on the LCD
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Green);
    GLCD_DisplayChar(4, 5+led, __FI, 0x80+0);  /* Circle Empty                 */
    os_mut_release(mut_GLCD);
}


/*----------------------------------------------------------------------------
  Task 1 'LEDs': Cycle through LEDs
 *---------------------------------------------------------------------------*/
__task void led (void) {
    int num_led = 3; //the number of LEDs to cycle through
    int on = 0;
    int off = 0;
    
    while(1)
    {
        for( on = 0; on <  num_led; on++)
        {
            
            off = on - 1; //Figure out which LED to turn off, wrap around if needed
            if(off == -1)
            { 
                off = num_led -1;
            }
            LED_off(off);            
            LED_on (on);
            os_dly_wait (100);                      /* delay 50 clock ticks             */
        }
    }
}

/*----------------------------------------------------------------------------
  Task 2 'keyread': process key stroke from int0 push button
 *---------------------------------------------------------------------------*/
__task void keyread (void) {

    const speed_t SPEED = 1;

    while (FOREVER) {              
                       /* endless loop                */
        if (INT0_Get() == 0) {                    /* if key pressed              */

            // Fire the shot 
            // shoot(&ship);
            spawn_asteroid();

            // Wait for button to be released
            while (INT0_Get() == 0) {}
        }
        os_dly_wait (5);                          /* wait for timeout: 5 ticks   */
    }
}

/*----------------------------------------------------------------------------
  Task 3 'ADC': Read potentiometer
 *---------------------------------------------------------------------------*/
/*NOTE: You need to complete this function*/
__task void adc (void) {

    unsigned char str[20];

    while (1) {
        
        ADC_start_conversion();
        ADC_update_value();

        sprintf(str, "ADC VALUE: %*i", 9, ADCValue);

        os_mut_wait(mut_GLCD, 0xffff);

        GLCD_SetBackColor(White);
        GLCD_SetTextColor(Red);
        GLCD_DisplayString(3, 0, __FI, str);   

        os_mut_release(mut_GLCD);

        os_dly_wait (10);
    }
}

/*----------------------------------------------------------------------------
  Task 4 'joystick': process an input from the joystick
 *---------------------------------------------------------------------------*/
/*NOTE: You need to complete this function*/
__task void joystick (void) {

    uint32_t joystick_value;
    const speed_t SPEED = 5;
    speed_t x_speed, y_speed;
    
    while (FOREVER) {

        // Collect joystick value
        joystick_value = KBD_Get();

        // Update horizontal speed
        if (IS_LEFT(joystick_value)) {
            x_speed = -SPEED;
        } else if (IS_RIGHT(joystick_value)) {
            x_speed = SPEED;
        } else {
            x_speed = 0;
        }

        // Update vertical speed
        if (IS_UP(joystick_value)) {
            y_speed = -SPEED;
        } else if (IS_DOWN(joystick_value)) {
            y_speed = SPEED;
        } else {
            y_speed = 0;
        }

        // Update position
        set_speed(&ship, x_speed, y_speed);
        update_position(&ship);

        // Re-draw ship
        os_mut_wait(mut_GLCD, 0xffff);
        draw_ship(ship);
        os_mut_release(mut_GLCD);

        // Delay
        os_dly_wait(FRAME_DELAY);
    }
}


/*----------------------------------------------------------------------------
  Task 5 'lcd': LCD Control task
 *---------------------------------------------------------------------------*/
__task void lcd (void) {

    for (;;) {
        os_mut_wait(mut_GLCD, 0xffff);
        GLCD_SetBackColor(Blue);
        GLCD_SetTextColor(White);
        GLCD_DisplayString(0, 0, __FI, "      MTE 241        ");
        GLCD_DisplayString(1, 0, __FI, "      RTX            ");
        GLCD_DisplayString(2, 0, __FI, "  Project 4 Demo   ");
        os_mut_release(mut_GLCD);
        os_dly_wait (400);

        os_mut_wait(mut_GLCD, 0xffff);
        GLCD_SetBackColor(Blue);
        GLCD_SetTextColor(Red);
        GLCD_DisplayString(0, 0, __FI, "      MTE 241        ");
        GLCD_DisplayString(1, 0, __FI, "      Other text     ");
        GLCD_DisplayString(2, 0, __FI, "    More text        ");
        os_mut_release(mut_GLCD);
        os_dly_wait (400);
    }
}

__task void moveables_manager(void) {

    while (FOREVER) {

        if (get_num_bullets()) {
            os_mut_wait(mut_GLCD, 0xffff);
            update_bullets_in_frame();
            os_mut_release(mut_GLCD);
        }

        if (get_num_asteroids()) {
            os_mut_wait(mut_GLCD, 0xffff);
            update_asteroids_in_frame();
            os_mut_release(mut_GLCD); 
        }

        os_dly_wait(FRAME_DELAY);
    }

}

__task collision_detection() {

    linked_list_t *bullet_list, *asteroid_list;

    bullet_list = get_bullet_list();
    asteroid_list = get_asteroid_list();

    while (FOREVER) {
        handle_collisions(&ship, bullet_list, asteroid_list);
        os_dly_wait(FRAME_DELAY);
    }
}

__task monitor() {
    unsigned char str[20];

    while (FOREVER) { 

        sprintf(str, "collision: %*i", 4, is_collision_detected());
        os_mut_wait(mut_GLCD, 0xffff);
        GLCD_DisplayString(0, 0, __FI, str);
        os_mut_release(mut_GLCD);
        os_dly_wait(FRAME_DELAY);
    }
}

void init_ship() {

    // Dynamically allocat
    set_position(&ship, 160, 120);
    set_speed(&ship, 0, 0);
    
    os_mut_wait(mut_GLCD, 0xffff);
    draw_ship(ship);
    os_mut_release(mut_GLCD);
}

/*----------------------------------------------------------------------------
  Task 6 'init': Initialize
 *---------------------------------------------------------------------------*/
/* NOTE: Add additional initialization calls for your tasks here */
__task void init (void) {

    // Initialize the mutex used for safely writing to the LCD
    os_mut_init(mut_GLCD);

    // Create and initialize the ship
    init_ship();

    // Start the joystick start to control the ship
    os_tsk_create(monitor, 0);
    os_tsk_create(joystick, 0);     /* start the joystick task           */
    os_tsk_create(keyread, 0);     /* start the kbd task           */
    // os_tsk_create(bullet_manager, 0);
    os_tsk_create(moveables_manager, 0);
    os_tsk_create(collision_detection, 0);
    // Delete this task once complete
    os_tsk_delete_self ();
}

/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {
    

    LED_Init ();                              /* Initialize the LEDs           */
    GLCD_Init();                              /* Initialize the GLCD           */
    KBD_Init ();                              /* initialize Push Button        */
    ADC_Init ();                                                            /* initialize the ADC            */

    // NVIC_EnableIRQ( ADC_IRQn );                             /* Enable ADC interrupt handler  */

    GLCD_Clear(Black);                        /* Clear the GLCD                */

    os_sys_init(init);                        /* Initialize RTX and start init */
}

    // t_led = os_tsk_create (led, 0);         /* start the led task               */
    // t_adc = os_tsk_create (adc, 0);         /* start the adc task               */
    // t_lcd = os_tsk_create (lcd, 0);     /* start task lcd                   */
    // os_tsk_create(test_joystick, 0);
    // os_tsk_create(test_);
    // os_tsk_create(test_draw_ship, 0);