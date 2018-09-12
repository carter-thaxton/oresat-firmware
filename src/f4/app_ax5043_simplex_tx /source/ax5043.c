/*! \file ax5043.h */


/*!
 * \addtogroup ax5043
 *  * @{
 *
 */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#include "ax5043.h"


/**
 * writes  to an AX5043 register.
 * @param spip: SPI Configuration, reg: Register address, value: register value, ret_value: returned data.
 * @return the value of the register.
 */
uint8_t ax5043_write_reg_spi(SPIDriver * spip, uint16_t reg, uint8_t value, uint8_t ret_value[])
{
  uint8_t command_buf[3] = {0,0,0};
  
  if(reg <  0x0070)
  {
    command_buf[0]=0x80|reg;
    command_buf[1]=value;
    //chprintf(DEBUG_CHP, "\r\r written [0]=0x%x, [1]=0x%x--\r\n", command_buf[0],command_buf[1]);
    spiSelect(spip);
    spiStartExchange(spip, 2, command_buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);
    //chprintf(DEBUG_CHP, "\r\r read after writting [0]=0x%x, [1]=0x%x--\r\n", ret_value[0],ret_value[1]);
  }
  else
  {
    command_buf[0]=0xF0|(reg>>8);
    command_buf[1]=reg;
    command_buf[2]=value;
    //chprintf(DEBUG_CHP, "\r\r written [0]=0x%x, [1]=0x%x, [2]=0x%x--\r\n", command_buf[0],command_buf[1],command_buf[2]);
    spiSelect(spip);
    spiStartExchange(spip, 3, command_buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);
    //chprintf(DEBUG_CHP, "\r\r read after writting [0]=0x%x, [1]=0x%x , [2]=0x%x--\r\n", ret_value[0],ret_value[1],ret_value[2]);
  }
  return ret_value[0];   //retun status while writting the register

}


/**
 * Writes  to an AX5043 register. This has retry logic. This calls the the function ax5043_write_reg_spi.
 * @param spip: SPI Configuration, reg: Register address, value: register value, ret_value: returned data.
 * @return the value of the register.
 */
void ax5043_write_reg(SPIDriver * spip, uint16_t reg, uint8_t value, uint8_t ret_value[])
{
  uint8_t return_val;
  int num_retries = 5;
  return_val=ax5043_write_reg_spi(spip, reg, value, ret_value);
  
  while (num_retries > 0 && return_val != 0x80)
  {
    chThdSleepMicroseconds(100);
    return_val=ax5043_write_reg_spi(spip, reg, value, ret_value);
    num_retries--;
    //chprintf(DEBUG_CHP, "\r\r num_retries= %d --\r\n", num_retries);
  }

}

/**
 * Reds an AX5043 register. This has retry logic. This calls the the function ax5043_write_reg_spi.
 * @param spip: SPI Configuration, reg: Register address, value: register value, ret_value: returned data.
 * @return the value in the register.
 */
uint8_t ax5043_read_reg(SPIDriver * spip, uint16_t reg, uint8_t value, uint8_t ret_value[])
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
    //chprintf(DEBUG_CHP, "\r\r spi read reg=0x%x, value=0x%x ret=0x%x %x--\r\n", command_buf[0],command_buf[1], ret_value[0],ret_value[1]);
    return ret_value[1];    //return the reg value when reading the register
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
    //chprintf(DEBUG_CHP, "\r\r spi read reg=0x%x %x, value=0x%x ret=0x%x %x %x--\r\n", command_buf[0],command_buf[1],command_buf[2],ret_value[0],ret_value[1],ret_value[2]);
    return ret_value[2];    //return the reg value when reading the register
  }

}



