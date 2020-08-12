/*
 * rtos-Task-EX-LCD.c
 *
 * Created: 4/11/2020 6:42:37 AM
 * Author : mosad
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include "lcd_drive.h"
#include <util/delay.h>
#include <avr/interrupt.h>


//global variables 
char *menue[6] = {"Task 1" ,
				  "Task 2" ,
				  "Task 3" ,
				  "Task 4" ,
				  "Task 5"  };
char i = 0 ; 
char pressed_button = 0;
                        
// tasks prototypes 
void system_initialize(void *p);
void check_button(void *p);
void take_action(void *p);

// task handelrs 
TaskHandle_t buttons = NULL ;
TaskHandle_t action = NULL ;

int main(void)
{
    xTaskCreate(system_initialize , NULL , 85 , NULL , 3 , NULL);
	xTaskCreate(check_button , NULL , 85 , NULL , 2 , &buttons);
	xTaskCreate(take_action , NULL , 85 , NULL , 1 , &action);
	vTaskStartScheduler();
    while (1) 
    {
    }
}
/************************************************************************************************************
* Description : initialize the system will show "hello rtos" for one second then display the first two 
                items in the menue
* Parameters  : a void pointer
* Return type : void
************************************************************************************************************/
void system_initialize(void *p){
	DDRA |= (31 << 0); //seting pins as output for leds
	Lcd_Init();
	_delay_ms(200);
	Lcd_Out(1 , 1 , "Hello RTOS");
	_delay_ms(1000);
	send_command(_LCD_CLEAR);
	_delay_ms(10);
	Lcd_Chr(1 , 1 , '<');
	Lcd_Out(1 , 2 , menue[0]);
	Lcd_Chr(1 , 8 , '>');
	Lcd_Out(2 , 1 , menue[1]);
	
	vTaskDelete(NULL);
	
	}

/************************************************************************************************************
* Description : check if a button with pressed and if it is pressed will lower its priority letting task 
                take_action take control 
* Parameters  : void pointer
* Return type : void
************************************************************************************************************/	
void check_button(void *p){
	
	DDRD &= ~ (7 << 0); //seting from d0 to d2 as input
	while(1){
		// will stay here until a button is pressed
		while(1){
			if ( !(PIND & (1 << 0)) ) {pressed_button = 'f' ; break ;}
			else if ( !(PIND & (1 << 1)) ) {pressed_button = 'k' ; break ;}
			else if ( !(PIND & (1 << 2)) ) {pressed_button = 'b' ; break ;}
			else {}			
		}
		// giving control to task take action
		vTaskPrioritySet(buttons , 1);
		vTaskPrioritySet(action , 2);
	}
}

/************************************************************************************************************
* Description : take action based on the pressed button to move through the menue or choose a task which is 
                a led will be on
* Parameters  : void pointer
* Return type : void
************************************************************************************************************/
void take_action(void *p){
	
		while(1){
			_delay_ms(300);
			if( (i < 5 ) && (i >= 0) ){
				if(pressed_button == 'f'){
					i++;
					Lcd_Out(1 , 2 , menue[i]);
					Lcd_Out(2 , 1 , menue[i+1]);
					if(i == 5 ){i--;}
				}
				else if(pressed_button == 'b'){
					i--;
					Lcd_Out(1 , 2 , menue[i]);
					Lcd_Out(2 , 1 , menue[i+1]);
				}
				else if(pressed_button == 'k'){
					PORTA |= (1 << i);
				}
			}
			// giving control back to task check buttons
			vTaskPrioritySet(buttons , 2);
			vTaskPrioritySet(action , 1);
		}
		
}	

	

ISR (INT0_vect){
	
}