#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <mqueue.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "Board.h"

void
*uartThread (void *arg0)
{
    mqd_t *mqdes = (mqd_t *) arg0;

    const char  echoPrompt[] = "Echoing characters:\r\n";
    UART_Handle uart;
    UART_Params uartParams;

    /* Call driver init functions */
    UART_init ();

    /* Create a UART with data processing off */
    UART_Params_init (&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readTimeout = UART_WAIT_FOREVER;
    uartParams.writeTimeout = UART_WAIT_FOREVER;

    uart = UART_open (Board_UART0, &uartParams);

    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }

    UART_write (uart, echoPrompt, sizeof(echoPrompt));

    char input;
    int msg;
    /* Loop forever echoing input received from the UART */
    while (1) {
        UART_read (uart, &input, 1);

        if (input == '2'){
            msg = 2;
            mq_send (*mqdes, (char *)&msg, sizeof(msg), 0);
        }

        if (input == '3'){
            msg = 3;
            mq_send (*mqdes, (char *)&msg, sizeof(msg), 0);
        }

        if (input == '4'){
            msg = 4;
            mq_send (*mqdes, (char *)&msg, sizeof(msg), 0);
        }

        if (input == '5'){
            msg = 5;
            mq_send (*mqdes, (char *)&msg, sizeof(msg), 0);
        }

        /* Echo typed character to terminal */
        UART_write (uart, &input, 1);
    }
}