/**
 * Sets AX5043 registers. This is based on config file generated by radio lab.
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_set_regs(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};
  ax5043_write_reg(spip, AX5043_REG_MODULATION, (uint8_t)0x08, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_ENCODING, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FRAMING, (uint8_t)0x26, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PINFUNCSYSCLK, (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PINFUNCDCLK, (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PINFUNCDATA, (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PINFUNCANTSEL, (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PINFUNCPWRAMP, (uint8_t)0x07, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_WAKEUPXOEARLY, (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_IFFREQ1, (uint8_t)0x07, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_IFFREQ0, (uint8_t)0x77, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_DECIMATION, (uint8_t)0x04, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_RXDATARATE2, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_RXDATARATE1, (uint8_t)0x3C, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_RXDATARATE0, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MAXDROFFSET2, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MAXDROFFSET1, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MAXDROFFSET0, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MAXRFOFFSET2, (uint8_t)0x80, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MAXRFOFFSET1, (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MAXRFOFFSET0, (uint8_t)0x30, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDMAX1, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDMAX0, (uint8_t)0xA6, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDMIN1, (uint8_t)0xFF, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDMIN0, (uint8_t)0x5A, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AMPLFILTER, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_RXPARAMSETS, (uint8_t)0xF4, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCGAIN0, (uint8_t)0x93, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_AGCTARGET0, (uint8_t)0x84, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_TIMEGAIN0, (uint8_t)0xF8, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_DRGAIN0, (uint8_t)0xF2, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PHASEGAIN0, (uint8_t)0xC3, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINA0, (uint8_t)0x0F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINB0, (uint8_t)0x1F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINC0, (uint8_t)0x06, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAIND0, (uint8_t)0x06, ret_value);
  ax5043_write_reg(spip, AX5043_REG_AMPLITUDEGAIN0, (uint8_t)0x06, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQDEV10, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQDEV00, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_BBOFFSRES0, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCGAIN1, (uint8_t)0x93, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCTARGET1, (uint8_t)0x84, ret_value);
  ax5043_write_reg(spip, AX5043_REG_AGCAHYST1, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCMINMAX1, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TIMEGAIN1, (uint8_t)0xF6, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_DRGAIN1, (uint8_t)0xF1, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PHASEGAIN1, (uint8_t)0xC3, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINA1, (uint8_t)0x0F, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINB1, (uint8_t)0x1F, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINC1, (uint8_t)0x06, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAIND1, (uint8_t)0x06, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AMPLITUDEGAIN1, (uint8_t)0x06, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQDEV11, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQDEV01, (uint8_t)0x43, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FOURFSK1, (uint8_t)0x16, ret_value);
  ax5043_write_reg(spip, AX5043_REG_BBOFFSRES1, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_AGCGAIN3, (uint8_t)0xFF, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCTARGET3, (uint8_t)0x84, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCAHYST3, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCMINMAX3, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TIMEGAIN3, (uint8_t)0xF5, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_DRGAIN3, (uint8_t)0xF0, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PHASEGAIN3, (uint8_t)0xC3, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINA3, (uint8_t)0x0F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINB3, (uint8_t)0x1F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINC3, (uint8_t)0x0A, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAIND3, (uint8_t)0x0A, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AMPLITUDEGAIN3, (uint8_t)0x06, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQDEV13, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FREQDEV03, (uint8_t)0x43, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FOURFSK3, (uint8_t)0x16, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_BBOFFSRES3, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MODCFGF, (uint8_t)0x03, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDEV2, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDEV1, (uint8_t)0x16, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDEV0, (uint8_t)0xC1, ret_value);
  ax5043_write_reg(spip, AX5043_REG_MODCFGA, (uint8_t)0x05, ret_value);
  ax5043_write_reg(spip, AX5043_REG_TXRATE2, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TXRATE1, (uint8_t)0x44, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TXRATE0, (uint8_t)0x44, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TXPWRCOEFFB1, (uint8_t)0x0F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TXPWRCOEFFB0, (uint8_t)0xFF, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PLLVCOI, (uint8_t)0x98, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PLLRNGCLK, (uint8_t)0x05, ret_value);
  ax5043_write_reg(spip, AX5043_REG_BBTUNE, (uint8_t)0x0B, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_BBOFFSCAP, (uint8_t)0x77, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PKTADDRCFG, (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PKTLENCFG, (uint8_t)0x80, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PKTLENOFFSET, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PKTMAXLEN, (uint8_t)0xC8, ret_value);
  ax5043_write_reg(spip, AX5043_REG_MATCH0PAT3, (uint8_t)0xAA, ret_value);
  ax5043_write_reg(spip, AX5043_REG_MATCH0PAT2, (uint8_t)0xCC, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MATCH0PAT1, (uint8_t)0xAA, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MATCH0PAT0, (uint8_t)0xCC, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MATCH0LEN, (uint8_t)0x1F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MATCH0MAX, (uint8_t)0x1F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MATCH1PAT1, (uint8_t)0x55, ret_value);
  ax5043_write_reg(spip, AX5043_REG_MATCH1PAT0, (uint8_t)0x55, ret_value);
  ax5043_write_reg(spip, AX5043_REG_MATCH1LEN, (uint8_t)0x8A, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MATCH1MAX, (uint8_t)0x0A, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TMGTXBOOST, (uint8_t)0x5B, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TMGTXSETTLE, (uint8_t)0x3E, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TMGRXBOOST, (uint8_t)0x5B, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TMGRXSETTLE, (uint8_t)0x3E, ret_value);
  ax5043_write_reg(spip, AX5043_REG_TMGRXOFFSACQ, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_TMGRXCOARSEAGC, (uint8_t)0x9C, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TMGRXRSSI, (uint8_t)0x03, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TMGRXPREAMBLE2, (uint8_t)0x35, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_RSSIABSTHR, (uint8_t)0xE0, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_BGNDRSSITHR, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PKTCHUNKSIZE, (uint8_t)0x0D, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTACCEPTFLAGS, (uint8_t)0x20, ret_value);
  ax5043_write_reg(spip, AX5043_REG_DACVALUE1, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_DACVALUE0, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_DACCONFIG, (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_REF, (uint8_t)0x03, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_XTALOSC, (uint8_t)0x04, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_XTALAMPL, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_0xF1C, (uint8_t)0x07, ret_value);
  ax5043_write_reg(spip, AX5043_REG_0xF21, (uint8_t)0x68, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_0xF22, (uint8_t)0XFF, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_0xF23, (uint8_t)0x84, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_0xF26, (uint8_t)0x98, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_0xF34, (uint8_t)0x28, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_0xF35, (uint8_t)0x11, ret_value);
  ax5043_write_reg(spip, AX5043_REG_0xF44, (uint8_t)0x25, ret_value);
}


/**
 * Sets AX5043 registers for TX. This is based on config file generated by radio lab.
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_set_regs_tx(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};
  ax5043_write_reg(spip, AX5043_REG_PLLLOOP, (uint8_t)0x0B, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_PLLCPI, (uint8_t)0x10, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PLLVCODIV, (uint8_t)0x24, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_XTALCAP, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_0xF00, (uint8_t)0x0F, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_0xF18, (uint8_t)0x06, ret_value);
}


/**
 * Sets AX5043 registers for RX. This is based on config file generated by radio lab.
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_set_regs_rx(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};
  ax5043_write_reg(spip, AX5043_REG_PLLLOOP, (uint8_t)0x0B, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_PLLCPI, (uint8_t)0x10, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PLLVCODIV, (uint8_t)0x25, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_XTALCAP, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_0xF00, (uint8_t)0x0F, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_0xF18, (uint8_t)0x02, ret_value);
}


/**
 * Sets AX5043 registers for RX continous. This is based on config file generated by radio lab.
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_set_regs_rxcont(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};
  ax5043_write_reg(spip, AX5043_REG_TMGRXAGC, (uint8_t)0x00, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_TMGRXPREAMBLE1, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTMISCFLAGS, (uint8_t)0x00, ret_value);
}


/**
 * Sets AX5043 registers for TX. This is based on config file generated by radio lab.
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_set_regs_rxcont_singleparamset(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};
  ax5043_write_reg(spip, AX5043_REG_RXPARAMSETS, (uint8_t)0xFF, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQDEV13, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FREQDEV03, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_AGCGAIN3, (uint8_t)0xB5, ret_value);
}

/**
 * Shutdown after reset the AX5043
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_shutdown(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};
  uint8_t value;
  value = ax5043_read_reg(spip, AX5043_REG_PWRMODE, (uint8_t)0x00, ret_value);
  value = value & 0xF0;
  value = value | AX5043_POWERDOWN;
  ax5043_write_reg(spip, AX5043_REG_PWRMODE, value, ret_value);
}


/**
 * Standby the AX5043
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_standby(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};
  //ax5043_write_reg(spip, AX5043_REG_PWRMODE, AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT | AX5043_POWERDOWN, ret_value);
  uint8_t value;
  value = ax5043_read_reg(spip, AX5043_REG_PWRMODE, (uint8_t)0x00, ret_value);
  value = value & 0xF0;
  value = value | AX5043_STANDBY;
  ax5043_write_reg(spip, AX5043_REG_PWRMODE, value, ret_value);
}

/**
 * Enable FIFO in AX5043
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_fifo_en(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};
  uint8_t value;
  value = ax5043_read_reg(spip, AX5043_REG_PWRMODE, (uint8_t)0x00, ret_value);
  value = value & 0xF0;
  value = value | AX5043_FIFO_ENABLED;
  ax5043_write_reg(spip, AX5043_REG_PWRMODE, value, ret_value);
}

/**
 * put AX5043 in synthesizer RX mode
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_synth_rx(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};

  uint8_t value;
  value = ax5043_read_reg(spip, AX5043_REG_PWRMODE, (uint8_t)0x00, ret_value);
  value = value & 0xF0;
  value = value | AX5043_SYNTH_RX;
  ax5043_write_reg(spip, AX5043_REG_PWRMODE, value, ret_value);
}

/**
 * changes AX5043 to full RX mode
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_full_rx(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};

  uint8_t value;
  value = ax5043_read_reg(spip, AX5043_REG_PWRMODE, (uint8_t)0x00, ret_value);
  value = value & 0xF0;
  value = value | AX5043_FULL_RX;
  ax5043_write_reg(spip, AX5043_REG_PWRMODE, value, ret_value);
}


/**
 * put AX5043 in synthesizer TX mode
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_synth_tx(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};
  uint8_t value;
  value = ax5043_read_reg(spip, AX5043_REG_PWRMODE, (uint8_t)0x00, ret_value);
  value = value & 0xF0;
  value = value | AX5043_SYNTH_TX;
  ax5043_write_reg(spip, AX5043_REG_PWRMODE, value, ret_value);
}


/**
 * changes AX5043 to full TX mode
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_full_tx(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};
  uint8_t value;
  value = ax5043_read_reg(spip, AX5043_REG_PWRMODE, (uint8_t)0x00, ret_value);
  value = value & 0xF0;
  value = value | AX5043_FULL_TX;
  ax5043_write_reg(spip, AX5043_REG_PWRMODE, value, ret_value);
}






/**
 * Resets the AX5043
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_reset(SPIDriver * spip)
{

  //uint8_t reg=0;
  uint8_t value = 0;
  uint8_t ret_value[3]={0,0,0};

  spiSelect(spip);
  chThdSleepMicroseconds(5);
  spiUnselect(spip);
  chThdSleepMicroseconds(5);
  spiSelect(spip);
  chThdSleepMicroseconds(5);
  //spiUnselect(spip);
  //chThdSleepMicroseconds(10);

  //Reset the chip through powermode register 
  ax5043_write_reg(spip, AX5043_REG_PWRMODE, AX5043_RESET_BIT, ret_value);
  chThdSleepMilliseconds(10);
  //chThdSleepMicroseconds(5);

  //read the powermode register
  value=ax5043_read_reg(&SPID2, AX5043_REG_PWRMODE, value, ret_value);
  //write to powermode register for enabling XOEN and REFIN and shutdown mode
  //page 33 in programming manual
  //value = value | AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT;
  value = AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT | AX5043_POWERDOWN;
  ax5043_write_reg(spip, AX5043_REG_PWRMODE, value, ret_value);
  chThdSleepMilliseconds(10);

  //ax5043_write_reg(spip, AX5043_REG_MODULATION, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_SCRATCH, (uint8_t)0x55, ret_value);
  value = ax5043_read_reg(&SPID2, AX5043_REG_SCRATCH, (uint8_t)0x00, ret_value);
  if (value != 0x55)
  {
        chprintf(DEBUG_CHP, "Scratch register does not match\r\n");
  }
  ax5043_write_reg(spip, AX5043_REG_SCRATCH, (uint8_t)0xAA, ret_value);
  value = ax5043_read_reg(spip, AX5043_REG_SCRATCH, (uint8_t)0x00, ret_value);
  if (value != 0xAA)
  {
        chprintf(DEBUG_CHP, "Scratch register does not match\r\n");
  }
  ax5043_write_reg(spip, AX5043_REG_PINFUNCIRQ, (uint8_t)0x02, ret_value);
  chThdSleepMilliseconds(1500);
  

}


/**
 * prepare AX5043 for tx
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_prepare_tx(SPIDriver * spip)
{

  //uint8_t reg=0;
  //uint8_t value=0;
  uint8_t ret_value[3]={0,0,0};

  ax5043_standby(spip);
  ax5043_fifo_en(spip);
  ax5043_set_regs_tx(spip);
  ax5043_write_reg(spip, AX5043_REG_FIFOTHRESH1, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFOTHRESH0, (uint8_t)0x80, ret_value);
  ax5043_write_reg(spip, AX5043_REG_IRQMASK0, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_IRQMASK1, (uint8_t)0x01, ret_value);
  ax5043_full_tx(spip);
}


/**
 * prepare AX5043 for rx
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_prepare_rx(SPIDriver * spip)
{

  //uint8_t reg=0;
  //uint8_t value=0;
  uint8_t ret_value[3]={0,0,0};

  ax5043_standby(spip);
  ax5043_fifo_en(spip);
  ax5043_set_regs_rx(spip);
  ax5043_write_reg(spip, AX5043_REG_FIFOTHRESH1, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFOTHRESH0, (uint8_t)0x80, ret_value);
  ax5043_write_reg(spip, AX5043_REG_IRQMASK0, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_IRQMASK1, (uint8_t)0x01, ret_value);
  ax5043_full_rx(spip);
}


/**
 * Initializes AX5043. This is written based on easyax5043.c generated from AX Radiolab
 * @param conf the AX5043 configuration handler
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_init(SPIDriver * spip)
{

  //uint8_t reg=0;
  uint8_t value=0;
  int num_retries;
  uint8_t pll_range_done = 0;
  uint8_t pll_range_after;
  uint8_t vcoi_save;
  uint8_t ret_value[3]={0,0,0};

  ax5043_reset(spip);         //reset is causing problems and hence disabling it.need to remove it
  
  //adding two more lines to diable the interrupts. Need to remove it later
  //ax5043_write_reg(spip, AX5043_REG_IRQMASK0, (uint8_t)0x00, ret_value);
  //ax5043_write_reg(spip, AX5043_REG_IRQMASK1, (uint8_t)0x00, ret_value);


  //ax5043_shutdown(spip);
  ax5043_set_regs(spip);
  //ax5043_write_reg(spip, AX5043_REG_PINFUNCIRQ, (uint8_t)0x03, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTSTOREFLAGS, (uint8_t)0x15, ret_value);
  ax5043_set_regs_tx(spip);

  //value = AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT | AX5043_POWERDOWN;
  ax5043_write_reg(spip, AX5043_REG_PLLLOOP, (uint8_t)0x09, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PLLCPI, (uint8_t)0x08, ret_value);

  ax5043_standby(spip);
  ax5043_write_reg(spip, AX5043_REG_MODULATION, (uint8_t)0x08, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FSKDEV2, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FSKDEV1, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FSKDEV0, (uint8_t)0x00, ret_value);

  //wait for Xtal
  num_retries=100;
  value=0;
  while (num_retries > 0 && (value & 0x01) != 0x01  )
  {
    chThdSleepMilliseconds(1);
    value=ax5043_read_reg(spip, AX5043_REG_XTALSTATUS, (uint8_t)0x00, ret_value); 
    num_retries--;
  }
  
  //set frequency based on line 693 on conig.c and 1640 on easyax5043.c from 
  //codeblocks generated code
  ax5043_write_reg(spip, AX5043_REG_FREQA0, (uint8_t)0xab, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQA1, (uint8_t)0xaa, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQA2, (uint8_t)0x12, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQA3, (uint8_t)0x09, ret_value); 


  //PLL autoranging
  ax5043_write_reg(spip, AX5043_REG_PLLRANGINGA, (uint8_t)0x1a, ret_value); 
  num_retries = 100; 
  pll_range_done = 0;
  while (num_retries > 0 && pll_range_done != 1)
  {
    chThdSleepMilliseconds(1);
    value = ax5043_read_reg(spip, AX5043_REG_PLLRANGINGA, (uint8_t)0x00, ret_value);
    if ((value & 0x08) == 0x00)
      pll_range_done = 1;
    num_retries--;
  }
  pll_range_after = ax5043_read_reg(spip, AX5043_REG_PLLRANGINGA, (uint8_t)0x00, ret_value); 
  chprintf(DEBUG_CHP, "\r\r PLL ranging done. %d --\r\n");


  //VCOI calibration
  /*
  ax5043_set_regs_tx(spip);
  ax5043_write_reg(spip, AX5043_REG_MODULATION, (uint8_t)0x08, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FSKDEV2, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FSKDEV1, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FSKDEV0, (uint8_t)0x00, ret_value);
  value=ax5043_read_reg(spip, AX5043_REG_PLLLOOP, (uint8_t)0x00, ret_value);
  value = value | 0x04;
  ax5043_write_reg(spip, AX5043_REG_PLLLOOP, value, ret_value);
  value=ax5043_read_reg(spip, AX5043_REG_0xF35, (uint8_t)0x00, ret_value);
  value = value | 0x80;
  if (2 & (uint8_t)~value)
    ++value;
  ax5043_write_reg(spip, AX5043_REG_0xF35, value, ret_value);
  ax5043_synth_tx(spip);
  vcoi_save = ax5043_read_reg(spip, AX5043_REG_PLLVCOI, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PLLRANGINGA, (uint8_t)(pll_range_after & 0x0F), ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQA0, (uint8_t)0xab, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQA1, (uint8_t)0xaa, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQA2, (uint8_t)0x12, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQA3, (uint8_t)0x09, ret_value); 
  //tune voltage. doesn't make sense. So, not implementing it for now..
  
  num_retries = 64;
  while (num_retries > 0)
  {
    ax5043_write_reg(spip, AX5043_REG_GPADCCTRL, (uint8_t)0x84, ret_value); 
    do{} while (ax5043_read_reg(spip, AX5043_REG_GPADCCTRL, (uint8_t)0x00, ret_value) & 0x80);
    num_retries--;
  }
  num_retries = 32;
  while (num_retries > 0)
  {
    ax5043_write_reg(spip, AX5043_REG_GPADCCTRL, (uint8_t)0x84, ret_value); 
    do{} while (ax5043_read_reg(spip, AX5043_REG_GPADCCTRL, (uint8_t)0x00, ret_value) & 0x80);
    num_retries--;
  }
  //ax5043_write_reg(spip, AX5043_REG_PLLVCOI, vcoi_save, ret_value);
  */

  ax5043_shutdown(spip);
  ax5043_set_regs(spip);
  ax5043_set_regs_rx(spip);
  //ax5043_write_reg(spip, AX5043_REG_PLLRANGINGA, (uint8_t)0x0A, ret_value); 

  ax5043_write_reg(spip, AX5043_REG_FREQA0, (uint8_t)0xab, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQA1, (uint8_t)0xaa, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQA2, (uint8_t)0x12, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQA3, (uint8_t)0x09, ret_value); 

  //ax5043_write_reg(spip, AX5043_REG_PLLRANGINGA, (uint8_t)(pll_range_after & 0x0F), ret_value); 

}



