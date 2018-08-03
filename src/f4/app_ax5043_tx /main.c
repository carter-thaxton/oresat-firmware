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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <stdio.h>
#include <stdbool.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "util_version.h"
#include "util_numbers.h"
//#include "ax5043.h"

//#include "adf7030.h"

#include "ax.h"
#include "ax_hw.h"
#include "ax_modes.h"
#include "ax_reg_values.h"

#define     DEBUG_SERIAL                    SD2
#define     DEBUG_CHP                       ((BaseSequentialStream *) &DEBUG_SERIAL)

uint8_t ax25_frame[0x100];
#define AX25_CONTROL_WORD   0x03 /* Use Unnumbered Information (UI) frames */
#define AX25_PROTOCOL_ID    0xF0 /* No third level protocol */


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



/*
    uint8_t rx_buf[2]={0x00 ,0 };
    uint8_t command_buf[2] = {0x00 ,0x00 };

	while (true)
    {
    //uint8_t command_buf[6];
    //uint8_t rx_bufi=0;

    //command_buf[0] = AD_READ_BLK_LADR;

    //command_buf[1] = (uint8_t) (AD_MISC_FW >> 24);
    //command_buf[2] = (uint8_t) (AD_MISC_FW >> 16);
    //command_buf[3] = (uint8_t) (AD_MISC_FW >> 8);
    //command_buf[4] = (uint8_t) (AD_MISC_FW);
    //ommand_buf[5] = 0;
    //chprintf(DEBUG_CHP, "\r\r request-- 0x%x --\r\n", AD_MISC_FW);
    //chprintf(DEBUG_CHP, "\r\r request-- 0x%x %x %x %x %x %x --\r\n", command_buf[0],command_buf[1],command_buf[2],command_buf[3],command_buf[4],command_buf[5]);

	spiSelect(&SPID2);
	spiStartExchange(&SPID2, 2, command_buf, rx_buf);
	while((SPID2).state != SPI_READY) { }
    chprintf(DEBUG_CHP, "\r\r Sent to Radio-- 0x%x 0x%x --\r\n", command_buf[0],command_buf[1]);
    chprintf(DEBUG_CHP, "\r\r Radio returned-- 0x%x 0x%x --\r\n", rx_buf[0],rx_buf[1]);
	spiUnselect(&SPID2);
    chThdSleepMilliseconds(500);
    command_buf[0] = command_buf[0] +1;

    palTogglePad(GPIOA, GPIOA_SX_TESTOUT);
    }
*/

/*
    uint16_t reg=0;
    uint8_t value=0;
    uint8_t value1=0x55;
    uint8_t ret_value[3]={0,0,0};


    reg = AX5043_REG_REV;
    ax5043_read_reg(&SPID2, reg, value, ret_value);
    //chprintf(DEBUG_CHP, "\r\r reg=0x%x, value=0x%x, ret_value=0x%x 0x%x, --\r\n", reg,value,ret_value[0],ret_value[1]);
    chThdSleepMilliseconds(1500);

    reg = AX5043_REG_SCRATCH;
    ax5043_read_reg(&SPID2, reg, value, ret_value);
    //chprintf(DEBUG_CHP, "\r\r reg=0x%x, value=0x%x, ret_value=0x%x 0x%x, --\r\n", reg,value,ret_value[0],ret_value[1]);
    chThdSleepMilliseconds(1500);

    reg = AX5043_REG_SCRATCH;
    value1 =value1+1;
    ax5043_write_reg(&SPID2, reg, value1, ret_value);
    //chprintf(DEBUG_CHP, "\r\r written reg=0x%x, value=0x%x, ret_value=0x%x 0x%x, --\r\n", reg,value1,ret_value[0],ret_value[1]);
    chThdSleepMilliseconds(1500);

    reg = AX5043_REG_SCRATCH;
    ax5043_read_reg(&SPID2, reg, value, ret_value);
    //chprintf(DEBUG_CHP, "\r\r reg=0x%x, value=0x%x, ret_value=0x%x 0x%x, --\r\n", reg,value,ret_value[0],ret_value[1]);
    chThdSleepMilliseconds(1500);




    chprintf(DEBUG_CHP, "Configuring AX5043\r\n");
    chThdSleepMilliseconds(50);
    ax5043_init(&SPID2);
    chprintf(DEBUG_CHP, "done reseting AX5043\r\n");
*/

/*
  while(true)
  {

    reg = 0x314;
    ax5043_write_reg(&SPID2, reg, value1, ret_value);
    //chprintf(DEBUG_CHP, "\r\r reg 220 contents=0x%x, value=0x%x, ret_value=0x%x 0x%x, --\r\n", reg,value,ret_value[0],ret_value[1]);
    chThdSleepMilliseconds(1500);

    reg = 0x314;
    ax5043_read_reg(&SPID2, reg, value, ret_value);
    //chprintf(DEBUG_CHP, "\r\r reg 220 contents=0x%x, value=0x%x, ret_value=0x%x 0x%x, --\r\n", reg,value,ret_value[0],ret_value[1]);
    chThdSleepMilliseconds(1500);

    reg = AX5043_REG_SCRATCH;
    value1 =value1+1;
    ax5043_write_reg(&SPID2, reg, value1, ret_value);
    //chprintf(DEBUG_CHP, "\r\r written reg=0x%x, value=0x%x, ret_value=0x%x 0x%x, --\r\n", reg,value1,ret_value[0],ret_value[1]);
    chThdSleepMilliseconds(1500);

    reg = AX5043_REG_SCRATCH;
    ax5043_read_reg(&SPID2, reg, value, ret_value);
    //chprintf(DEBUG_CHP, "\r\r reg=0x%x, value=0x%x, ret_value=0x%x 0x%x, --\r\n", reg,value,ret_value[0],ret_value[1]);
    chThdSleepMilliseconds(1500);
  }*/

/*
  ax5043_prepare_tx(&SPID2);
  while(true)
  {
    ax5043_transmit(&SPID2);
    chThdSleepMilliseconds(3000);
    chprintf(DEBUG_CHP, ".");
  }
*/


  ax_packet rx_pkt;
  uint8_t tx_pkt[0x100];

  ax_config config;
  memset(&config, 0, sizeof(ax_config));

  config.clock_source = AX_CLOCK_SOURCE_TCXO;
  config.f_xtal = 16369000;

  config.synthesiser.A.frequency = 434600000;
  config.synthesiser.B.frequency = 434600000;

  config.spip = &SPID2;

  config.pkt_store_flags = AX_PKT_STORE_RSSI |
    AX_PKT_STORE_RF_OFFSET;


  /* ------- init ------- */
  ax_init(&config);
  ax_default_params(&config, &gmsk_hdlc_fec_modulation);

  /* -------- tx -------- */
  /* ax_tx_on(&config, &gmsk_hdlc_fec_modulation); */
  /* while (1) { */
  /*   /\* int aprs_len = aprs(); *\/ */
  /*   /\* ax_tx_packet(&config, &msk1_modulation, ax25_frame, aprs_len); *\/ */

  /*                          //////////////////////////////////////// */
  /*   strcpy((char*)tx_pkt, "ughdffgiuhdfudshfdjshfdjshfsudhfdskjfdfd"); */
  /*   ax_tx_packet(&config, &gmsk_hdlc_fec_modulation, tx_pkt, 40); */
  /* } */

  /* -------- rx -------- */
  ax_rx_on(&config, &gmsk_hdlc_fec_modulation);
  while (1) {
    while (ax_rx_packet(&config, &rx_pkt)) {
      printf("rx!\n");
    }
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



