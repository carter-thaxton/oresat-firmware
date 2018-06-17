#ifndef _BLDC_H_
#define _BLDC_H_

/**
 *
 *
 *
 *
 *
 */

#include "ch.h"
#include "hal.h"

#include <unistd.h>

/// TODO: openloop is going away
#define OPENLOOP /// open loop control (oxymoron)

//#define THREAD_SIZE	(1<<7)
/// TODO had to reduce this to compile with new ADC code
/// We should figure out what an actual good value is
#define THREAD_SIZE	(96)

/// serial debugging
#define DEBUG_SERIAL SD2
#define DEBUG_CHP ((BaseSequentialStream *) &DEBUG_SERIAL)


/**
 * TODO: the definitions of STEP, STRETCH, and SKIP
 * evolved over the course of various experiments. These
 * will completely change in v2.0.
 *
 * SCALE: Duty cycle scaling factor from 0-100 %
 *
 * STEPS: the number of discrete steps in the LUT
 *
 * STRETCH: 
 *
 * SKIP:
 *
 * STEP_SIZE: 
 *
 */
#define SCALE			100
#define STEPS			360 
#define STRETCH		1
#define SKIP      1
#define STEP_SIZE 100

/// encoder has 14 bits of precision
#define ENCODER_MAX 1<<14
/// chunk amount is the number of times through
/// the LUT for 1 revolution of the reaction wheel
#define CHUNK_AMOUNT 6
/// chunk size is the number
#define CHUNK_SIZE 2730

#define PWM_TIMER_FREQ	48e6 /// Hz
#define PWM_FREQ				15e3 /// periods per sec
#define PWM_PERIOD			PWM_TIMER_FREQ/PWM_FREQ 

#define PWM_U			0U
#define PWM_V			1U
#define PWM_W			2U

#define ADC_GRP_NUM_CHANNELS  1
#define ADC_GRP_BUF_DEPTH     8 

#define sinctrl_t uint16_t 

/**
 *
 *
 *
 */
typedef struct{
	uint16_t count,		// period counter
					 scale,		// scales the duty cycle
					 steps,		// number of steps in lut 
					 stretch,
					 skip;  
	sinctrl_t u,v,w,// signals
						phase_shift;		// 
  uint16_t current_sin_u, next_sin_u,
           current_sin_v, next_sin_v,
           current_sin_w, next_sin_w;
  uint8_t stretch_count;
  uint8_t sin_diff;
	uint16_t position;				// motor position from encoder
  bool openLoop;
					 sinctrl_t const *sinctrl; // pointer to the sin lut
	uint16_t spi_rxbuf[2]; // receive buffer
	thread_t *p_spi_thread;
  adcsample_t samples[ADC_GRP_NUM_CHANNELS * ADC_GRP_BUF_DEPTH]; // ADC conversion storage array
	int started;
} bldc;

/**
 *
 *
 *
 */
static const uint16_t chunk_low[6] = {
	0 * CHUNK_SIZE,
	1 * CHUNK_SIZE,
	2 * CHUNK_SIZE,
	3 * CHUNK_SIZE,
	4 * CHUNK_SIZE,
	5 * CHUNK_SIZE
};


/**
 *
 *
 *
 */
static const SPIConfig spicfg = {
	false,              // Enables circular buffer if == 1
	NULL,               // Operation complete call back.
	GPIOA,              // Chip select line
	GPIOA_SPI1_NSS,     // Chip select port
	SPI_CR1_BR_0|SPI_CR1_BR_1|SPI_CR1_BR_2|SPI_CR1_CPHA,//reg 1 mask
	SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2|SPI_CR2_DS_3,//reg 2 mask
};

extern THD_WORKING_AREA(wa_spiThread,THREAD_SIZE);
extern THD_FUNCTION(spiThread,arg);

extern void bldcInit(bldc *pbldc);
extern void bldcStart(bldc *pbldc);
extern void bldcStop(bldc *pbldc);
extern void bldcSetDC(uint8_t channel,uint16_t dc);
extern void bldcExit(bldc *pbldc);

#endif
