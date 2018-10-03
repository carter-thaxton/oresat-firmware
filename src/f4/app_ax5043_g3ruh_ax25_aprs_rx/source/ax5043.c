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





// physical layer from config.c file generated from radiolab
const uint8_t axradio_phy_pn9 = 0;
const uint8_t axradio_phy_nrchannels = 1;
const uint32_t axradio_phy_chanfreq[1] = { 0x0912aaab };
const uint8_t axradio_phy_chanpllrnginit[1] = { 0x0a };
const uint8_t axradio_phy_chanvcoiinit[1] = { 0x98 };
uint8_t axradio_phy_chanpllrng[1];
uint8_t axradio_phy_chanvcoi[1];
const uint8_t axradio_phy_vcocalib = 0;
const int32_t axradio_phy_maxfreqoffset = 913;
const int8_t axradio_phy_rssioffset = 64;
// axradio_phy_rssioffset is added to AX5043_RSSIREFERENCE and subtracted from chip RSSI value to prevent overflows (8bit RSSI only goes down to -128)
// axradio_phy_rssioffset is also added to AX5043_RSSIABSTHR
const int8_t axradio_phy_rssireference = (int8_t)(0xF7 + 64);
const int8_t axradio_phy_channelbusy = -93 + 64;
const uint16_t axradio_phy_cs_period = 7; // timer0 units, 10ms
const uint8_t axradio_phy_cs_enabled = 0;
const uint8_t axradio_phy_lbt_retries = 0;
const uint8_t axradio_phy_lbt_forcetx = 0;
const uint16_t axradio_phy_preamble_wor_longlen = 9; // wor_longlen + wor_len totals to 240.0ms plus 112bits
const uint16_t axradio_phy_preamble_wor_len = 40;
const uint16_t axradio_phy_preamble_longlen = 0;
const uint16_t axradio_phy_preamble_len = 40;
const uint8_t axradio_phy_preamble_byte = 0x55;
const uint8_t axradio_phy_preamble_flags = 0x18;
const uint8_t axradio_phy_preamble_appendbits = 4;
const uint8_t axradio_phy_preamble_appendpattern = 0x03;

//framing variables generated from radiolab
const uint8_t axradio_framing_maclen = 0;
const uint8_t axradio_framing_addrlen = 0;
const uint8_t axradio_framing_destaddrpos = 0;
const uint8_t axradio_framing_sourceaddrpos = 0xff;
const uint8_t axradio_framing_lenpos = 2;
const uint8_t axradio_framing_lenoffs = 37;
const uint8_t axradio_framing_lenmask = 0x00;
const uint8_t axradio_framing_swcrclen = 0;

const uint8_t axradio_framing_synclen = 32;
const uint8_t axradio_framing_syncword[] = { 0x33, 0x55, 0x33, 0x55};
const uint8_t axradio_framing_syncflags = 0x38;
const uint8_t axradio_framing_enable_sfdcallback = 0;

const uint32_t axradio_framing_ack_timeout = 13; // 98.9ms in wtimer0 units (640Hz)
const uint32_t axradio_framing_ack_delay = 313; // 1.0ms in wtimer1 units (20MHz/64)
const uint8_t axradio_framing_ack_retransmissions = 0;
const uint8_t axradio_framing_ack_seqnrpos = 0xff;

const uint8_t axradio_framing_minpayloadlen = 0; // must be set to 1 if the payload directly follows the destination address, and a CRC is configured
//WOR
const uint16_t axradio_wor_period = 128;

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
  //uint8_t return_val;
  //int num_retries = 1;
  //return_val=ax5043_write_reg_spi(spip, reg, value, ret_value);
  ax5043_write_reg_spi(spip, reg, value, ret_value);
  
/*
  while (num_retries > 0 && return_val != 0x80)
  {
    chThdSleepMicroseconds(100);
    return_val=ax5043_write_reg_spi(spip, reg, value, ret_value);
    num_retries--;
    //chprintf(DEBUG_CHP, "\r\r num_retries= %d --\r\n", num_retries);
  }*/

}

/**
 * Reds an AX5043 register. This has retry logic. This calls the the function ax5043_write_reg_spi.
 * @param spip: SPI Configuration, reg: Register address, value: register value, ret_value: returned data.
 * @return the value in the register.
 */
uint8_t ax5043_read_reg(SPIDriver * spip, uint16_t reg, uint8_t value, uint8_t ret_value[])
{
  uint8_t command_buf[3] = {0,0,0 };

  if(reg <  0x0070)
  {
    command_buf[0]=reg;
    command_buf[1]=value;
    spiSelect(spip);
    spiStartExchange(spip, 2, command_buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);
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
    return ret_value[2];    //return the reg value when reading the register
  }

}



