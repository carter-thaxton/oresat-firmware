/*! \file adf7030.c
 *
 * API for adf7030.c
 */

/*!
 * \defgroup adf7030 Semtech Radio Chip
 *
 * @{
 */

/*
 *
 *     Modification Log
 *     04/15/2018    Malay Das    Initial Code.  
 *
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#include "adf7030.h"
#include "cfg_pckt.h"

/*
 * ADF7030 saving contents of the configuration file
 * ADF7030 software reference manual p 23
 */
//const uint8_t Radio_Memory_Configuration[] = {
//#include "cfg_pckt.cfg"
//};


/*
 * Configuring ADF7030 by loading contents of the configuration file
 * ADF7030 software reference manual p 22
 */
void adf7030_cfg(SPIDriver * spip)
{


	spiSelect(spip);
	spiStartSend(spip, sizeof(cfg_buf1), cfg_buf1);
	while((*spip).state != SPI_READY) {}
	spiUnselect(spip);

	spiSelect(spip);
	spiStartSend(spip, sizeof(cfg_buf2), cfg_buf2);
	while((*spip).state != SPI_READY) {}
	spiUnselect(spip);

	spiSelect(spip);
	spiStartSend(spip, sizeof(cfg_buf3), cfg_buf3);
	while((*spip).state != SPI_READY) {}
	spiUnselect(spip);

	spiSelect(spip);
	spiStartSend(spip, sizeof(cfg_buf4), cfg_buf4);
	while((*spip).state != SPI_READY) {}
	spiUnselect(spip);

	spiSelect(spip);
	spiStartSend(spip, sizeof(cfg_buf5), cfg_buf5);
	while((*spip).state != SPI_READY) {}
	spiUnselect(spip);

	spiSelect(spip);
	spiStartSend(spip, sizeof(cfg_buf6), cfg_buf6);
	while((*spip).state != SPI_READY) {}
	spiUnselect(spip);

	spiSelect(spip);
	spiStartSend(spip, sizeof(cfg_buf7), cfg_buf7);
	while((*spip).state != SPI_READY) {}
	spiUnselect(spip);

	spiSelect(spip);
	spiStartSend(spip, sizeof(cfg_buf8), cfg_buf8);
	while((*spip).state != SPI_READY) {}
	spiUnselect(spip);

	spiSelect(spip);
	spiStartSend(spip, sizeof(cfg_buf9), cfg_buf9);
	while((*spip).state != SPI_READY) {}
	spiUnselect(spip);

	spiSelect(spip);
	spiStartSend(spip, sizeof(cfg_buf10), cfg_buf10);
	while((*spip).state != SPI_READY) {}
	spiUnselect(spip);

	spiSelect(spip);
	spiStartSend(spip, sizeof(cfg_buf11), cfg_buf11);
	while((*spip).state != SPI_READY) {}
	spiUnselect(spip);
}


/*
 * Reports ADF7030 status over SPI
 * ADF7030 software reference manual p 15
 */
void adf7030_status(SPIDriver * spip)
{
    uint8_t command_buf = AD_NOP;
    uint8_t rx_buf;
	spiSelect(spip);

	spiStartSend(spip, 1, &command_buf);
	while((*spip).state != SPI_READY) {}

	spiStartReceive(spip, 1, &rx_buf);
	while((*spip).state != SPI_READY) { }

    chprintf(DEBUG_CHP, "\r\r radio status-- 0x%x --\r\n", rx_buf);
	spiUnselect(spip);
}


/*
 * ADF7030 state change functions - PHY_SLEEP state
 */
void adf7030_phy_sleep(SPIDriver * spip)
{
    uint8_t command_buf = AD_CNM_1|AD_CMD_PHY_SLEEP;

	spiSelect(spip);

	spiStartSend(spip, 1, &command_buf);
	while((*spip).state != SPI_READY) {}

    chprintf(DEBUG_CHP, "\r\r phy sleep-- 0x%x --\r\n", command_buf);
	spiUnselect(spip);
}


/*
 * ADF7030 state change functions - PHY_OFF state
 */
