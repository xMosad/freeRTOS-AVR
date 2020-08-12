/*
 * rtos_mutex.c
 *
 * Created: 4/19/2020 2:46:14 PM
 * Author : mosad
 */ 

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include "semphr.h"
#include "lcd_drive.h"

/*                 prototypes                       */
void task1(void *p);
void task2(void *p);
void task3(void *p);

/*                 global variables                 */


/*                 handlers                       */
SemaphoreHandle_t smphr_lcd =NULL ;

int main(void)
{
	Lcd_Init();
    xTaskCreate(task1 , NULL , 85 , NULL , 1 , NULL);
	xTaskCreate(task2 , NULL , 85 , NULL , 2 , NULL);
	xTaskCreate(task3 , NULL , 85 , NULL , 3 , NULL);
	smphr_lcd = xSemaphoreCreateMutex();
	xSemaphoreGive(smphr_lcd);
	
	vTaskStartScheduler();
    while (1) 
    {
    }
}

void task1(void *p){
	
	while(1) {
		xSemaphoreTake(smphr_lcd ,portMAX_DELAY);
		send_command(_LCD_CLEAR);
		_delay_ms(10);
		Lcd_Out(1 , 1 , "lowest priority task");
		_delay_ms(300);
		xSemaphoreGive(smphr_lcd);	
	}
	
}

void task2(void *p){
	DDRD |= (1 << 0);
	while(1) {
		vTaskDelay(200);
		PORTD |= (1 << 0);
		/*_delay_ms(1000);
		PORTD &= ~(1 << 0);
		_delay_ms(1000);*/
		vTaskDelete(NULL);	
	}
	
}

void task3(void *p){
	
	while(1) {
		vTaskDelay(10);
		xSemaphoreTake(smphr_lcd ,portMAX_DELAY);
		send_command(_LCD_CLEAR);
		_delay_ms(10);
		Lcd_Out(1 , 1 , "highest priority task");
		_delay_ms(1000);
		xSemaphoreGive(smphr_lcd);	
	}
	
}

