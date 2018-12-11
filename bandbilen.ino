#include <cmsis_os.h>
#include <croutine.h>
#include <event_groups.h>
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOSConfig_Default.h>
#include <list.h>
#include <mpu_prototypes.h>
#include <portmacro.h>
#include <queue.h>
#include <semphr.h>
#include <StackMacros.h>
#include <STM32FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include "bandbilen.h"


const int leftForward = 2;
const int leftBackward = 3;
const int rightForward = 4;
const int rightBackward = 5;
const int greenPin = 10;
const int redPin = 12;
const int yellowPin = 15;
int i = 0;

//front floor
//vcc	trig	echo	gnd
const int trigPinFrontFloor = 6;
const int echoPinFrontFloor = 7;
//wall
//gnd	echo	trig	vcc
const int trigPinWall = 8;
const int echoPinWall = 9;

//back floor
const int trigPinBackFloor = 13;	
const int echoPinBackFloor = 14;  


long duration, frontFloorDistance, backFloorDistance,  wallDistance, previousRead = 50;


enum 
{
	SEARCH, DRIVE, DERBY, BACKOFF, MAKESURE
} state = SEARCH;

void forward()
{
    digitalWrite (leftBackward, 0);
    digitalWrite (rightBackward, 0);
    digitalWrite (leftForward, 1);
    digitalWrite (rightForward, 1);
}

void backward()
{
    digitalWrite (leftForward, 0);
    digitalWrite (rightForward, 0);
    digitalWrite (leftBackward, 1);
    digitalWrite (rightBackward, 1);
 }

void left()
{
    digitalWrite(rightBackward, 0);
    digitalWrite(leftForward, 0);
    digitalWrite(leftBackward, 1);
    digitalWrite(rightForward, 1);
}

void right()
{
    digitalWrite(leftBackward, 0);
    digitalWrite(rightForward, 0);
    digitalWrite(rightBackward, 1);
    digitalWrite(leftForward, 1);
}

void stopit()
{
    digitalWrite(leftForward, 0);
    digitalWrite(rightForward, 0);
    digitalWrite(leftBackward, 0);
    digitalWrite(rightBackward, 0);
}

void setup()
{
	//Nollställer allt
	digitalWrite (redPin, 0);
	digitalWrite (yellowPin, 0);
	digitalWrite (greenPin, 0);
	digitalWrite(leftForward, 0);
    digitalWrite(rightForward, 0);
    digitalWrite(leftBackward, 0);
    digitalWrite(rightBackward, 0);
	digitalWrite(trigPinFrontFloor, 0);
	digitalWrite(echoPinFrontFloor, 0);
	digitalWrite(trigPinWall, 0);
	digitalWrite(echoPinWall, 0);
	digitalWrite(trigPinBackFloor, 0);
	digitalWrite(echoPinBackFloor, 0);
	
 
 frontFloorDistance = 5;
 backFloorDistance = 5;
 pinMode(leftForward , OUTPUT);
 pinMode(leftBackward , OUTPUT);
 pinMode(rightForward , OUTPUT);
 pinMode(rightBackward , OUTPUT);
 
 pinMode(greenPin, OUTPUT);
 pinMode(yellowPin, OUTPUT);
 pinMode(redPin, OUTPUT);
 
 pinMode(trigPinFrontFloor, OUTPUT); // Sets the trigPin as an Output
 pinMode(echoPinFrontFloor, INPUT); // Sets the echoPin as an Input
 pinMode(trigPinWall, OUTPUT); // Sets the trigPin as an Output
 pinMode(echoPinWall, INPUT); // Sets the echoPin as an Input
 pinMode(trigPinBackFloor, OUTPUT); // Sets the trigPin as an Output
 pinMode(echoPinBackFloor, INPUT); // Sets the echoPin as an Input
 pinMode(11, INPUT);
 
 Serial.begin(9600); // Starts the serial communication

 
 stopit();
 
 //Create new task
 xTaskCreate(v_state,"State", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY +10, NULL);
 xTaskCreate(v_senseFrontFloor,"Sensor front floor", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY +2, NULL);
 xTaskCreate(v_senseBackFloor,"Sensor back floor", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY +2, NULL);
 xTaskCreate(v_senseWall,"Sensor wall", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY +1, NULL);

 vTaskStartScheduler();
 
 // Serial.println("Out of memory");
}