/**
 * Sets AX5043 registers. This is based on config file generated by radio lab.
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
// TX: fcarrier=435.500MHz dev= 25.000kHz br=100.000kBit/s pwr= 10.0dBm
// RX: fcarrier=435.500MHz bw=150.000kHz br=100.000kBit/s
void ax5043_set_regs(SPIDriver * spip)
{
  uint8_t ret_value[3]={0,0,0};
  ax5043_write_reg(spip, AX5043_REG_MODULATION,     (uint8_t)0x08, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_ENCODING,       (uint8_t)0x07, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FRAMING,        (uint8_t)0x24, ret_value); 
  //ax5043_write_reg(spip, AX5043_REG_FEC,            (uint8_t)0x13, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_PINFUNCSYSCLK,  (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PINFUNCDCLK,    (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PINFUNCDATA,    (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PINFUNCANTSEL,  (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PINFUNCPWRAMP,  (uint8_t)0x07, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_WAKEUPXOEARLY,  (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_IFFREQ1,        (uint8_t)0x02, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_IFFREQ0,        (uint8_t)0x0C, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_DECIMATION,     (uint8_t)0x14, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_RXDATARATE2,    (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_RXDATARATE1,    (uint8_t)0x3E, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_RXDATARATE0,    (uint8_t)0x80, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MAXDROFFSET2,   (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MAXDROFFSET1,   (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MAXDROFFSET0,   (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MAXRFOFFSET2,   (uint8_t)0x80, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MAXRFOFFSET1,   (uint8_t)0x01, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MAXRFOFFSET0,   (uint8_t)0x30, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDMAX1,       (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDMAX0,       (uint8_t)0xA6, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDMIN1,       (uint8_t)0xFF, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDMIN0,       (uint8_t)0x5A, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AMPLFILTER,     (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_RXPARAMSETS,    (uint8_t)0xF4, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCGAIN0,       (uint8_t)0xC5, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_AGCTARGET0,     (uint8_t)0x84, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_TIMEGAIN0,      (uint8_t)0xF8, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_DRGAIN0,        (uint8_t)0xF2, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PHASEGAIN0,     (uint8_t)0xC3, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINA0,(uint8_t)0x0F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINB0,(uint8_t)0x1F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINC0,(uint8_t)0x0A, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAIND0,(uint8_t)0x0A, ret_value);
  ax5043_write_reg(spip, AX5043_REG_AMPLITUDEGAIN0, (uint8_t)0x06, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQDEV10,      (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQDEV00,      (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_BBOFFSRES0,     (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCGAIN1,       (uint8_t)0xC5, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCTARGET1,     (uint8_t)0x84, ret_value);
  ax5043_write_reg(spip, AX5043_REG_AGCAHYST1,      (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCMINMAX1,     (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TIMEGAIN1,      (uint8_t)0xF6, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_DRGAIN1,        (uint8_t)0xF1, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PHASEGAIN1,     (uint8_t)0xC3, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINA1,(uint8_t)0x0F, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINB1,(uint8_t)0x1F, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINC1,(uint8_t)0x0A, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAIND1,(uint8_t)0x0A, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AMPLITUDEGAIN1, (uint8_t)0x06, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQDEV11,      (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQDEV01,      (uint8_t)0x3C, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FOURFSK1,       (uint8_t)0x16, ret_value);
  ax5043_write_reg(spip, AX5043_REG_BBOFFSRES1,     (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_AGCGAIN3,       (uint8_t)0xFF, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCTARGET3,     (uint8_t)0x84, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCAHYST3,      (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AGCMINMAX3,     (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TIMEGAIN3,      (uint8_t)0xF5, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_DRGAIN3,        (uint8_t)0xF0, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PHASEGAIN3,     (uint8_t)0xC3, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINA3,(uint8_t)0x0F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINB3,(uint8_t)0x1F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAINC3,(uint8_t)0x0D, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQUENCYGAIND3,(uint8_t)0x0D, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_AMPLITUDEGAIN3, (uint8_t)0x06, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQDEV13,      (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FREQDEV03,      (uint8_t)0x3C, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FOURFSK3,       (uint8_t)0x16, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_BBOFFSRES3,     (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MODCFGF,        (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDEV2,        (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDEV1,        (uint8_t)0x04, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FSKDEV0,        (uint8_t)0x5E, ret_value);
  ax5043_write_reg(spip, AX5043_REG_MODCFGA,        (uint8_t)0x05, ret_value);
  ax5043_write_reg(spip, AX5043_REG_TXRATE2,        (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TXRATE1,        (uint8_t)0x0D, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TXRATE0,        (uint8_t)0x1B, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TXPWRCOEFFB1,   (uint8_t)0x07, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TXPWRCOEFFB0,   (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PLLVCOI,        (uint8_t)0x98, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PLLRNGCLK,      (uint8_t)0x05, ret_value);
  ax5043_write_reg(spip, AX5043_REG_BBTUNE,         (uint8_t)0x0F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_BBOFFSCAP,      (uint8_t)0x77, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PKTADDRCFG,     (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PKTLENCFG,      (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PKTLENOFFSET,   (uint8_t)0x25, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PKTMAXLEN,      (uint8_t)0xC8, ret_value);
  ax5043_write_reg(spip, AX5043_REG_MATCH0PAT3,     (uint8_t)0xAA, ret_value);
  ax5043_write_reg(spip, AX5043_REG_MATCH0PAT2,     (uint8_t)0xCC, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MATCH0PAT1,     (uint8_t)0xAA, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MATCH0PAT0,     (uint8_t)0xCC, ret_value);  
  //ax5043_write_reg(spip, AX5043_REG_MATCH0LEN,      (uint8_t)0x9F, ret_value);  
  //ax5043_write_reg(spip, AX5043_REG_MATCH0MAX,      (uint8_t)0x1F, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MATCH1PAT1,     (uint8_t)0x55, ret_value);
  ax5043_write_reg(spip, AX5043_REG_MATCH1PAT0,     (uint8_t)0x55, ret_value);
  ax5043_write_reg(spip, AX5043_REG_MATCH1LEN,      (uint8_t)0x0A, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_MATCH1MAX,      (uint8_t)0x0A, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TMGTXBOOST,     (uint8_t)0x5B, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TMGTXSETTLE,    (uint8_t)0x3E, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TMGRXBOOST,     (uint8_t)0x5B, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TMGRXSETTLE,    (uint8_t)0x3E, ret_value);
  ax5043_write_reg(spip, AX5043_REG_TMGRXOFFSACQ,   (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_TMGRXCOARSEAGC, (uint8_t)0x9C, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TMGRXRSSI,      (uint8_t)0x03, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_TMGRXPREAMBLE2, (uint8_t)0x17, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_RSSIABSTHR,     (uint8_t)0xE3, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_BGNDRSSITHR,    (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_PKTCHUNKSIZE,   (uint8_t)0x0D, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTACCEPTFLAGS, (uint8_t)0x20, ret_value); //original 20, 3f=accept all errors
  ax5043_write_reg(spip, AX5043_REG_DACVALUE1,      (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_DACVALUE0,      (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_DACCONFIG,      (uint8_t)0x00, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_REF,            (uint8_t)0x03, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_XTALOSC,        (uint8_t)0x04, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_XTALAMPL,       (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_0xF1C,          (uint8_t)0x07, ret_value);
  ax5043_write_reg(spip, AX5043_REG_0xF21,          (uint8_t)0x68, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_0xF22,          (uint8_t)0XFF, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_0xF23,          (uint8_t)0x84, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_0xF26,          (uint8_t)0x98, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_0xF34,          (uint8_t)0x28, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_0xF35,          (uint8_t)0x11, ret_value);
  ax5043_write_reg(spip, AX5043_REG_0xF44,          (uint8_t)0x25, ret_value);
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
  ax5043_write_reg(spip, AX5043_REG_PLLLOOP,        (uint8_t)0x09, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_PLLCPI,         (uint8_t)0x02, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PLLVCODIV,      (uint8_t)0x24, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_XTALCAP,        (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_0xF00,          (uint8_t)0x0F, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_0xF18,          (uint8_t)0x06, ret_value);
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
  ax5043_write_reg(spip, AX5043_REG_PLLLOOP,        (uint8_t)0x0B, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_PLLCPI,         (uint8_t)0x10, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PLLVCODIV,      (uint8_t)0x25, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_XTALCAP,        (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_0xF00,          (uint8_t)0x0F, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_0xF18,          (uint8_t)0x02, ret_value);
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
  ax5043_write_reg(spip, AX5043_REG_TMGRXAGC,       (uint8_t)0x00, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_TMGRXPREAMBLE1, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTMISCFLAGS,   (uint8_t)0x00, ret_value);
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
  ax5043_write_reg(spip, AX5043_REG_RXPARAMSETS,    (uint8_t)0xFF, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQDEV13,      (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FREQDEV03,      (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_AGCGAIN3,       (uint8_t)0xE7, ret_value);
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
 * switch off xtal for AX5043
 * @param spip: SPI Configuration.
 * @return void 
 * TODO return a -ve return code if there are any errors
 */
