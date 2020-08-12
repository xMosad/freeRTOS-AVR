/*
 * EventGroup_test.c
 *
 * Created: 7/19/2020 4:33:27 AM
 * Author : mosad
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include "FreeRTOS.h"
#include "event_groups.h"
#include "task.h"
#include "list.h"
#include <util/delay.h>
#include <avr/interrupt.h>

// defines
#define FIRST_BIT_CHECK (1 << 0)
#define SECOND_BIT_CHECK (1 << 1)
EventGroupHandle_t event0 ;

void taskEventTrig( void *p ){
	while(1){
		xEventGroupSetBits (event0 ,FIRST_BIT_CHECK );
		vTaskDelay(200);
		xEventGroupSetBits(event0 , SECOND_BIT_CHECK);
		vTaskDelay(200);
	}	
}

void checkEvent( void *p ){
	EventBits_t xEventGroupValue;
	const EventBits_t xBitsToWaitFor = (FIRST_BIT_CHECK | SECOND_BIT_CHECK);
	while(1){
		xEventGroupValue = xEventGroupWaitBits(event0 , xBitsToWaitFor ,pdTRUE ,pdFALSE,portMAX_DELAY );
		if (xEventGroupValue & FIRST_BIT_CHECK){
			PORTD ^= (1 << 0);
		}
	    if (xEventGroupValue & SECOND_BIT_CHECK){
			PORTD ^= (1 << 1);
		}
	}
}

int main(void)
{
	DDRD = 3 ;
	event0 = xEventGroupCreate();
	xTaskCreate(taskEventTrig , NULL , 80 , NULL , 1 , NULL);
	xTaskCreate(checkEvent , NULL , 80 , NULL , 2 , NULL);
	vTaskStartScheduler();
    while (1) 
    {
    }
}


