/*
 * rtos-semaphore-synch-tasks.c
 *
 * Created: 4/17/2020 3:21:22 AM
 * Author : mosad
 */ 


#define F_CPU 1000000
#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include "semphr.h"
#include <util/delay.h>
#include <avr/interrupt.h>


//tasks handle
TaskHandle_t led1 = NULL ;
TaskHandle_t led2 = NULL ;
TaskHandle_t led3 = NULL ;
SemaphoreHandle_t xsemaphr = NULL ;
SemaphoreHandle_t xsemaphr2 =NULL ;
SemaphoreHandle_t xsemaphr3 =NULL ;

//tasks prototypes
void TASK3( void *p);
void TASK2( void *p);
void TASK( void *p);



int main(void)
{
	DDRD = 255 ; // seting all portd as output
	xsemaphr = xSemaphoreCreateBinary();
	xsemaphr2 = xSemaphoreCreateBinary();
	xsemaphr3 = xSemaphoreCreateBinary();
	xSemaphoreGive(xsemaphr3);
	xTaskCreate(TASK ,NULL ,80 , NULL , 1 , &led1);  //create task 1
	xTaskCreate(TASK2 ,NULL ,80 , NULL , 2 , &led2); //create task 2
	xTaskCreate(TASK3 ,NULL ,80 , NULL , 3 , &led3); //create task 3
	vTaskStartScheduler(); // start scheduler
	
	/* If all is well then main() will never reach here as the scheduler will
	now be running the tasks. If main() does reach here then it is likely that
	there was insufficient heap memory available for the idle task to be created.
	Chapter 2 provides more information on heap memory management. */
	while(1){}
}

void TASK3( void *p){
	for (;;)
	{
		vTaskDelay(2);
		xSemaphoreTake(xsemaphr2 , portMAX_DELAY);
		PORTD |= ( 1 << 2);
		vTaskDelay(1000);
		PORTD &=~(1 << 2);
		xSemaphoreGive(xsemaphr3);
	}
}

void TASK2( void *p){
	for (;;)
	{
		xSemaphoreTake(xsemaphr , portMAX_DELAY);
		PORTD |= ( 1 << 1);
		vTaskDelay(1000);
		PORTD &=~(1 << 1);
		xSemaphoreGive(xsemaphr2);
	}
}

void TASK( void *p){
	for (;;)
	{
		xSemaphoreTake(xsemaphr3 , portMAX_DELAY);
		PORTD |= ( 1 << 0);
		vTaskDelay(1000);
		PORTD &=~(1 << 0);
		xSemaphoreGive(xsemaphr);
	}
}