void ax5043_off_xtal(SPIDriver * spip)
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
  ax5043_write_reg(spip, AX5043_REG_PWRMODE, value, ret_value);
  chThdSleepMilliseconds(1);

  //ax5043_write_reg(spip, AX5043_REG_MODULATION, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_SCRATCH, (uint8_t)0xAA, ret_value);
  ax5043_write_reg(spip, AX5043_REG_SCRATCH, (uint8_t)0xAA, ret_value);
  value = ax5043_read_reg(&SPID2, AX5043_REG_SCRATCH, (uint8_t)0x00, ret_value);
  if (value != 0xAA)
  {
        chprintf(DEBUG_CHP, "Scratch register does not match 0\r\n");
  }
  ax5043_write_reg(spip, AX5043_REG_SCRATCH, (uint8_t)0x55, ret_value);
  ax5043_write_reg(spip, AX5043_REG_SCRATCH, (uint8_t)0x55, ret_value);
  chThdSleepMilliseconds(10);
  value = ax5043_read_reg(spip, AX5043_REG_SCRATCH, (uint8_t)0x00, ret_value);
  if (value != 0x55)
  {
        chprintf(DEBUG_CHP, "Scratch register does not match 1\r\n");
  }
  ax5043_write_reg(spip, AX5043_REG_PINFUNCIRQ, (uint8_t)0x02, ret_value);
  chThdSleepMilliseconds(10);
  

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
  ax5043_init_registers_common(spip);
  ax5043_write_reg(spip, AX5043_REG_FIFOTHRESH1, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFOTHRESH0, (uint8_t)0x80, ret_value);
  ax5043_write_reg(spip, AX5043_REG_IRQMASK0, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_IRQMASK1, (uint8_t)0x01, ret_value);

  //set address values
  ax5043_write_reg(spip, AX5043_REG_PKTADDR0, (uint8_t)0x32, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTADDR1, (uint8_t)0x34, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTADDR2, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTADDR3, (uint8_t)0x00, ret_value);
  //set address mask
  ax5043_write_reg(spip, AX5043_REG_PKTADDRMASK0, (uint8_t)0xFF, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTADDRMASK1, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTADDRMASK2, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTADDRMASK3, (uint8_t)0x00, ret_value);

  //wait for xtal
  while ((ax5043_read_reg(spip, AX5043_REG_XTALSTATUS, (uint8_t)0x00, ret_value) & 0x01) == 0) {
    chThdSleepMilliseconds(1);
  }

  //ax5043_full_tx(spip);
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
  //int8_t value= (int8_t)(0xF9 + 64);
  uint8_t ret_value[3]={0,0,0};

  ax5043_write_reg(spip, AX5043_REG_PWRMODE, AX5043_STANDBY, ret_value);
  ax5043_write_reg(spip, AX5043_REG_LPOSCCONFIG, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PWRMODE, AX5043_POWERDOWN, ret_value);

  ax5043_set_regs_rx(spip);
  ax5043_init_registers_common(spip);

  ax5043_write_reg(spip, AX5043_REG_RSSIREFERENCE, axradio_phy_rssireference, ret_value);
  ax5043_set_regs_rxcont(spip);
  
  ax5043_write_reg(spip, AX5043_REG_FIFOSTAT, (uint8_t)0x03, ret_value);//FIFO reset
  ax5043_write_reg(spip, AX5043_REG_PWRMODE, AX5043_FULL_RX, ret_value);//Full RX

  ax5043_write_reg(spip, AX5043_REG_FIFOTHRESH1, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFOTHRESH0, (uint8_t)0x80, ret_value);
  ax5043_write_reg(spip, AX5043_REG_IRQMASK0, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_IRQMASK1, (uint8_t)0x01, ret_value);
  
  //set address values
  ax5043_write_reg(spip, AX5043_REG_PKTADDR0, (uint8_t)0x33, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTADDR1, (uint8_t)0x34, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTADDR2, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTADDR3, (uint8_t)0x00, ret_value);
  //set address mask
  ax5043_write_reg(spip, AX5043_REG_PKTADDRMASK0, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTADDRMASK1, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTADDRMASK2, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PKTADDRMASK3, (uint8_t)0x00, ret_value);

}


