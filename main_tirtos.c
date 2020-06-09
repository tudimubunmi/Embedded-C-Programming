#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include <ti/sysbios/BIOS.h>
#include <ti/drivers/Board.h>

extern void *blinkyThread (void *arg0);
extern void *uartThread (void *arg0);

/* Stack size in bytes */
#define THREADSTACKSIZE    1024
#define MSG_SIZE sizeof (int)
#define MSG_NUM 4

/* Message queue variable */
mqd_t mqdes;

int
main (void)
{
    pthread_t blinkyThreadID, uartThreadID;
    pthread_attr_t attrs;
    struct sched_param threadPriority;
    int status;

    /* Initialize board functions */
    Board_init ();

    /* Create the message queue for inter-thread communication */
    struct mq_attr mqAttribute;
    mqAttribute.mq_maxmsg = MSG_NUM;
    mqAttribute.mq_msgsize = MSG_SIZE;
    mqAttribute.mq_flags = 0;
    mqdes = mq_open ("mailbox", O_RDWR | O_CREAT, 0664, &mqAttribute);
    if (mqdes == (mqd_t)-1) {
      /* mq_open() failed */
      while (1);
    }

    /* Create thread to control LED1 */
    pthread_attr_init (&attrs);
    threadPriority.sched_priority = 1;
    pthread_attr_setschedparam (&attrs, &threadPriority);
    pthread_attr_setdetachstate (&attrs, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize (&attrs, THREADSTACKSIZE);
    status = pthread_create (&blinkyThreadID, &attrs, blinkyThread, (void *)&mqdes);
    if (status != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /* Create thread to read user input from the terminal */
    threadPriority.sched_priority = 2;
    pthread_attr_setschedparam (&attrs, &threadPriority);
    status = pthread_create (&uartThreadID, &attrs, uartThread, (void *)&mqdes);
    if (status != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /* Start RTOS scheduler */
    BIOS_start ();

    return (0);
}
