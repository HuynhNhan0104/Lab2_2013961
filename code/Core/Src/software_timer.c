/*
 * software_timer.c
 *
 *  Created on: Oct 4, 2022
 *      Author: NHAN
 */

#include "software_timer.h"

// Thiet lap thoi gian sang cua 2 den led
int counter_2_LED_RED = 1000;
int timer_2_LED_RED_flag = 0;
void setTimer_2_LED_RED(int duration){
	counter_2_LED_RED = duration/TIMER_CYCLE;
	timer_2_LED_RED_flag = 0;
}

// bien dem thoi gian hien thi cua 1 led 7 doan
int counter_led_7_seg = 500;
int timer_led_7_seg_flag = 0;
void setTimer_led_7_seg(int duration){
	counter_led_7_seg = duration/TIMER_CYCLE;
	timer_led_7_seg_flag = 0;
}


// bien dem thoi gian  cua dong ho so
int counter_clock = 1000;
int timer_clock_flag = 0;
void setTimer_clock(int duration){
	counter_clock = duration/TIMER_CYCLE;
	timer_clock_flag = 0;
}

//matrix_led

int counter_matrix_led = 500;
int timer_matrix_led_flag = 0;

void setTimer_matrix_led(int duration){
	counter_matrix_led = duration/TIMER_CYCLE;
	timer_matrix_led_flag = 0;
}



//Timer_run

void timer_run(){
	if(counter_2_LED_RED > 0){
		counter_2_LED_RED--;
		if(counter_2_LED_RED <= 0) timer_2_LED_RED_flag = 1;
	}


	if(counter_led_7_seg > 0){
			counter_led_7_seg--;
			if(counter_led_7_seg <= 0) timer_led_7_seg_flag = 1;
	}


	if(counter_clock > 0){
		counter_clock--;
		if(counter_clock <= 0) timer_clock_flag = 1;
	}
	if(counter_matrix_led > 0){
		counter_matrix_led--;
		if(counter_matrix_led <= 0 ){
			timer_matrix_led_flag = 1;
		}
	}
}