void ax5043_init_registers_common(SPIDriver * spip)
{
    uint8_t ret_value[3]={0,0,0};
    uint8_t rng = axradio_phy_chanpllrng[0];
    if (rng & 0x20)
        chprintf(DEBUG_CHP, "\r\r ERROR at ax5043_init_registers_common --\r\n");
    if ( ax5043_read_reg(spip, AX5043_REG_PLLLOOP, (uint8_t)0x00, ret_value) & 0x80) {
        ax5043_write_reg(spip, AX5043_REG_PLLRANGINGB, (uint8_t)(rng & 0x0F), ret_value);
    } else {
       ax5043_write_reg(spip, AX5043_REG_PLLRANGINGA, (uint8_t)(rng & 0x0F), ret_value);
    }
    rng = axradio_get_pllvcoi(spip);
   if (rng & 0x80)
        ax5043_write_reg(spip, AX5043_REG_PLLVCOI, (uint8_t)(rng), ret_value);
}

 

uint8_t axradio_get_pllvcoi(SPIDriver * spip)
{
    uint8_t ret_value[3]={0,0,0};
    uint8_t x = axradio_phy_chanvcoiinit[0];
    if (x & 0x80) {
      if (!(axradio_phy_chanpllrnginit[0] & 0xF0)) {
        x += (axradio_phy_chanpllrng[0] & 0x0F) - (axradio_phy_chanpllrnginit[0] & 0x0F);
       x &= 0x3f;
        x |= 0x80;
      }
      return x;
    }
    return ax5043_read_reg(spip, AX5043_REG_PLLVCOI, (uint8_t)0x00, ret_value);
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
  //uint8_t value=0;
  //int num_retries;
  //uint8_t pll_range_done = 0;
  //uint8_t pll_range_after;
  //uint8_t vcoi_save;
  uint8_t ret_value[3]={0,0,0};

  ax5043_reset(spip);         //reset is causing problems and hence disabling it.need to remove it
  
  //adding two more lines to diable the interrupts. Need to remove it later
  //ax5043_write_reg(spip, AX5043_REG_IRQMASK0, (uint8_t)0x00, ret_value);
  //ax5043_write_reg(spip, AX5043_REG_IRQMASK1, (uint8_t)0x00, ret_value);


  //ax5043_shutdown(spip);
  ax5043_set_regs(spip);
  ax5043_set_regs_tx(spip);

  ax5043_write_reg(spip, AX5043_REG_PLLLOOP, (uint8_t)0x09, ret_value);
  ax5043_write_reg(spip, AX5043_REG_PLLCPI, (uint8_t)0x08, ret_value);

  ax5043_standby(spip);
  ax5043_write_reg(spip, AX5043_REG_MODULATION, (uint8_t)0x08, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FSKDEV2, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FSKDEV1, (uint8_t)0x00, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FSKDEV0, (uint8_t)0x00, ret_value);

  //wait for Xtal
  while ((ax5043_read_reg(spip, AX5043_REG_XTALSTATUS, (uint8_t)0x00, ret_value) & 0x01) == 0)
  {
    chThdSleepMilliseconds(1);
  }
  chprintf(DEBUG_CHP, "XTAL Status 0x%x \r\n", ax5043_read_reg(spip, AX5043_REG_XTALSTATUS, (uint8_t)0x00, ret_value));
  
  //set frequency based on line 693 on conig.c and 1640 on easyax5043.c from 
  //codeblocks generated code
  uint32_t f = axradio_phy_chanfreq[0];
  ax5043_write_reg(spip, AX5043_REG_FREQA0, (uint8_t)f, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQA1, (uint8_t)(f >> 8), ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQA2, (uint8_t)(f >> 16), ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQA3, (uint8_t)(f >> 24), ret_value); 


  //ax5043_write_reg(spip, AX5043_REG_FREQA0, (uint8_t)0xab, ret_value);  
  //ax5043_write_reg(spip, AX5043_REG_FREQA1, (uint8_t)0xaa, ret_value); 
  //ax5043_write_reg(spip, AX5043_REG_FREQA2, (uint8_t)0x12, ret_value); 
  //ax5043_write_reg(spip, AX5043_REG_FREQA3, (uint8_t)0x09, ret_value); 

  //PLL autoranging
  uint8_t r;
  if( !(axradio_phy_chanpllrnginit[0] & 0xF0) ) { // start values for ranging available
    r = axradio_phy_chanpllrnginit[0] | 0x10;
  }
  else {
    r = 0x18;
  }
  ax5043_write_reg(spip, AX5043_REG_PLLRANGINGA, (uint8_t)r, ret_value); 
  chThdSleepMilliseconds(1);
  while ((ax5043_read_reg(spip, AX5043_REG_PLLRANGINGA, (uint8_t)0x00, ret_value) & 0x10) != 0)
  {
    chThdSleepMilliseconds(1);
  }
  axradio_phy_chanpllrng[0] = ax5043_read_reg(spip, AX5043_REG_PLLRANGINGA, (uint8_t)0x00, ret_value); 
  chprintf(DEBUG_CHP, "\r\r PLL ranging done. 0x%x --\r\n", axradio_phy_chanpllrng[0]);


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

  ax5043_read_reg(spip, AX5043_REG_PLLRANGINGA, (uint8_t)(axradio_phy_chanpllrng[0] & 0x0F), ret_value);
  f = axradio_phy_chanfreq[0];
  ax5043_write_reg(spip, AX5043_REG_FREQA0, (uint8_t)f, ret_value);  
  ax5043_write_reg(spip, AX5043_REG_FREQA1, (uint8_t)(f >> 8), ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQA2, (uint8_t)(f >> 16), ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FREQA3, (uint8_t)(f >> 24), ret_value); 


}



