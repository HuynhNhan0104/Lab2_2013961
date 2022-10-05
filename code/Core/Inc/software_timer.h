/*
 * software_timer.h
 *
 *  Created on: Oct 4, 2022
 *      Author: NHAN
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

extern int TIMER_CYCLE;



// bien diem thoi gian sang 2 den led
extern int counter_2_LED_RED;
extern int timer_2_LED_RED_flag;
void setTimer_2_LED_RED(int duration);

// bien dem thoi gian hien thi cua 1 led 7 doan
extern int counter_led_7_seg;
extern int timer_led_7_seg_flag;
void setTimer_led_7_seg(int duration);

// bien dem thoi gian  cua dong ho so
extern int counter_clock;
extern int timer_clock_flag;
void setTimer_clock(int duration);



// tao bien dem cho matrix_led
extern int counter_matrix_led;
extern int timer_matrix_led_flag;

void setTimer_matrix_led(int duration);

//timer run
void  timer_run ();




#endif /* INC_SOFTWARE_TIMER_H_ */