/**
 * Transmits a packet
 * @param conf the AX5043 configuration handler
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_transmit(SPIDriver * spip)
{

  //uint8_t reg=0;
  //uint8_t value=0;
  uint8_t ret_value[3]={0,0,0};
  int i=0;

  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)AX5043_DATA_CMD, ret_value);//The data follows
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x80, ret_value);//packet length
  //ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x24, ret_value);//packet details like raw packet
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x08, ret_value);//packet details like raw packet
  for (i=0;i<128;i++)
  {
      ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x55, ret_value);//some random data
  }
  ax5043_write_reg(spip, AX5043_REG_FIFOSTAT, (uint8_t)0x04, ret_value);//FIFO Commit  

}

/**
 * read FIFO packet for a packet
 * @param conf the AX5043 configuration handler
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_receive(SPIDriver * spip)
{

  //uint8_t reg=0;
  uint8_t value=0;
  uint8_t ret_value[3]={0,0,0};
  value=ax5043_read_reg(spip, AX5043_REG_FIFOSTAT, (uint8_t)0x00, ret_value);
  chprintf(DEBUG_CHP, "FIFO status value 0x%x \r\n", value);
  while(!(ax5043_read_reg(spip, AX5043_REG_FIFOSTAT, (uint8_t)0x00, ret_value)&0x01))
  {
    value=ax5043_read_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x00, ret_value);//some random data
    chprintf(DEBUG_CHP, "0x%x ,", value);
  }

  chprintf(DEBUG_CHP, "\r\r finished reading FIFO--\r\n", value);


}



//!@

