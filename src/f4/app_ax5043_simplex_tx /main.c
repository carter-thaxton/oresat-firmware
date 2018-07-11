/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/***************************************************
 *     Modification Log
 *     04/15/2018    Malay Das    Initial Code.  
 ***************************************************/


/*
 * Include Files
 */
#include <stdbool.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "util_version.h"
#include "util_numbers.h"
//#include "adf7030.h"

#define     DEBUG_SERIAL                    SD2
#define     DEBUG_CHP                       ((BaseSequentialStream *) &DEBUG_SERIAL)


/*
 * Serial Driver Configuration
 */
static SerialConfig ser_cfg =
{
    115200,                                 //Baud rate
    0,                                      //
    0,                                      //
    0,                                      //
};


/*
 * Receive and Transmit SPI Configurations
 */
static const SPIConfig spicfg_rx =
{
    false,
    NULL,                                   // Operation complete callback
    GPIOA,                                  // Slave select port
    GPIOA_SPI1_NSS,                         // Slave select pad
    // SPI cr1 data                            (see 446 ref man.)
    SPI_CR1_SPE     |                       // SPI enable
    SPI_CR1_MSTR    |                       // Master
    //SPI_CR1_BR_2    |
    SPI_CR1_BR_1    |
    SPI_CR1_BR_0   |                        // fpclk/16  approx 5Mhz? BR = 0x011
    SPI_CR1_SSM,
    0, // SPI_CR2_SSOE,
};

static const SPIConfig spicfg_tx =
{
    false,
    NULL,                                   // Operation complete callback
    GPIOB,                                  // Slave select port
    GPIOB_SPI2_NSS,                         // Slave select pad
    // SPI cr1 data                         (see 446 ref man.)
    SPI_CR1_SPE     |                       // SPI enable
    SPI_CR1_MSTR    |                       // Master
    //SPI_CR1_BR_2    |
    SPI_CR1_BR_1    |
    SPI_CR1_BR_0   |                        // fpclk/16  approx 5Mhz? BR = 0x011
    SPI_CR1_SSM,
    0, // SPI_CR2_SSOE,
};


/*
 * Initialize the SPI drivers and configure the adf7030 chips
 */
