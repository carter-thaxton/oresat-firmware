#ifndef _MAGNETORQUER_H_
#define _MAGNETORQUER_H_

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#define MTQR_PWM_TIMER_FREQ  	2e6	/// PWM Timer frequency
#define MTQR_PWM_FREQ 				2e3 /// periods per second

#define FORWARD 0
#define REVERSE 1

#define MTQR_PWM_PERIOD MTQR_PWM_TIMER_FREQ/MTQR_PWM_FREQ

#define PWM_CH_MTQR      			0   /// pwm channel
#define MTQR_STARTING_DC			5000		/// starting duty cycle
#define PH				 						9u	/// Phase (motor direction)
//#define PH				 						6u	/// Phase (motor direction)
#define ENABLE    						3u	/// Chip enable
//#define ENABLE    						7u	/// Chip enable
																	/// Logic low

#define EXIT_SUCCESS 					0
#define EXIT_FAILURE 					1

/**
 *
 *
 *
 *
 *
 * 
 */

typedef struct{
	uint8_t direction;
	int started;
	uint16_t pwm_dc; /// duty cycle
}MTQR;

extern void mtqrInit(MTQR *mtqr);
extern void mtqrStart(MTQR *mtqr);
extern void mtqrStop(MTQR *mtqr);
extern void mtqrSetDC(uint16_t dc);
extern void mtqrSetDir(uint8_t dc);
extern void mtqrExit(MTQR *mtqr);

	
#endif
