/* **************************************************************
 * Project Name : 
 * Description  : Program to implement semaphore
 * Author       : 
 * Date         :           
 * *********************************************************** */
/* ========* uCOS-II : Task-to-task context switching.=========
 * TaskA posts semA and pends on semB,
 * TaskB pends on semA and posts semB, 
 * ============================================================*/
#include "includes.h"
/*========== UCOS_II parameter Declaration ==========*/
#define TASK_STACK_SIZE 512
OS_STK task_a_stack[TASK_STACK_SIZE];
OS_STK task_b_stack[TASK_STACK_SIZE];
void task_a(void *pdata);
void task_b(void *pdata);
OS_EVENT *sem_a;
OS_EVENT *sem_b;
/* ================== main() ======================*/
int  main (void)
{
    unsigned int i, j;
	/* Function called to initialise the LEDs*/
   	led_init();
  	/* Function called to initialise the OS*/
 	OSInit();
	/* Create task A (at priority 0) */
	OSTaskCreate(task_a, (void *)0, &task_a_stack[TASK_STACK_SIZE-1], 0);
    OSStart();
	return 0;
}

/*==================== Task A ==================== */
void task_a(void *pdata)
{
	int i=0;
	INT8U status;

	/* Initialize */
	timer_init();

	/* Semaphores */
	sem_a = OSSemCreate(0);
	sem_b = OSSemCreate(0);
	
	/* Create task B (at priority 1) */
	status = OSTaskCreate(
		task_b, (void *)0, &task_b_stack[TASK_STACK_SIZE-1], 1);
	
	/* Start task-to-task posting */
 	IOCLR0 =1<<12;// 0x00003000;
 	
 	while(1) 
 	{
		/* Posting semaphore A to task B */
	 	IOSET0 =1<<12;// 0x00003000;				/*0000 0000 0111 1000 0000 0000 0001 0000*/
	 	OSTimeDly(100);
	 	status= OSSemPost(sem_a);
		/* Wait for semaphore B */
    	OSSemPend(sem_b, 0, &status);
		
		
		IOCLR0 =1<<12;// 0x00003000;
	 	OSTimeDly(100);
	 	/* Posting semaphore A to task B */
    	OSSemPost(sem_a);
    	/* Wait for semaphore B */
    	OSSemPend(sem_b, 0, &status);
		
	}
}

/*==================== Task B ==================== */
void task_b(void *pdata)
{
	INT8U status;

 	IOCLR0 = 1<<5;//0x00000C00;
 	while(1) 
 	{
		/* Wait for semaphore A */
    	OSSemPend(sem_a, 0, &status);
		IOSET0 = 1<<5;//0x00000C00;
		OSTimeDly(100);
		
		/* Posting semaphore B to task A */
    	OSSemPost(sem_b);

		
		/* Wait for semaphore A */
    	OSSemPend(sem_a, 0, &status);
		
	 	IOCLR0 = 1<<5;//0x00000C00;
		OSTimeDly(100);
		/* Posting semaphore B to task A */
    	OSSemPost(sem_b);

	}
}