static void app_init(void)
{
    // Start up debug output, chprintf(DEBUG_CHP,...)
    sdStart(&DEBUG_SERIAL, &ser_cfg);

    set_util_fwversion(&version_info);
    set_util_hwversion(&version_info);


    //Print FW/HW information
    chprintf(DEBUG_CHP, "\r\nFirmware Info\r\n");
    chprintf(DEBUG_CHP, "FW HASH: %s\r\n", version_info.firmware);
    chprintf(DEBUG_CHP, "STF0x UNIQUE HW ID (H,C,L):\r\n0x%x\t0x%x\t0x%x\r\n"
             , version_info.hardware.id_high
             , version_info.hardware.id_center
             , version_info.hardware.id_low
            );
		
    chThdSleepMilliseconds(1000);	
    spiStart(&SPID1, &spicfg_rx);
    spiStart(&SPID2, &spicfg_tx);
	//spiSelect(&SPID2); 
    chThdSleepMilliseconds(1000);

    chprintf(DEBUG_CHP, "Configuring AX5043\r\n");

    //adf7030_phy_off(&SPID1);
	//spiSelect(&SPID1);
    //chThdSleepMilliseconds(500);
	//spiUnselect(&SPID1);
    //adf7030_read_status_reg(&SPID1);

    //adf7030_phy_sleep(&SPID1);
    //chThdSleepMilliseconds(500);
    //adf7030_read_status_reg(&SPID1);

/*
    adf7030_phy_off(&SPID1);
    chThdSleepMilliseconds(500);
    adf7030_status(&SPID1);
    adf7030_read_status_reg(&SPID1);

    adf7030_cfg(&SPID1);
    //adf7030_write_address(&SPID1, (uint32_t) (0x20000548), (uint32_t) (0x00000200));
    adf7030_status(&SPID1);
    chThdSleepMilliseconds(2500);

    adf7030_phy_cfg(&SPID1);
    //adf7030_read_status_reg(&SPID1);
    chThdSleepMilliseconds(2500);
    adf7030_read_address(&SPID1, (uint32_t) (0x20000548));
    //adf7030_read_address(&SPID1, (uint32_t) (0x200002EC));
    //adf7030_read_address(&SPID1, (uint32_t) (0x2000031C));
    //adf7030_write_address(&SPID1, (uint32_t) (0x20000548), (uint32_t) (0x00000100));
    //adf7030_read_address(&SPID1, (uint32_t) (0x20000548));
    adf7030_status(&SPID1);
    adf7030_read_status_reg(&SPID1);


    adf7030_phy_off(&SPID1);
    chThdSleepMilliseconds(500);
    adf7030_status(&SPID1);
    adf7030_read_status_reg(&SPID1);

    adf7030_phy_on(&SPID1);
    chThdSleepMilliseconds(500);
    adf7030_status(&SPID1);
    adf7030_read_status_reg(&SPID1);

    //adf7030_phy_rx(&SPID1);
    //adf7030_read_status_reg(&SPID1);

    //adf7030_phy_tx(&SPID1);
    //chThdSleepMilliseconds(500);
    //adf7030_status(&SPID1);
    //adf7030_read_status_reg(&SPID1);
    adf7030_write_packet(&SPID1);

    adf7030_phy_tx(&SPID1);
    adf7030_read_status_reg(&SPID1);
    adf7030_status(&SPID1);
    //adf7030_write_packet(&SPID1);

	while (true)
    {
      chThdSleepMilliseconds(1000);
      chprintf(DEBUG_CHP, ".\n\r\n");
      adf7030_write_packet(&SPID1);
      adf7030_phy_tx(&SPID1);
      chThdSleepMilliseconds(1000);
      //adf7030_read_status_reg(&SPID1);
      //chThdSleepMilliseconds(500);
      adf7030_read_status_reg(&SPID1);
      adf7030_status(&SPID1);
    }
*/

	while (true)
    {
    //uint8_t command_buf[6];
    uint8_t rx_bufi=0;
    uint8_t rx_buf[5]={0 ,0 ,0 ,0 ,0};
    uint8_t command_buf = 0x00;
    //command_buf[0] = AD_READ_BLK_LADR;

    //command_buf[1] = (uint8_t) (AD_MISC_FW >> 24);
    //command_buf[2] = (uint8_t) (AD_MISC_FW >> 16);
    //command_buf[3] = (uint8_t) (AD_MISC_FW >> 8);
    //command_buf[4] = (uint8_t) (AD_MISC_FW);
    //ommand_buf[5] = 0;
    //chprintf(DEBUG_CHP, "\r\r request-- 0x%x --\r\n", AD_MISC_FW);
    //chprintf(DEBUG_CHP, "\r\r request-- 0x%x %x %x %x %x %x --\r\n", command_buf[0],command_buf[1],command_buf[2],command_buf[3],command_buf[4],command_buf[5]);

	spiSelect(&SPID2);
	spiStartReceive(&SPID2, 1, &rx_bufi);
	while((SPID2).state != SPI_READY) { }
    chprintf(DEBUG_CHP, "\r\r radio status received-- 0x%x  --\r\n", rx_bufi);
	//spiUnselect(&SPID2);
    chThdSleepMilliseconds(500);

	//spiSelect(&SPID2);
	spiStartSend(&SPID2, 1, &command_buf);
	while((SPID2).state != SPI_READY) {}

	spiStartReceive(&SPID2, 5, rx_buf);
	while((SPID2).state != SPI_READY) { }

    chprintf(DEBUG_CHP, "\r\r long radio status-- 0x%x %x %x %x --\r\n", rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4]);
	spiUnselect(&SPID2);
    chThdSleepMilliseconds(500);
    palTogglePad(GPIOA, GPIOA_SX_TESTOUT);
    }

}


/*
 * main loop blinks the led
 */
static void main_loop(void)
{   
    chThdSleepMilliseconds(500);


	while (true)
    {
        chThdSleepMilliseconds(500);
        chprintf(DEBUG_CHP, ".");
		palTogglePad(GPIOA, GPIOA_SX_TESTOUT);
    }

}


/*
 * Entry to our code
 */
int main(void)
{
    halInit();
    chSysInit();
    app_init();

    // Enabling events on both edges of the button line.*/
    //palEnableLineEvent(GPIOC_SX_DIO3, PAL_EVENT_MODE_RISING_EDGES);

	//chThdCreateStatic(waThread_sx1236_rx,      sizeof(waThread_sx1236_rx),   NORMALPRIO, Thread_sx1236_rx, NULL);
    //chThdSleepMilliseconds(500);
    //chThdCreateStatic(waThread_sx1236_tx,      sizeof(waThread_sx1236_tx),   NORMALPRIO, Thread_sx1236_tx, NULL);
    chThdSleepMilliseconds(500);

    main_loop();
    return 0;
}



