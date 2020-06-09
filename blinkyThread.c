#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <mqueue.h>
#include <ti/drivers/GPIO.h>
#include "Board.h"

void
*blinkyThread (void *arg0)
{
    /* Message queue descriptor */
    mqd_t *mqdes = (mqd_t *) arg0;
    int msg;

    /* Call driver init functions */
    GPIO_init ();

    /* Configure the LED pin and turn on LED1  */
    GPIO_setConfig (Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_write (Board_GPIO_LED1, Board_GPIO_LED_OFF);

    while (mq_receive (*mqdes, (char *)&msg, sizeof(msg), NULL) != -1) {
        int toggleNum = 2 * msg;
        int increment = 0;
        while (increment < toggleNum) {
            increment++;
            GPIO_toggle (Board_GPIO_LED0);
            sleep (1);
        }
    }

    return (void *) 0;
}