void v_senseFrontFloor( void * pvParameters)
{
    while(1)
    {   
        // The sensor is triggered by a 1 pulse of 10 or more microseconds.
        // Give a short 0 pulse beforehand to ensure a clean 1 pulse:
        digitalWrite(trigPinFrontFloor, 0);
        vTaskDelay( pdMS_TO_TICKS(5) );
        digitalWrite(trigPinFrontFloor, 1);
        vTaskDelay( pdMS_TO_TICKS(10) );
        digitalWrite(trigPinFrontFloor, 0);
 
        // Read the signal from the sensor: a 1 pulse whose
        // duration is the time (in microseconds) from the sending
        // of the ping to the reception of its echo off of an object.
        pinMode(echoPinFrontFloor, INPUT);
        duration = pulseIn(echoPinFrontFloor, 1);
 
        // Convert the time into a distance
        frontFloorDistance = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
		
		Serial.print("Front Floor: ");
        Serial.print(frontFloorDistance);
        Serial.print("cm");
        Serial.println();
		
		
		if(frontFloorDistance > 15)
		{
			i=0;
			state = BACKOFF;
		}
		
        vTaskDelay( pdMS_TO_TICKS(200) );
    }
}

void v_senseBackFloor( void * pvParameters)
{
    while(1)
    {   
        // The sensor is triggered by a 1 pulse of 10 or more microseconds.
        // Give a short 0 pulse beforehand to ensure a clean 1 pulse:
        digitalWrite(trigPinBackFloor, 0);
        vTaskDelay( pdMS_TO_TICKS(5) );
        digitalWrite(trigPinBackFloor, 1);
        vTaskDelay( pdMS_TO_TICKS(10) );
        digitalWrite(trigPinBackFloor, 0);
 
        // Read the signal from the sensor: a 1 pulse whose
        // duration is the time (in microseconds) from the sending
        // of the ping to the reception of its echo off of an object.
        pinMode(echoPinBackFloor, INPUT);
        duration = pulseIn(echoPinBackFloor, 1);
 
        // Convert the time into a distance
        backFloorDistance = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
		
		Serial.print("Back Floor: ");
        Serial.print(backFloorDistance);
        Serial.print("cm");
        Serial.println();
		
		
		if(backFloorDistance > 15)
		{
			i=0;
			state = DRIVE;
		}
		
        vTaskDelay( pdMS_TO_TICKS(200) );
    }
}

void v_senseWall( void * pvParameters)
{
    while(1)
    {   
		if(state == SEARCH)
		{
			// The sensor is triggered by a 1 pulse of 10 or more microseconds.
			// Give a short 0 pulse beforehand to ensure a clean 1 pulse:
			digitalWrite(trigPinWall, 0);
			vTaskDelay( pdMS_TO_TICKS(5) );
			digitalWrite(trigPinWall, 1);
			vTaskDelay( pdMS_TO_TICKS(10) );
			digitalWrite(trigPinWall, 0);
	 
			// Read the signal from the sensor: a 1 pulse whose
			// duration is the time (in microseconds) from the sending
			// of the ping to the reception of its echo off of an object.
			pinMode(echoPinWall, INPUT);
			duration = pulseIn(echoPinWall, 1);
	 
			// Convert the time into a distance
			wallDistance = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
			
			if( wallDistance > 50 && wallDistance > 4)
			{
				wallDistance = 50;
			}
			Serial.print("Wall: ");
			Serial.print(wallDistance);
			Serial.print("cm");
			Serial.println();
			
			if(wallDistance < 50 && wallDistance > 4)		{
				i=0;
				state = DRIVE;
			}
			previousRead = wallDistance;
			
			// if(i == 10)
			// {
				// int temp = 0;
				// for(int j = 0; j < 10; j++)
				// {
					// temp += sensorVars[i];
				// }
				// temp = temp / 10;
				// temp = wallDistance - temp;
				// if(temp > 5 && wallDistance != 50)
				// {
					// state = DRIVE;
				// }
				// i = 0;
			// }
		}
        vTaskDelay( pdMS_TO_TICKS(250) );
    }
}

void v_state( void* pvParameters)
{
	while(1)
	{
		digitalWrite(greenPin, 0);
		digitalWrite(yellowPin, 0);
		digitalWrite(redPin, 0); 
		switch(state)
		{	
			case SEARCH:
				digitalWrite(yellowPin, 1);
        Serial.println("SEARCHING FOR BALL");
        right();
				if(i >= 50)
				{
					i = 0;
					state = DRIVE;
				}
				break;
			case DRIVE:
				digitalWrite(greenPin, 1);
				Serial.println("DRIVING FORWARD");
				forward();
				if(i >= 20)
				{
					i = 0;
					state = SEARCH;
				}
				break;
			case DERBY:
				digitalWrite(greenPin, 1);
				Serial.println("DERBY!");
				forward();
				break;
			case BACKOFF:
				digitalWrite(redPin, 1);
				Serial.println("BACKING OFF");
				backward();
				if(i >= 20)
				{
					i = 0;
					state = SEARCH;
				}
				break;
			case MAKESURE:
				digitalWrite(yellowPin, 1);
				Serial.println("chill");
				stopit();
				if(i >= 50)
				{
					i = 0;
					state = DRIVE;
				}
				break;
			default:
				break;
		}
		i++;
		vTaskDelay( pdMS_TO_TICKS(125) );
	}
}   

void loop()
{
}
