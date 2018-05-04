/*
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/***** Includes *****/


/* Example/Board Header files */


/* Standard C Libraries */
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* TI Drivers */
#include <ti/drivers/rf/RF.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/display/Display.h>

/* Driverlib Header files */
#include DeviceFamily_constructPath(driverlib/rf_prop_mailbox.h)

/* Board Header files */
#include "Board.h"
#include "smartrf_settings/smartrf_settings.h"

/***** Defines *****/

/* Packet TX Configuration */
#define PAYLOAD_LENGTH      32
#define PACKET_INTERVAL     500000  /* Set packet interval to 500000us or 500ms */
#define THREADSTACKSIZE (1024)

#define SPI_MSG_LENGTH  (64)
#define SLAVE_MSG       ("Hello from slave, msg#: ")

#define MAX_LOOP        (300)


/* Do power measurement */
//#define POWER_MEASUREMENT

/***** Prototypes *****/

/***** Variable declarations *****/
static RF_Object rfObject;
static RF_Handle rfHandle;

/* Pin driver handle */
static PIN_Handle ledPinHandle;
static PIN_State ledPinState;

static uint8_t packet[PAYLOAD_LENGTH];
//static uint16_t seqNumber;

static Display_Handle display;

uint8_t slaveRxBuffer[SPI_MSG_LENGTH];
uint8_t slaveTxBuffer[SPI_MSG_LENGTH];

/* Semaphore to block slave until transfer is complete */
sem_t slaveSem;


/*
 * Application LED pin configuration table:
 *   - All LEDs board LEDs are off.
 */
PIN_Config pinTable[] =
{
    Board_PIN_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#if defined __CC1352R1_LAUNCHXL_BOARD_H__
    Board_DIO30_RFSW | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#endif
#ifdef POWER_MEASUREMENT
#if defined(__CC1350_LAUNCHXL_BOARD_H__)
    Board_DIO30_SWPWR | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#endif
#endif
    PIN_TERMINATE
};

/*
*  ======== transferCompleteFxn ========
*  Callback function for SPI_transfer().
*/
void transferCompleteFxn(SPI_Handle handle, SPI_Transaction *transaction)
{
   sem_post(&slaveSem);
}

/*
* ======== Radio transfer function ========
*  Transmits a packet via radio.
*/

void radioTxFxn(uint8_t tx_packet[])
{

        uint8_t i;
        for (i = 0; i < PAYLOAD_LENGTH; i++)
        {
            packet[i] = tx_packet[i];
        }

        /* Send packet */
        RF_EventMask terminationReason = RF_runCmd(rfHandle, (RF_Op*)&RF_cmdPropTx,
                                                   RF_PriorityNormal, NULL, 0);

        switch(terminationReason)
        {
            case RF_EventCmdDone:
                // A radio operation command in a chain finished
                break;
            case RF_EventLastCmdDone:
                // A stand-alone radio operation command or the last radio
                // operation command in a chain finished.
                break;
            case RF_EventCmdCancelled:
                // Command cancelled before it was started; it can be caused
            // by RF_cancelCmd() or RF_flushCmd().
                break;
            case RF_EventCmdAborted:
                // Abrupt command termination caused by RF_cancelCmd() or
                // RF_flushCmd().
                break;
            case RF_EventCmdStopped:
                // Graceful command termination caused by RF_cancelCmd() or
                // RF_flushCmd().
                break;
            default:
                // Uncaught error event
                while(1);
        }

        uint32_t cmdStatus = ((volatile RF_Op*)&RF_cmdPropTx)->status;
        switch(cmdStatus)
        {
            case PROP_DONE_OK:
                // Packet transmitted successfully
                break;
            case PROP_DONE_STOPPED:
                // received CMD_STOP while transmitting packet and finished
                // transmitting packet
                break;
            case PROP_DONE_ABORT:
                // Received CMD_ABORT while transmitting packet
                break;
            case PROP_ERROR_PAR:
                // Observed illegal parameter
                break;
            case PROP_ERROR_NO_SETUP:
                // Command sent without setting up the radio in a supported
                // mode using CMD_PROP_RADIO_SETUP or CMD_RADIO_SETUP
                break;
            case PROP_ERROR_NO_FS:
                // Command sent without the synthesizer being programmed
                break;
            case PROP_ERROR_TXUNF:
                // TX underflow observed during operation
                break;
            default:
                // Uncaught error event - these could come from the
                // pool of states defined in rf_mailbox.h
                while(1);
        }

#ifndef POWER_MEASUREMENT
        PIN_setOutputValue(ledPinHandle, Board_PIN_LED1,!PIN_getOutputValue(Board_PIN_LED1));
#endif
        /* Power down the radio */
        RF_yield(rfHandle);

        /* Sleep for PACKET_INTERVAL us */
        //usleep(PACKET_INTERVAL);

}

