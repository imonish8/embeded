/* ****************************************************************
 * Project Name : 
 * Description  : Program to implement simple schedular
 * Author       : 
 * Date         :           
 * *************************************************************** */

#include "includes.h"

/* ============= uCOS-II parameters ============*/
OS_STK TestTask1Stack[200];     //Stack declaration for Task 1
void TestTask1(void *pdata);    //TestTask1() declaration

OS_STK TestTask2Stack[200];    //Stack declaration for Task 2
void TestTask2(void *pdata);   //TestTask2() declaration

OS_STK TestTask3Stack[200];    //Stack declaration for Task 3
void TestTask3(void *pdata);   //TestTask2() declaration

void ledon(int );
void ledon1(int );
void ledon2(int );
void ledoff(int );
void ledoff1(int );
void ledoff2(int );
void ledinit(void);
/* ================== main() ======================*/
int  main (void)
{
   	/* Function called to initialise the LEDs*/
   	ledinit();
   	
 	/* Function called to initialise the OS*/
 	OSInit();
 	
 	/* Function called to create TASK 1*/
	OSTaskCreate(TestTask1, (void *)0, &TestTask1Stack[199], 0);
	OSTaskCreate(TestTask3, (void *)0, &TestTask3Stack[199], 2);	
    /* Function called to start multitasking*/
    OSStart();
    
	return 0;
}

/*==================== Task A =====================*/
       /*( LED[6:7] Task: blink ascending ) */
/*=================================================*/
void TestTask1(void *pdata)
{
	int led = 0;
   	/* Function called to initialise the timer */
	timer_init();
	/* Function called to create TASK 2*/
	OSTaskCreate(TestTask2, (void *)0, &TestTask2Stack[199], 1);
	
	while(1) 
	{
	 	ledon(led);
	 	OSTimeDlyHMSM(0, 0, 3, 0); //Generate delay of 1 sec
		ledoff(led);
		led++;
		if(led==2)
		{
			led = 0;
		}
	}
}

/*==================== Task B =====================*/
       /*( LED[8&9] Task: blink ascending ) */
/*=================================================*/
void TestTask2(void *pdata)
{
	int led1 =0;
	

	while(1) 
	{
	 	ledon1(led1);
	 	OSTimeDlyHMSM(0, 0, 2, 0); //Generate delay of 250ms
		ledoff1(led1);
		
		led1++;
		if(led1==2)
		{
			led1 = 0;
		}
	}
}
/*==================== Task C =====================*/
       /*( LED 10 Task: blink  ) */
/*=================================================*/
void TestTask3(void *pdata)
{
	int led2 =0;

	while(1) 
	{
	 	ledon2(led2);
	 	OSTimeDlyHMSM(0, 0, 0, 500); //Generate delay of 250ms
		ledoff2(led2);
		OSTimeDlyHMSM(0, 0, 0, 500); //Generate delay of 250ms
	}
}


void ledon(int led)
{
	IOSET0 = 1<<(led+6);
}
void ledon1(int led)
{
	IOSET0 = 1<<(led+8);
}
void ledon2(int led)
{
	IOSET0 = 1<<(led+10);
}
void ledoff(int led)
{
	IOCLR0 = 1<<(led+6);
}
void ledoff1(int led)
{
	IOCLR0 = 1<<(led+8);
}
void ledoff2(int led)
{
	IOCLR0 = 1<<(led+10);
}


void ledinit(void)
{
	IODIR0= 0x0000FFF0;
	IOCLR0= 0x0000FFF0;
}