void adf7030_phy_off(SPIDriver * spip)
{
    uint8_t command_buf = AD_CNM_1|AD_CMD_PHY_OFF;

	spiSelect(spip);

	spiStartSend(spip, 1, &command_buf);
	while((*spip).state != SPI_READY) {}

    chprintf(DEBUG_CHP, "\r\r phy off-- 0x%x --\r\n", command_buf);
	spiUnselect(spip);
}


/*
 * ADF7030 state change functions - PHY_ON state
 */
void adf7030_phy_on(SPIDriver * spip)
{
    uint8_t command_buf = AD_CNM_1|AD_CMD_PHY_ON;

	spiSelect(spip);

	spiStartSend(spip, 1, &command_buf);
	while((*spip).state != SPI_READY) {}

    chprintf(DEBUG_CHP, "\r\r phy on-- 0x%x --\r\n", command_buf);
	spiUnselect(spip);
}


/*
 * ADF7030 state change functions - CFG_DEV state
 */
void adf7030_phy_cfg(SPIDriver * spip)
{
    uint8_t command_buf = AD_CNM_1|AD_CMD_CFG_DEV;

	spiSelect(spip);

	spiStartSend(spip, 1, &command_buf);
	while((*spip).state != SPI_READY) {}

    chprintf(DEBUG_CHP, "\r\r phy cfg-- 0x%x --\r\n", command_buf);
	spiUnselect(spip);
}


/*
 * ADF7030 state change functions - PHY_RX state
 */
void adf7030_phy_rx(SPIDriver * spip)
{
    uint8_t command_buf = AD_CNM_1|AD_CMD_PHY_RX;

	spiSelect(spip);

	spiStartSend(spip, 1, &command_buf);
	while((*spip).state != SPI_READY) {}

    chprintf(DEBUG_CHP, "\r\r phy rx-- 0x%x --\r\n", command_buf);
	spiUnselect(spip);
}


/*
 * ADF7030 state change functions - PHY_TX state
 */
void adf7030_phy_tx(SPIDriver * spip)
{
    uint8_t command_buf = AD_CNM_1|AD_CMD_PHY_TX;

	spiSelect(spip);

	spiStartSend(spip, 1, &command_buf);
	while((*spip).state != SPI_READY) {}

    chprintf(DEBUG_CHP, "\r\r phy tx-- 0x%x --\r\n", command_buf);
	spiUnselect(spip);
}


/*
 * Read ADF7030 status and debug register over SPI
 * ADF7030 software reference manual p 80
 */
void adf7030_read_status_reg(SPIDriver * spip)
{
    uint8_t command_buf[6];
    //uint8_t *address_buf = (uint8_t *)AD_MISC_FW;
    uint8_t rx_buf[5]={0 ,0 ,0 ,0 ,0};
    //command_buf[0] = 0x00;
    command_buf[0] = AD_READ_BLK_LADR;

    command_buf[1] = (uint8_t) (AD_MISC_FW >> 24);
    command_buf[2] = (uint8_t) (AD_MISC_FW >> 16);
    command_buf[3] = (uint8_t) (AD_MISC_FW >> 8);
    command_buf[4] = (uint8_t) (AD_MISC_FW);
    command_buf[5] = 0;
    //chprintf(DEBUG_CHP, "\r\r request-- 0x%x --\r\n", AD_MISC_FW);
    //chprintf(DEBUG_CHP, "\r\r request-- 0x%x %x %x %x %x %x --\r\n", command_buf[0],command_buf[1],command_buf[2],command_buf[3],command_buf[4],command_buf[5]);

	spiSelect(spip);

	spiStartSend(spip, 6, command_buf);
	while((*spip).state != SPI_READY) {}

	spiStartReceive(spip, 5, rx_buf);
	while((*spip).state != SPI_READY) { }

    chprintf(DEBUG_CHP, "\r\r radio status register content-- 0x%x %x %x %x %x --\r\n", rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4]);
	spiUnselect(spip);
}


/*
 * write a random packet on ADF7030
 * ADF7030 software reference manual p 80
 */
