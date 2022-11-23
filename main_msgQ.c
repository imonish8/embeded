/* *************************************************************
 * Project Name : 
 * Description  : Program to implement message queue
 * Author       : 
 * Date         :           
 * **************************************************************/
#include "includes.h"

#define TASK_STACK_SIZE 512
#define MSG_QUEUE_SIZE 2

/*====== led() declarations =====*/
void ledon(int );
void ledon1(int );
void ledoff(int );
void ledoff1(int );
void ledinit(void);
/*========== UCOS_II parameter Declaration ==========*/
OS_STK task_a_stack[TASK_STACK_SIZE];
OS_STK task_b_stack[TASK_STACK_SIZE];
void task_a(void *pdata);
void task_b(void *pdata);
OS_EVENT *MsgQ;
void *MsgQTbl[MSG_QUEUE_SIZE];

/*====== Main() =========*/
int  main (void)
{
    /* Function called to initialise the LEDs*/
    ledinit();
    /* Function called to initialise the OS*/
    OSInit();
    /* Msg Queue Create */
    MsgQ=OSQCreate(&MsgQTbl[0],MSG_QUEUE_SIZE);
	/* Create tasks  */
	OSTaskCreate(task_a, (void *)0, &task_a_stack[TASK_STACK_SIZE-1], 0);
	OSTaskCreate(task_b, (void *)0, &task_b_stack[TASK_STACK_SIZE-1], 1);
	/* Function called to start multitasking*/
    OSStart();
	return 0;
}

/*=============== Task A  =============== */
void task_a(void *pdata)
{
	unsigned int i,temp[2]={0,1};
	
	/* Initialize The Timer */
	timer_init();
	IOCLR0 = 0x00FFFF0;
  	while(1) 
 	{
	 	for(i=0;i<2;i++)
	 	{
	 		ledoff(temp[i]);
		 	OSTimeDly(500);
	 		ledon(temp[i]); 	
 		 	OSQPost(MsgQ,(void*)&temp[i]);
			OSTimeDlyHMSM(0,0,1,0);
	   }	
	}
}

/* =============== Task B  =============== */
void task_b(void *pdata)
{
	unsigned int i,*rtemp=0;
	while(1) 
	{		
		/*Pend (wait) For The Created Msg Queue */
		rtemp=OSQAccept(MsgQ);
		/*perform The Respective task Using The Receieved Message*/
	 	ledoff1(*rtemp);
		OSTimeDly(25);
	 	ledon1(*rtemp);
	}
}
void ledon(int led)
{
	IOSET0 = 1<<(led+5);
	
}
void ledon1(int led)
{
	IOSET0 = 1<<(led+8);
	
}

void ledoff(int led)
{
	IOCLR0 = 1<<(led+5);
	
}
void ledoff1(int led)
{
	IOCLR0 = 1<<(led+8);
}
void ledinit(void)
{
	IODIR0= 0x0000FFF0;
	IOCLR0= 0xffffffff;
}
