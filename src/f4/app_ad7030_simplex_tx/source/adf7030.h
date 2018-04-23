/*! \file adf7030.h */


/*!
 * \addtogroup adf7030
 *  * @{
 *
 */
#ifndef _ADF7030_H
#define _ADF7030_H

#define DEBUG_SERIAL  SD2
#define DEBUG_CHP     ((BaseSequentialStream *) &DEBUG_SERIAL)

#include <stdbool.h>
#include <stdint.h>

#include "util_numbers.h"



/*
 * ADF7030 register addresses
 * ADF7030 software reference manual p 61 to 80
 */
#define AD_PACKET_MEMORY                ((uint32_t)(0x20000AF0))
#define AD_PTR_TX                       ((uint32_t)(0x20000AF0))
#define AD_PTR_RX                       ((uint32_t)(0x20000C18))


/*
 * ADF7030 register addresses
 * ADF7030 software reference manual p 61 to 80
 */
#define AD_MISC_FW                      ((uint32_t)(0x400042B4))

/*
 * ADF7030 state machine radio commands
 * ADF7030 software reference manual p 14
 */
#define AD_CNM_0                        ((uint8_t)(0b0<<7))
#define AD_CNM_1                        ((uint8_t)(0b1<<7))

#define AD_SPCNTRN_0                    ((uint8_t)(0b0<<6))
#define AD_SPCNTRN_1                    ((uint8_t)(0b1<<6))

#define AD_CMD_PHY_SLEEP                ((uint8_t)(0x00))
#define AD_CMD_PHY_OFF                  ((uint8_t)(0x01))
#define AD_CMD_PHY_ON                   ((uint8_t)(0x02))
#define AD_CMD_PHY_RX                   ((uint8_t)(0x03))
#define AD_CMD_PHY_TX                   ((uint8_t)(0x04))
#define AD_CMD_CFG_DEV                  ((uint8_t)(0x05))
#define AD_CMD_CCA                      ((uint8_t)(0x06))
#define AD_CMD_DO_CAL                   ((uint8_t)(0x09))
#define AD_CMD_GPCLK                    ((uint8_t)(0x10))
#define AD_CMD_MON                      ((uint8_t)(0x0A))
#define AD_CMD_LFRC_CAL                 ((uint8_t)(0x0C))

/*
 * ADF7030 status over SPI
 * ADF7030 software reference manual p 15
 */
#define AD_NOP                          ((uint8_t)(0xFF))


/*
 * ADF7030 status byte
 * ADF7030 software reference manual p 15
 */
#define AD_STATUS_CMD_READY_NO          ((uint8_t)(0b0<<5))
#define AD_STATUS_CMD_READY_YES         ((uint8_t)(0b1<<5))
#define AD_STATUS_TRANSITION_PROG       ((uint8_t)(0b00<<1))
#define AD_STATUS_TRANSITION_EXEC       ((uint8_t)(0b01<<1))
#define AD_STATUS_TRANSITION_IDLE       ((uint8_t)(0b10<<1))


/*
 * ADF7030 configuration radio commands
 * ADF7030 software reference manual p 16
 */
#define AD_CMD_IRQ1_DIS_IRQ0_DIS        ((uint8_t)(0x0C))
#define AD_CMD_IRQ1_DIS_IRQ0_EN         ((uint8_t)(0x0D))
#define AD_CMD_IRQ1_EN_IRQ0_DIS         ((uint8_t)(0x0E))


/*
 * ADF7030 configuration radio commands
 * ADF7030 software reference manual p 16
 */
#define AD_RNW_WRITE                    ((uint8_t)(0b0<<6))
#define AD_RNW_READ                     ((uint8_t)(0b1<<6))

#define AD_BNR_RANDOM                   ((uint8_t)(0b0<<5))
#define AD_BNR_BURST                    ((uint8_t)(0b1<<5))

#define AD_ANP_POINTER                  ((uint8_t)(0b0<<4))
#define AD_ANP_ADDRESS                  ((uint8_t)(0b1<<4))

#define AD_LNS_SHORT                    ((uint8_t)(0b0<<3))
#define AD_LNS_LONG                     ((uint8_t)(0b1<<3))

#define AD_MPNTR_POINTER0               ((uint8_t)(0b101))
#define AD_MPNTR_POINTER1               ((uint8_t)(0b110))
#define AD_MPNTR_POINTER2               ((uint8_t)(0b111))


/*
 * ADF7030 address read/write modes
 * ADF7030 software reference manual p 47
 */
#define AD_READ_BLK_LADR                ((uint8_t)(0x00|AD_RNW_READ|AD_BNR_BURST|AD_ANP_ADDRESS|AD_LNS_LONG))
#define AD_WRITE_BLK_LADR               ((uint8_t)(0x00|AD_RNW_WRITE|AD_BNR_BURST|AD_ANP_ADDRESS|AD_LNS_LONG))


/*
 * ADF7030 function definitions
 */
void adf7030_cfg(SPIDriver * spip);
uint8_t adf7030_status(SPIDriver * spip);
void adf7030_phy_sleep(SPIDriver * spip);
void adf7030_phy_off(SPIDriver * spip);
void adf7030_phy_on(SPIDriver * spip);
void adf7030_phy_cfg(SPIDriver * spip);
void adf7030_phy_rx(SPIDriver * spip);
void adf7030_phy_tx(SPIDriver * spip);
void adf7030_read_status_reg(SPIDriver * spip);
void adf7030_write_packet(SPIDriver * spip);
void adf7030_read_address(SPIDriver * spip, uint32_t address);
void adf7030_write_address(SPIDriver * spip, uint32_t address, uint32_t content);


#endif
//! @}