/**
 * Transmits a packet. Not used in thsi version of code.
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
  ax5043_full_tx(spip);

  ax5043_write_reg(spip, AX5043_REG_FIFOSTAT, (uint8_t)0x03, ret_value);//FIFO reset

  //send out the preamble
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)(AX5043_REPEATDATA_CMD|0x02), ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x38, ret_value);//preamble flag
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0xE0, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x55, ret_value);//preamble


  //sync word
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0xA1, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x18, ret_value);//sync flags
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0xCC, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0xAA, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0xCC, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0xAA, ret_value);


/* seems this will not get executed based on logic
  //send out the preamble. again?
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)(AX5043_REPEATDATA_CMD|0x02), ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x38, ret_value);//preamble flag
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x1F, ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x55, ret_value);//preamble


  //writting preamble based on address register. pretty weird.
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x41), ret_value);
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x1C, ret_value);//preamble flag
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x55, ret_value);
*/

  //write MAC and packet
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)AX5043_DATA_CMD, ret_value);//The data follows
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x0C, ret_value);//packet length
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x03, ret_value);//packet details like raw packet


  //length
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x0B, ret_value);  
  //address values
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x33, ret_value); 
  ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x34, ret_value); 
  for (i=0;i<8;i++)
  {
      ax5043_write_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x36, ret_value);//some random data
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
  //ax5043_full_rx(spip);
  //ax5043_write_reg(spip, AX5043_REG_FIFOSTAT, (uint8_t)0x03, ret_value);//FIFO reset

  value=ax5043_read_reg(spip, AX5043_REG_FIFOSTAT, (uint8_t)0x00, ret_value);
  chprintf(DEBUG_CHP, "FIFO status value 0x%x \r\n", value);
  value=ax5043_read_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x00, ret_value);
  chprintf(DEBUG_CHP, "FIFO Data 0x%x \r\n", value);
  while(!(ax5043_read_reg(spip, AX5043_REG_FIFOSTAT, (uint8_t)0x00, ret_value)&0x01))
  {
    value=ax5043_read_reg(spip, AX5043_REG_FIFODATA, (uint8_t)0x00, ret_value);
    chprintf(DEBUG_CHP, "0x%x ", value);
    value=ax5043_read_reg(spip, AX5043_REG_FIFOSTAT, (uint8_t)0x00, ret_value);
    chprintf(DEBUG_CHP, "FIFO 0x%x \r\n", value);
  }

  chprintf(DEBUG_CHP, "\r\r finished reading FIFO--\r\n", value);


}



