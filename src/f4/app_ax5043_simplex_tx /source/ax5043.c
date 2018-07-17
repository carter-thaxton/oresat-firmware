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
void ax5043_write_short_reg_8(SPIDriver * spip, uint8_t reg, uint8_t value, uint8_t ret_value[])
{
  uint8_t command_buf[2] = {0 ,0 };

  command_buf[0]=0x80|reg;
  command_buf[1]=value;
  chprintf(DEBUG_CHP, "\r\r written [0]=0x%x, [1]=0x%x--\r\n", command_buf[0],command_buf[1]);
  spiSelect(spip);
  //chThdSleepMilliseconds(1);
  spiStartExchange(spip, 2, command_buf, ret_value);
  //spiStartSend(spip, 2, command_buf);
  while((*spip).state != SPI_READY) { }
  spiUnselect(spip);
  chprintf(DEBUG_CHP, "\r\r read after writting [0]=0x%x, [1]=0x%x--\r\n", ret_value[0],ret_value[1]);

}


/**
 * reads 1 byte to a short register addressed in short format
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
void ax5043_read_short_reg_8(SPIDriver * spip, uint8_t reg, uint8_t value, uint8_t ret_value[])
{
  uint8_t command_buf[2] = {0 ,0 };
  //uint8_t rx_buf[2]={0 ,0 };

  command_buf[0]=reg;
  command_buf[1]=value;
  spiSelect(spip);
  spiStartExchange(spip, 2, command_buf, ret_value);
  while((*spip).state != SPI_READY) { }
  spiUnselect(spip);
  chprintf(DEBUG_CHP, "\r\r spi read reg=0x%x, value=0x%x ret=0x%x %x--\r\n", command_buf[0],command_buf[1], ret_value[0],ret_value[1]);
}




/**
 * Resets the AX5043
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
void ax5043_reset(SPIDriver * spip)
{

  uint8_t reg;
  uint8_t value;
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
  reg = AX5043_REG_PWRMODE;
  value =AX5043_RESET_BIT;
  ax5043_write_short_reg_8(&SPID2, reg, value, ret_value);
  //chprintf(DEBUG_CHP, "\r\r reg=0x%x, value=0x%x, ret_value=0x%x 0x%x, --\r\n", reg,value,ret_value[0],ret_value[1]);
  chThdSleepMilliseconds(500);

  //read the powermode register
  ax5043_read_short_reg_8(&SPID2, reg, value, ret_value);
  chThdSleepMilliseconds(500);

  //write to powermode register for enabling XOEN and REFIN
  //page 33 in programming manual
  value = ret_value[1] | AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT;
  ax5043_write_short_reg_8(&SPID2, reg, value, ret_value);
  chThdSleepMilliseconds(500);

  //set to shutdown powermode
  //page 33 in programming manual
  ax5043_read_short_reg_8(&SPID2, reg, value, ret_value);
  chThdSleepMilliseconds(500);

  value = ret_value[1] | AX5043_OSC_EN_BIT | AX5043_REF_EN_BIT | AX5043_POWERDOWN;
  ax5043_write_short_reg_8(&SPID2, reg, value, ret_value);
  //chprintf(DEBUG_CHP, "\r\r reg=0x%x, value=0x%x, ret_value=0x%x 0x%x, --\r\n", reg,value,ret_value[0],ret_value[1]);
  chThdSleepMilliseconds(100);
}





/**
 * Initialization routine for the AX5043 IC
 * @param conf the AX5043 configuration handler
 * @param spi the SPI handler
 * @param f_xtal the frequency of the crystal or the TCXO
 * @param vco the VCO mode
 * @param tx_complete_callback function to be called when a frame has been
 * transmitted. If none, set it to NULL
 * @return 0 on success or appropriate negative error code

int
ax5043_init (ax5043_conf_t *conf, SPI_HandleTypeDef *spi, uint32_t f_xtal,
             vco_mode_t vco,
             void (*tx_complete_callback)(ax5043_conf_t *))
{
  int ret;
  uint8_t revision;
  uint8_t val;

  if(!conf || !spi) {
    return -AX5043_INVALID_PARAM;
  }

  // Set the initial parameters 
  memset(conf, 0, sizeof(ax5043_conf_t));

  switch (vco) {
    case VCO_INTERNAL:
    case VCO_EXTERNAL:
      conf->vco = vco;
    break;
    default:
      return -AX5043_INVALID_PARAM;
  }

  conf->spi = spi;
  conf->rf_init = 0;
  conf->freqsel = -1;
  conf->f_xtal = f_xtal;
  if(conf->f_xtal > 24800000) {
    conf->f_xtaldiv = 2;
  }
  else {
    conf->f_xtaldiv = 1;
  }

  // Try first to read the revision register of the AX5043 
  ret = ax5043_spi_read_8(conf, &revision, AX5043_REG_REV);
  if(ret) {
    return ret;
  }

  if(revision != AX5043_REV) {
    return -AX5043_NOT_FOUND;
  }

  // To ensure communication try to write and read the scratch register 
  val = AX5043_SCRATCH_TEST;
  ret = ax5043_spi_write_8(conf, AX5043_REG_SCRATCH, val);
  if (ret) {
    return ret;
  }

  val = 0x0;
  ret = ax5043_spi_read_8(conf, &val, AX5043_REG_SCRATCH);
  if(ret) {
    return ret;
  }

  if(val != AX5043_SCRATCH_TEST) {
    return -AX5043_NOT_FOUND;
  }

  ret = ax5043_reset(conf);
  if(ret) {
    return ret;
  }

  ret = ax5043_set_pll_params (conf);
  if(ret) {
    return ret;
  }

  // Write the performance register F35 based on the XTAL frequency 
  if(conf->f_xtaldiv == 1) {
    ret = ax5043_spi_write_8(conf, 0xF35, 0x10);
  }
  else{
    ret = ax5043_spi_write_8(conf, 0xF35, 0x11);
  }
  if(ret) {
    return ret;
  }

  // FIFO maximum chunk 
  ret = ax5043_spi_write_8(conf, AX5043_REG_PKTCHUNKSIZE,
                           AX5043_PKTCHUNKSIZE_240);
  if(ret) {
    return ret;
  }

  // Set RF parameters 
  ret = ax5043_freqsel(conf, FREQA_MODE);
  if(ret) {
    return ret;
  }


   // We use APRS for all transmitted frames. APRS is encapsulated in a
   // AX.25 frame. For 9600 baudrate is FSK9600 G3RUH compatible modem
  ret = ax5043_aprs_framing_setup(conf);
  if(ret) {
    return ret;
  }

  // Setup TX only related parameters 
  ret = ax5043_conf_tx_path (conf);
  if(ret) {
    return ret;
  }

  // Set an internal copy for the IRQ handler 
  __ax5043_conf = conf;

  // Set the TX complete callback internal copy 
  __tx_complete_callback = tx_complete_callback;


   // Set the FIFO IRQ threshold. During TX, when the free space is larger
   // than this threshold, an IRQ is raised

  ret = ax5043_spi_write_16(conf, AX5043_REG_FIFOTHRESH1, AX5043_FIFO_FREE_THR);
  ret = ax5043_spi_write_16(conf, AX5043_REG_IRQMASK1, AX5043_IRQMRADIOCTRL);
  ret = ax5043_spi_write_8(conf, AX5043_REG_RADIOEVENTMASK0, AX5043_REVMDONE);
  if(ret) {
    return ret;
  }
  return AX5043_OK;
}
*/




