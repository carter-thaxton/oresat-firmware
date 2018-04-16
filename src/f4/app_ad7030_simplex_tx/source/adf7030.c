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

/*
 * ADF7030 saving contents of the configuration file
 * ADF7030 software reference manual p 23
 */
const uint8_t Radio_Memory_Configuration[] = {
#include "ADF7030.cfg"
};


/*
 * Configuring ADF7030 by loading contents of the configuration file
 * ADF7030 software reference manual p 22
 */
void adf7030_cfg(SPIDriver * spip)
{

	spiSelect(spip);

	spiStartSend(spip, sizeof(Radio_Memory_Configuration), Radio_Memory_Configuration);
	while((*spip).state != SPI_READY) {}

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

	spiUnselect(spip);
}

// ! @}