/**
 * writes to FIFO
 * Takn from Bradenburg library which seems to be taken from onSemi's code
 * @param conf the AX5043 configuration handler
 * @return void 
 * 
 */
void ax5043_writefifo(SPIDriver * spip,const uint8_t *ptr, uint8_t len)
{
    uint8_t ret_value[3]={0,0,0};
	if (!len)
		return;
	do {
		ax5043_write_reg(spip, AX5043_REG_FIFODATA, *ptr++, ret_value);
	} while (--len);
}

/**
 * Transmits a packet
 * Takn from Bradenburg library which seems to be taken from onSemi's code
 * @param conf the AX5043 configuration handler
 * @return void 
 * 
 */

void transmit_loop(SPIDriver * spip, axradio_trxstate_t axradio_trxstate, uint16_t axradio_txbuffer_len,uint8_t axradio_txbuffer[], uint16_t axradio_txbuffer_cnt)
{
    uint8_t ret_value[3]={0,0,0};

    for (;;) {
        uint8_t cnt = ax5043_read_reg(spip,AX5043_REG_FIFOFREE0, (uint8_t)0x00, ret_value);

        if (ax5043_read_reg(spip,AX5043_REG_FIFOFREE1, (uint8_t)0x00, ret_value))
            cnt = 0xff;

        switch (axradio_trxstate) {
        case trxstate_tx_longpreamble:
            if (!axradio_txbuffer_cnt) {
                axradio_trxstate = trxstate_tx_shortpreamble;
                axradio_txbuffer_cnt = axradio_phy_preamble_len;
                goto shortpreamble;
            }
            if (cnt < 4) {
                ax5043_write_reg(spip,AX5043_REG_FIFOSTAT, 4, ret_value); // commit
                chThdSleepMilliseconds(1);
                continue;
            }
            cnt = 7;
            if (axradio_txbuffer_cnt < 7)
                cnt = axradio_txbuffer_cnt;
            axradio_txbuffer_cnt -= cnt;
            cnt <<= 5;
            ax5043_write_reg(spip,AX5043_REG_FIFODATA, AX5043_FIFOCMD_REPEATDATA | (3 << 5), ret_value);
            ax5043_write_reg(spip,AX5043_REG_FIFODATA, axradio_phy_preamble_flags, ret_value);
            ax5043_write_reg(spip,AX5043_REG_FIFODATA, cnt, ret_value);
            ax5043_write_reg(spip,AX5043_REG_FIFODATA, axradio_phy_preamble_byte, ret_value);
            break;

        case trxstate_tx_shortpreamble:
        shortpreamble:
            if (!axradio_txbuffer_cnt) {
                if (cnt < 15) {
                    ax5043_write_reg(spip,AX5043_REG_FIFOSTAT, 4, ret_value); // commit
                    chThdSleepMilliseconds(1);
                    continue;
                }
                if (axradio_phy_preamble_appendbits) {
                    uint8_t byte;
                    ax5043_write_reg(spip,AX5043_REG_FIFODATA, AX5043_FIFOCMD_DATA | (2 << 5), ret_value);
                    ax5043_write_reg(spip,AX5043_REG_FIFODATA, 0x1C, ret_value);
                    byte = axradio_phy_preamble_appendpattern;
                    if (ax5043_read_reg(spip,AX5043_REG_PKTADDRCFG, (uint8_t)0x00, ret_value) & 0x80) {
                        // msb first -> stop bit below
                        byte &= 0xFF << (8-axradio_phy_preamble_appendbits);
                        byte |= 0x80 >> axradio_phy_preamble_appendbits;
                    } else {
                         // lsb first -> stop bit above
                        byte &= 0xFF >> (8-axradio_phy_preamble_appendbits);
                        byte |= 0x01 << axradio_phy_preamble_appendbits;
                    }
                    ax5043_write_reg(spip,AX5043_REG_FIFODATA, byte, ret_value);
                }
                if ((ax5043_read_reg(spip,AX5043_REG_FRAMING, (uint8_t)0x00, ret_value) & 0x0E) == 0x06 && axradio_framing_synclen) {
                    // write SYNC word if framing mode is raw_patternmatch, might use SYNCLEN > 0 as a criterion, but need to make sure SYNCLEN=0 for WMBUS (chip automatically sends SYNCWORD but matching in RX works via MATCH0PAT)
                    uint8_t len_byte = axradio_framing_synclen;
                    uint8_t i = (len_byte & 0x07) ? 0x04 : 0;
                    // SYNCLEN in bytes, rather than bits. Ceiled to next integer e.g. fractional bits are counted as full bits;v
                    len_byte += 7;
                    len_byte >>= 3;
                    ax5043_write_reg(spip,AX5043_REG_FIFODATA, AX5043_FIFOCMD_DATA | ((len_byte + 1) << 5), ret_value);
                    ax5043_write_reg(spip,AX5043_REG_FIFODATA, axradio_framing_syncflags | i, ret_value);
                    for (i = 0; i < len_byte; ++i) {
                        // better put a brace, it might prevent SDCC from optimizing away the assignement...
                        ax5043_write_reg(spip,AX5043_REG_FIFODATA, axradio_framing_syncword[i], ret_value);
                    }
                }
                axradio_trxstate = trxstate_tx_packet;
                continue;
            }
            if (cnt < 4) {
                ax5043_write_reg(spip,AX5043_REG_FIFOSTAT, 4, ret_value); // commit
                chThdSleepMilliseconds(1);
                continue;
            }
            cnt = 255;
            if (axradio_txbuffer_cnt < 255*8)
                cnt = axradio_txbuffer_cnt >> 3;
            if (cnt) {
                axradio_txbuffer_cnt -= ((uint16_t)cnt) << 3;
                ax5043_write_reg(spip,AX5043_REG_FIFODATA, AX5043_FIFOCMD_REPEATDATA | (3 << 5), ret_value);
                ax5043_write_reg(spip,AX5043_REG_FIFODATA, axradio_phy_preamble_flags, ret_value);
                ax5043_write_reg(spip,AX5043_REG_FIFODATA, cnt, ret_value);
                ax5043_write_reg(spip,AX5043_REG_FIFODATA, axradio_phy_preamble_byte, ret_value);
                continue;
            }
            {
                uint8_t byte = axradio_phy_preamble_byte;
                cnt = axradio_txbuffer_cnt;
                axradio_txbuffer_cnt = 0;
                ax5043_write_reg(spip,AX5043_REG_FIFODATA, AX5043_FIFOCMD_DATA | (2 << 5), ret_value);
                ax5043_write_reg(spip,AX5043_REG_FIFODATA, 0x1C, ret_value);
                if (ax5043_read_reg(spip,AX5043_REG_PKTADDRCFG, (uint8_t)0x00, ret_value) & 0x80) {
                    // msb first -> stop bit below
                    byte &= 0xFF << (8-cnt);
                    byte |= 0x80 >> cnt;
                } else {
                     // lsb first -> stop bit above
                    byte &= 0xFF >> (8-cnt);
                    byte |= 0x01 << cnt;
                }
                ax5043_write_reg(spip,AX5043_REG_FIFODATA, byte, ret_value);
            }
            continue;

        case trxstate_tx_packet:
            if (cnt < 11) {
                ax5043_write_reg(spip,AX5043_REG_FIFOSTAT, 4, ret_value); // commit
                chThdSleepMilliseconds(1);
                continue;
            }
            {
                uint8_t flags = 0;
                if (!axradio_txbuffer_cnt)
                    flags |= 0x01; // flag byte: pkt_start
                {
                    uint16_t len = axradio_txbuffer_len - axradio_txbuffer_cnt;
                    cnt -= 3;
                    if (cnt >= len) {
                        cnt = len;
                        flags |= 0x02; // flag byte: pkt_end
                    }
                }
                if (!cnt)
                    goto pktend;
                ax5043_write_reg(spip,AX5043_REG_FIFODATA, AX5043_FIFOCMD_DATA | (7 << 5), ret_value);
                ax5043_write_reg(spip,AX5043_REG_FIFODATA, cnt + 1, ret_value); // write FIFO chunk length byte (length includes the flag byte, thus the +1)
                ax5043_write_reg(spip,AX5043_REG_FIFODATA, flags, ret_value);
                ax5043_writefifo(spip,&axradio_txbuffer[axradio_txbuffer_cnt], cnt);
                axradio_txbuffer_cnt += cnt;
                if (flags & 0x02)
                    goto pktend;
            }
            break;

        default:
            chprintf(DEBUG_CHP, "ERROR: Unexpected state found in transmit_isr \r\n");
        }
    }

pktend:
    ax5043_write_reg(spip,AX5043_REG_RADIOEVENTMASK0, 0x01, ret_value); // enable REVRDONE event
    ax5043_write_reg(spip,AX5043_REG_FIFOSTAT, 4, ret_value); // commit
}





