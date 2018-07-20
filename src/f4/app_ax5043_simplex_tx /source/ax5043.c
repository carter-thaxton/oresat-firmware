#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#include "ax5043.h"


/**
 * writes 1 byte to a register addressed in short format
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
void ax5043_write_short_reg_8(SPIDriver * spip, uint16_t reg, uint8_t value, uint8_t ret_value[])
{
  uint8_t command_buf[3] = {0,0,0};
  
  if(reg <  0x0070)
  {
    command_buf[0]=0x80|reg;
    command_buf[1]=value;
    chprintf(DEBUG_CHP, "\r\r written [0]=0x%x, [1]=0x%x--\r\n", command_buf[0],command_buf[1]);
    spiSelect(spip);
    spiStartExchange(spip, 2, command_buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);
    chprintf(DEBUG_CHP, "\r\r read after writting [0]=0x%x, [1]=0x%x--\r\n", ret_value[0],ret_value[1]);
  }
  else
  {
    command_buf[0]=0xF0|(reg>>8);
    command_buf[1]=reg;
    command_buf[2]=value;
    chprintf(DEBUG_CHP, "\r\r written [0]=0x%x, [1]=0x%x, [2]=0x%x--\r\n", command_buf[0],command_buf[1],command_buf[2]);
    spiSelect(spip);
    spiStartExchange(spip, 3, command_buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);
    chprintf(DEBUG_CHP, "\r\r read after writting [0]=0x%x, [1]=0x%x , [2]=0x%x--\r\n", ret_value[0],ret_value[1],ret_value[2]);
  }


}


/**
 * reads 1 byte to a short register addressed in short format
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
void ax5043_read_short_reg_8(SPIDriver * spip, uint16_t reg, uint8_t value, uint8_t ret_value[])
{
  uint8_t command_buf[3] = {0,0,0 };
  //uint8_t rx_buf[2]={0 ,0 };

  if(reg <  0x0070)
  {
    command_buf[0]=reg;
    command_buf[1]=value;
    spiSelect(spip);
    spiStartExchange(spip, 2, command_buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);
    chprintf(DEBUG_CHP, "\r\r spi read reg=0x%x, value=0x%x ret=0x%x %x--\r\n", command_buf[0],command_buf[1], ret_value[0],ret_value[1]);
  }
  else
  {
    command_buf[0]=0x70|(reg>>8);
    command_buf[1]=reg;
    command_buf[2]=value;
    spiSelect(spip);
    spiStartExchange(spip, 3, command_buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);
    chprintf(DEBUG_CHP, "\r\r spi read reg=0x%x %x, value=0x%x ret=0x%x %x %x--\r\n", command_buf[0],command_buf[1],command_buf[2],ret_value[0],ret_value[1],ret_value[2]);
  }
}




/**
 * Resets the AX5043
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
void ax5043_reset(SPIDriver * spip)
{

  //uint8_t reg=0;
  uint8_t value=0;
  uint8_t ret_value[2]={0,0};

  spiUnselect(spip);
  chThdSleepMicroseconds(5);
  spiSelect(spip);
  chThdSleepMicroseconds(5);
  spiUnselect(spip);
  chThdSleepMicroseconds(5);
  spiSelect(spip);
  chThdSleepMicroseconds(5);
  spiUnselect(spip);
  chThdSleepMicroseconds(5);

  //Reset the chip through powermode register 
  ax5043_write_short_reg_8(&SPID2, AX5043_REG_PWRMODE, AX5043_RESET_BIT, ret_value);
  chThdSleepMilliseconds(100);

  //read the powermode register
  //ax5043_read_short_reg_8(&SPID2, AX5043_REG_PWRMODE, value, ret_value);
  //write to powermode register for enabling XOEN and REFIN
  //page 33 in programming manual
  //value = ret_value[1] | AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT;
  value = AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT | AX5043_POWERDOWN;
  ax5043_write_short_reg_8(spip, AX5043_REG_PWRMODE, value, ret_value);
  chThdSleepMilliseconds(500);

  //set to shutdown powermode
  //page 33 in programming manual
  //ax5043_read_short_reg_8(&SPID2, reg, value, ret_value);
  //chThdSleepMilliseconds(500);

  //value = ret_value[1] | AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT | AX5043_POWERDOWN;
  //ax5043_write_short_reg_8(&SPID2, reg, value, ret_value);
  //chprintf(DEBUG_CHP, "\r\r reg=0x%x, value=0x%x, ret_value=0x%x 0x%x, --\r\n", reg,value,ret_value[0],ret_value[1]);
  //chThdSleepMilliseconds(100);

  //value = AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT | AX5043_FULL_RX;
  //ax5043_write_short_reg_8(&SPID2, AX5043_REG_PWRMODE, value, ret_value);
  //chThdSleepMilliseconds(500);
}

/**
 * Sets AX5043 registers
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
void ax5043_set_regs(SPIDriver * spip)
{
  uint8_t ret_value[2]={0,0};
  ax5043_write_short_reg_8(spip, AX5043_REG_MODULATION, (uint8_t)0x08, ret_value); 
  ax5043_write_short_reg_8(spip, AX5043_REG_ENCODING, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FRAMING, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PINFUNCSYSCLK, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PINFUNCDCLK, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PINFUNCDATA, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PINFUNCANTSEL, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PINFUNCPWRAMP, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_WAKEUPXOEARLY, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_IFFREQ1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_IFFREQ0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_DECIMATION, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_RXDATARATE2, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_RXDATARATE1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_RXDATARATE0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MAXDROFFSET2, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MAXDROFFSET1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MAXDROFFSET0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MAXRFOFFSET2, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MAXRFOFFSET1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MAXRFOFFSET0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FSKDMAX1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FSKDMAX0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FSKDMIN1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FSKDMIN0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_AMPLFILTER, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_RXPARAMSETS, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_AGCGAIN0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PHASEGAIN0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQUENCYGAINA0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQUENCYGAINB0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQUENCYGAINC0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQUENCYGAIND0, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_AMPLITUDEGAIN0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQDEV10, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQDEV00, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_BBOFFSRES0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_AGCGAIN1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_AGCTARGET1, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_AGCAHYST1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_AGCMINMAX1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_TIMEGAIN1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_DRGAIN1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PHASEGAIN1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQUENCYGAINA1, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQUENCYGAINB1, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQUENCYGAINC1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQUENCYGAIND1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_AMPLITUDEGAIN1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQDEV11, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQDEV01, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FOURFSK1, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_BBOFFSRES1, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_AGCGAIN3, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_AGCTARGET3, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_AGCAHYST3, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_AGCMINMAX3, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_TIMEGAIN3, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_DRGAIN3, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_PHASEGAIN3, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQUENCYGAINA3, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQUENCYGAINB3, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQUENCYGAINC3, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQUENCYGAIND3, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_AMPLITUDEGAIN3, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQDEV13, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_FREQDEV03, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_FOURFSK3, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_BBOFFSRES3, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MODCFGF, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FSKDEV2, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FSKDEV1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_FSKDEV0, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_MODCFGA, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_TXRATE2, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_TXRATE1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_TXRATE0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_TXPWRCOEFFB1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_TXPWRCOEFFB0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PLLVCOI, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_PLLRNGCLK, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_BBTUNE, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_BBOFFSCAP, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PKTADDRCFG, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PKTLENCFG, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PKTLENOFFSET, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PKTMAXLEN, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_MATCH0PAT3, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_MATCH0PAT2, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MATCH0PAT1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MATCH0PAT0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MATCH0LEN, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MATCH0MAX, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MATCH1PAT1, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_MATCH1PAT0, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_MATCH1LEN, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_MATCH1MAX, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_TMGTXBOOST, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_TMGTXSETTLE, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_TMGRXBOOST, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_TMGRXSETTLE, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_TMGRXOFFSACQ, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_TMGRXCOARSEAGC, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_TMGRXRSSI, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_TMGRXPREAMBLE2, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_RSSIABSTHR, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_BGNDRSSITHR, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_PKTCHUNKSIZE, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_PKTACCEPTFLAGS, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_DACVALUE1, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_DACVALUE0, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_DACCONFIG, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_REF, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_XTALOSC, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_XTALAMPL, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_0xF1C, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_0xF21, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_0xF22, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_0xF23, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_0xF26, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_0xF34, (uint8_t)0x08, ret_value);  
  ax5043_write_short_reg_8(spip, AX5043_REG_0xF35, (uint8_t)0x08, ret_value);
  ax5043_write_short_reg_8(spip, AX5043_REG_0xF44, (uint8_t)0x08, ret_value);
}


/**
 * Shutdown after reset the AX5043
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
void ax5043_shutdown(SPIDriver * spip)
{
  uint8_t ret_value[2]={0,0};
  ax5043_write_short_reg_8(spip, AX5043_REG_PWRMODE, AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT | AX5043_POWERDOWN, ret_value);
}


/**
 * Shutdown after reset the AX5043
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
void ax5043_full_rx(SPIDriver * spip)
{
  uint8_t ret_value[2]={0,0};
  ax5043_write_short_reg_8(spip, AX5043_REG_PWRMODE, AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT | AX5043_FULL_RX, ret_value);
}


/**
 * Shutdown after reset the AX5043
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
void ax5043_full_tx(SPIDriver * spip)
{
  uint8_t ret_value[2]={0,0};
  ax5043_write_short_reg_8(spip, AX5043_REG_PWRMODE, AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT | AX5043_FULL_TX, ret_value);
}



/**
 * Initializes the AX5043
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
void ax5043_init(SPIDriver * spip)
{
  ax5043_reset(spip);
  ax5043_shutdown(spip);

}


