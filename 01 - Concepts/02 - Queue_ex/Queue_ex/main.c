/*
 * rtos-blink.c
 *
 * Created: 17/7/2020 7:53:33 AM
 * Author : mosad
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include <util/delay.h>
#include <avr/interrupt.h>



xQueueHandle dataQueue ;

void TASK3( void *p){
	for (;;)
	{
	
		
	}
}

void taskRecieve( void *p){
	unsigned char num = 0 ;
	for (;;)
	{
		xQueueReceive (dataQueue , &num , 2000);
		PORTD = num ;
	}
}

void taskSend( void *p){
	unsigned char num = 0 ;
	for (;;)
	{
		num++;
		vTaskDelay(1000);
		xQueueSendToBack (dataQueue , &num , 0);	
	}
}



int main(void)
{
	/* Replace with your application code */
	DDRD = 255 ;
	dataQueue =xQueueCreate(5 , sizeof(char));
	xTaskCreate(taskSend ,NULL ,80 , NULL , 1 , NULL);
	xTaskCreate(taskRecieve ,NULL ,80 , NULL , 2 , NULL);
	vTaskStartScheduler();
	while(1){}
}