/**
 * Transmits a packet
 * Takn from Bradenburg library which seems to be taken from onSemi's code
 * @param conf the AX5043 configuration handler
 * @return void 
 * 
 */

uint8_t transmit_packet(SPIDriver * spip, const struct axradio_address *addr, const uint8_t *pkt, uint16_t pktlen) 
{
    uint8_t ret_value[3]={0,0,0};
	axradio_trxstate_t axradio_trxstate;
	uint16_t axradio_txbuffer_len;
	uint8_t axradio_txbuffer[PKTDATA_BUFLEN];
	struct axradio_address_mask axradio_localaddr;
	uint16_t axradio_txbuffer_cnt = 0;

    axradio_txbuffer_len = pktlen + axradio_framing_maclen;
    if (axradio_txbuffer_len > sizeof(axradio_txbuffer))
        return AXRADIO_ERR_INVALID;
    memset(axradio_txbuffer, 0, axradio_framing_maclen);
    memcpy(&axradio_txbuffer[axradio_framing_maclen], pkt, pktlen);
    if (axradio_framing_destaddrpos != 0xff)
        memcpy(&axradio_txbuffer[axradio_framing_destaddrpos], &addr->addr, axradio_framing_addrlen);
    if (axradio_framing_sourceaddrpos != 0xff)
        memcpy(&axradio_txbuffer[axradio_framing_sourceaddrpos], &axradio_localaddr.addr, axradio_framing_addrlen);
    if (axradio_framing_lenmask) {
        uint8_t len_byte = (uint8_t)(axradio_txbuffer_len - axradio_framing_lenoffs) & axradio_framing_lenmask; // if you prefer not counting the len byte itself, set LENOFFS = 1
        axradio_txbuffer[axradio_framing_lenpos] = (axradio_txbuffer[axradio_framing_lenpos] & (uint8_t)~axradio_framing_lenmask) | len_byte;
    }
/*
    if (axradio_framing_swcrclen)
        axradio_txbuffer_len = axradio_framing_append_crc(axradio_txbuffer, axradio_txbuffer_len);
    if (axradio_phy_pn9)
        pn9_buffer(axradio_txbuffer, axradio_txbuffer_len, 0x1ff, -(ax5043_read_reg(spip, AX5043_REG_ENCODING, (uint8_t)0x00, ret_value) & 0x01));
    axradio_txbuffer_cnt = axradio_phy_preamble_longlen;
*/
    ax5043_prepare_tx(spip);

    ax5043_read_reg(spip,AX5043_REG_RADIOEVENTREQ0, (uint8_t)0x00, ret_value);; // make sure REVRDONE bit is cleared, so it is a reliable indicator that the packet is out
    ax5043_write_reg(spip, AX5043_REG_FIFOSTAT, 3, ret_value); // clear FIFO data & flags (prevent transmitting anything left over in the FIFO, this has no effect if the FIFO is not powerered, in this case it is reset any way)
    axradio_trxstate = trxstate_tx_longpreamble;

    if ((ax5043_read_reg(spip,AX5043_REG_MODULATION, (uint8_t)0x00, ret_value) & 0x0F) == 9) { // 4-FSK
        ax5043_write_reg(spip,AX5043_REG_FIFODATA, AX5043_FIFOCMD_DATA | (7 << 5), ret_value);
        ax5043_write_reg(spip,AX5043_REG_FIFODATA, 2, ret_value);  // length (including flags)
        ax5043_write_reg(spip,AX5043_REG_FIFODATA, 0x01, ret_value);  // flag PKTSTART -> dibit sync
        ax5043_write_reg(spip,AX5043_REG_FIFODATA, 0x11, ret_value); // dummy byte for forcing dibit sync
    }
    transmit_loop(spip, axradio_trxstate, axradio_txbuffer_len, axradio_txbuffer, axradio_txbuffer_cnt);
    ax5043_write_reg(spip,AX5043_REG_PWRMODE, AX5043_FULL_TX, ret_value);

    ax5043_read_reg(spip,AX5043_REG_RADIOEVENTREQ0, (uint8_t)0x00, ret_value);
    //printf("INFO: Waiting for transmission to complete\n");
    while (ax5043_read_reg(spip,AX5043_REG_RADIOSTATE, (uint8_t)0x00, ret_value) != 0) {
        chThdSleepMilliseconds(1);
    }
    //printf("INFO: Transmission complete\n");

    ax5043_write_reg(spip,AX5043_REG_RADIOEVENTMASK0, 0x00, ret_value);

    return AXRADIO_ERR_NOERROR;
}





//!@