/*
* ======== slaveThread ========
*  Slave SPI sends a message to master while simultaneously receiving a
*  message from the master.
*/
void *slaveThread(void *arg0)
{
   SPI_Handle      slaveSpi;
   SPI_Params      spiParams;
   SPI_Transaction transaction;
   uint32_t        i;
   uint32_t        j;
   bool            transferOK;
   int32_t         status;
   uint8_t         tx_packet[PAYLOAD_LENGTH];

   /*
    * Create synchronization semaphore; this semaphore will block the slave
    * until a transfer is complete.  The slave is configured in callback mode
    * to allow us to configure the SPI transfer & then notify the master the
    * slave is ready.  However, we must still wait for the current transfer
    * to be complete before setting up the next.  Thus, we wait on slaveSem;
    * once the transfer is complete the callback function will unblock the
    * slave.
    */
   status = sem_init(&slaveSem, 0, 0);
   if (status != 0) {
       Display_printf(display, 0, 0, "Error creating slaveSem\n");

       while(1);
   }


   /*
    * Open SPI as slave in callback mode; callback mode is used to allow us to
    * configure the transfer & then set Board_SPI_SLAVE_READY high.
    */
   SPI_Params_init(&spiParams);
   spiParams.frameFormat = SPI_POL0_PHA1;
   spiParams.mode = SPI_SLAVE;
   spiParams.transferCallbackFxn = transferCompleteFxn;
   spiParams.transferMode = SPI_MODE_CALLBACK;
   slaveSpi = SPI_open(Board_SPI_SLAVE, &spiParams);
   if (slaveSpi == NULL) {
       Display_printf(display, 0, 0, "Error initializing slave SPI\n");
       while (1);
   }
   else {
       Display_printf(display, 0, 0, "Slave SPI initialized\n");
   }

   /* Copy message to transmit buffer */
   strncpy((char *) slaveTxBuffer, SLAVE_MSG, SPI_MSG_LENGTH);

   for (i = 0; i < MAX_LOOP; i++) {
       /* Initialize slave SPI transaction structure */
       slaveTxBuffer[sizeof(SLAVE_MSG) - 1] = (i % 10) + '0';
       memset((void *) slaveRxBuffer, 0, SPI_MSG_LENGTH);
       transaction.count = SPI_MSG_LENGTH;
       transaction.txBuf = (void *) slaveTxBuffer;
       transaction.rxBuf = (void *) slaveRxBuffer;

       /*
        * Setup SPI transfer; Board_SPI_SLAVE_READY will be set to notify
        * master the slave is ready.
        */
       transferOK = SPI_transfer(slaveSpi, &transaction);
       if (transferOK) {

           /* Wait until transfer has completed */
           sem_wait(&slaveSem);


           Display_printf(display, 0, 0, "Slave received from Master");

           if(slaveRxBuffer[0]==0x01){
               for (j = 1; j <= PAYLOAD_LENGTH; j++){
                   Display_printf(display, 0, 0, "Slave received: %x", slaveRxBuffer[j]);
                   tx_packet[j-1]=slaveRxBuffer[j];
               }
               radioTxFxn(tx_packet);
           }
       }
       else {
           Display_printf(display, 0, 0, "Unsuccessful slave SPI transfer");
       }
   }

   SPI_close(slaveSpi);

   /* Example complete - set pins to a known state */
   GPIO_setConfig(Board_SPI_MASTER_READY, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);

   Display_printf(display, 0, 0, "\nDone");

   return (NULL);
}





/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    pthread_t           thread0;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    RF_Params rfParams;
    RF_Params_init(&rfParams);



    /* Open LED pins */
    ledPinHandle = PIN_open(&ledPinState, pinTable);
    if (ledPinHandle == NULL)
    {
        while(1);
    }



#ifdef POWER_MEASUREMENT
#if defined(__CC1350_LAUNCHXL_BOARD_H__)
    /* Route out PA active pin to Board_DIO30_SWPWR */
    PINCC26XX_setMux(ledPinHandle, Board_DIO30_SWPWR, PINCC26XX_MUX_RFC_GPO1);
#endif
#endif

    RF_cmdPropTx.pktLen = PAYLOAD_LENGTH;
    RF_cmdPropTx.pPkt = packet;
    RF_cmdPropTx.startTrigger.triggerType = TRIG_NOW;
    RF_cmdPropTx.startTrigger.pastTrig = 1;
    RF_cmdPropTx.startTime = 0;

    /* Request access to the radio */
    rfHandle = RF_open(&rfObject, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rfParams);

    /* Set the frequency */
    RF_postCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

    /* Call driver init functions. */
    Display_init();
    GPIO_init();
    SPI_init();

    /* Open the display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        /* Failed to open display driver */
        while (1);
    }


    Display_printf(display, 0, 0, "Starting the SPI slave example");
    Display_printf(display, 0, 0, "This example requires external wires to be "
        "connected to the header pins. Please see the Board.html for details.\n");

    /* Create application thread */
    pthread_attr_init(&attrs);

    detachState = PTHREAD_CREATE_DETACHED;
    /* Set priority and stack size attributes */
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    /* Create slave thread */
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc = pthread_create(&thread0, &attrs, slaveThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    return (NULL);
}