void adf7030_write_packet(SPIDriver * spip)
{
    uint8_t command_buf[511];
    command_buf[0] = (uint8_t) (AD_WRITE_BLK_LADR);
    command_buf[1] = (uint8_t) (AD_PACKET_MEMORY >> 24);
    command_buf[2] = (uint8_t) (AD_PACKET_MEMORY >> 16);
    command_buf[3] = (uint8_t) (AD_PACKET_MEMORY >> 8);
    command_buf[4] = (uint8_t) (AD_PACKET_MEMORY);
    command_buf[5] = 1;
    command_buf[6] = 2;
    command_buf[7] = 3;
    command_buf[8] = 4;
    command_buf[9] = 5;
    command_buf[10] = 6;
    command_buf[11] = 7;
    command_buf[12] = 8;
    command_buf[13] = 9;
    command_buf[14] = 10;
    command_buf[15] = 11;
    command_buf[16] = 12;
    command_buf[17] = 13;
    command_buf[18] = 14;
    command_buf[19] = 15;
    command_buf[20] = 16;
    command_buf[21] = 17;
    command_buf[22] = 18;
    command_buf[23] = 19;
    command_buf[24] = 20;
    command_buf[25] = 21;
    command_buf[26] = 22;
    command_buf[27] = 23;
    command_buf[28] = 24;
    command_buf[29] = 25;
    command_buf[30] = 26;
    command_buf[31] = 27;



    //chprintf(DEBUG_CHP, "\r\r request-- 0x%x --\r\n", AD_PACKET_MEMORY);
    //chprintf(DEBUG_CHP, "\r\r request-- 0x%x %x %x %x %x %x --\r\n", command_buf[0],command_buf[1],command_buf[2],command_buf[3],command_buf[4],command_buf[5]);

	spiSelect(spip);

	spiStartSend(spip, 511, &command_buf);
	while((*spip).state != SPI_READY) {}

	spiUnselect(spip);
}


/*
 * Read ADF7030 addresses SPI
 */
void adf7030_read_address(SPIDriver * spip, uint32_t address)
{
    uint8_t command_buf[6];
    //uint8_t *address_buf = (uint8_t *)AD_MISC_FW;
    uint8_t rx_buf[5]={0 ,0 ,0 ,0 ,0};
    //command_buf[0] = 0x00;
    command_buf[0] = AD_READ_BLK_LADR;

    command_buf[1] = (uint8_t) (address >> 24);
    command_buf[2] = (uint8_t) (address >> 16);
    command_buf[3] = (uint8_t) (address >> 8);
    command_buf[4] = (uint8_t) (address);
    command_buf[5] = 0;
    //chprintf(DEBUG_CHP, "\r\r request-- 0x%x --\r\n", AD_MISC_FW);
    //chprintf(DEBUG_CHP, "\r\r request-- 0x%x %x %x %x %x %x --\r\n", command_buf[0],command_buf[1],command_buf[2],command_buf[3],command_buf[4],command_buf[5]);

	spiSelect(spip);

	spiStartSend(spip, 6, command_buf);
	while((*spip).state != SPI_READY) {}

	spiStartReceive(spip, 5, rx_buf);
	while((*spip).state != SPI_READY) { }

    chprintf(DEBUG_CHP, "\r\r radio address 0x%x content : 0x%x %x %x %x %x --\r\n",address, rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4]);
	spiUnselect(spip);
}


/*
 * write to an address
 * ADF7030 software reference manual p 80
 */
void adf7030_write_address(SPIDriver * spip, uint32_t address, uint32_t content)
{
    uint8_t command_buf[9];
    command_buf[0] = (uint8_t) (AD_WRITE_BLK_LADR);
    command_buf[1] = (uint8_t) (address >> 24);
    command_buf[2] = (uint8_t) (address >> 16);
    command_buf[3] = (uint8_t) (address >> 8);
    command_buf[4] = (uint8_t) (address);
    command_buf[5] = (uint8_t) (content >> 24);
    command_buf[6] = (uint8_t) (content >> 16);
    command_buf[7] = (uint8_t) (content >> 8);
    command_buf[8] = (uint8_t) (content);



	spiSelect(spip);

	spiStartSend(spip, 9, &command_buf);
	while((*spip).state != SPI_READY) {}

	spiUnselect(spip);
    chprintf(DEBUG_CHP, "\r\r radio address 0x%x written with : 0x%x --\r\n",address, content);

}



// ! @}
