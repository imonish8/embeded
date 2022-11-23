
/* ***********************************************************
 * Project Name : 
 * Description  : Program to implement (Task-to-task context switching)using mailbox
 * Author       : 
 * Date         :           
 * ******************************************************* */

#include "includes.h"

/*====== led() declarations =====*/
void ledon(int );
void ledon1(int );
void ledoff(int );
void ledoff1(int );
void ledinit(void);
/* ============= uCOS-II parameters ============*/
#define TASK_STACK_SIZE 512

OS_STK task_a_stack[TASK_STACK_SIZE];   //Stack declaration for Task a
void task_a(void *pdata);               //task_a() declaration   

OS_STK task_b_stack[TASK_STACK_SIZE];   //Stack declaration for Task b
void task_b(void *pdata);               //task_b() declaration  
  
OS_EVENT *AckMbox; //Global declaration of AckMbox variable                    
OS_EVENT *TxMbox;  //Global declaration of AckMbox variable


/* ================== main() ======================*/
int  main (void)
{
    unsigned int i;
    INT8U status;
    
   	ledinit();		/* Function called to initialise the LEDs*/
   	
 	OSInit();		/* Function called to initialise the OS*/
    
	AckMbox = OSMboxCreate((void *)0);/* Create Mailbox Ack parameter*/
	
	TxMbox = OSMboxCreate((void *)0);/* Create Mailbox Transmit parameter*/
		
	status =OSTaskCreate(task_a, (void *)0, &task_a_stack[TASK_STACK_SIZE-1], 0);		/* Function called to create TASK a*/
	
	status = OSTaskCreate(task_b, (void *)0, &task_b_stack[TASK_STACK_SIZE-1], 1);		/* Function called to create TASK b*/
	
    OSStart();			/* Function called to start multitasking*/
	return 0;
}

/*==================== Task A ==================== */
void task_a(void *pdata)
{
	int temp=0;;
	INT8U status;
	/* Function called to initialise the timer */
	timer_init();
	
	ledoff(temp);
 	while(1) 
 	{
		ledoff(temp);
	 	OSTimeDly(500);
	 	ledon(temp);
	 	
	 	/*  Post Or Transmit Mailbox Message  */
	 	status= OSMboxPost(TxMbox,(void*)&temp);
	 	if(status==OS_NO_ERR)	
		{
			//msg posted
		 	IOSET0 = 1<<(6);
		}	
		else
		 	IOCLR0 = 1<<(6);
		
    	/* Wait Mailbox Acknoledge  */
    	OSMboxPend(AckMbox, 0, &status);
		if(status==OS_NO_ERR)	
		{
			//msg received
		 	IOSET0 = 1<<(5);
		}	
		else
		 	IOCLR0 = 1<<(5);
		temp++;	
		if(temp==2)
		{
			temp=0;
		} 	
	}
}

/*==================== Task B ==================== */
void task_b(void *pdata)
{
	int *rtemp;
	unsigned int i,j;
	INT8U status;
	
 	while(1) 
 	{
		/* Wait Mailbox Acknoledge  */
		rtemp=OSMboxPend(TxMbox,0,&status);
		
		ledoff1(*rtemp);
		OSTimeDly(500);
	 	ledon1(*rtemp);
	 	/*  give acknowledge  */
	 	OSMboxPost(AckMbox,(void*)1);
	}
}


void ledon(int led)
{
	IOSET0 = 1<<(led+7);
	
}
void ledon1(int led)
{
	IOSET0 = 1<<(led+12);
	
}

void ledoff(int led)
{
	IOCLR0 = 1<<(led+7);
	
}
void ledoff1(int led)
{
	IOCLR0 = 1<<(led+12);
}
void ledinit(void)
{
	IODIR0= 0x0000FFF0;
	IOCLR0= 0xffffffff;
}
