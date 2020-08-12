/*
 * Created: 7/22/2020 6:33:27 AM
 * Author : mosad
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include <util/delay.h>
#include <avr/interrupt.h>

xTaskHandle t1 = NULL ;


void task1( void *p ){
	uint32_t val ;
	while(1){
		xTaskNotifyWait(0 , ((1 << 0) | (1 << 1)) ,&val , portMAX_DELAY );
		if (val == 1 ){ PORTD ^= (1 << 0);}
	}	
}

void task2( void *p ){
	uint8_t val = 0 ;
	while(1){
		val++;
		xTaskNotify(t1 , (1 << 0) , eSetBits);
		vTaskDelay(200);
	}
}

void task3( void *p ){
	while(1){
		xTaskNotify(t1 , (1 << 1) , eSetBits);
		vTaskDelay(1000);
		
	}
}

int main(void)
{
	DDRD = 255 ;
	DDRC =1;
	//GICR |= (1 << 6);
	xTaskCreate(task1 , NULL , 80 , NULL , 1 , &t1);
	xTaskCreate(task2 , NULL , 80 , NULL , 1 , NULL);
	xTaskCreate(task3 , NULL , 80 , NULL , 1 , NULL);
	vTaskStartScheduler();
    while (1) 
    {
    }
}
ISR (INT0_vect){
}




