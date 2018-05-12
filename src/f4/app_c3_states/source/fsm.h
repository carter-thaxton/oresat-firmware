/*! \file c3_fsm.h
 *
 * API for c3_fsm.h
 */

/*!
 * \defgroup c3_fsm
 *
 * @{
 */

//! State Machine for C3.
/*
 *
 *     Modification Log
 *     04/15/2018    William Harrington  Generic state machine template.  
 *     04/15/2018    Malay Das           Initial code to handle timer event.  
 *
 */
 

#ifndef _FSM_H
#define _FSM_H

#include <stdbool.h>
#include <stdint.h>


//! State machine enum.
/*! This currently handles 8 different states.*/
typedef enum{
  ejected,             /*!< Happens just after throwing out the sattelitte. */
  deploy,              /*!< The antennas are being deployed. */
  sputnik,             /*!< Sending CW (OOK), AX.25 and packet data in loop.*/
  active,              /*!< Processing instructions from ground. */
  ethertorque,         /*!< Orienting the sattelitte along with being active. */
  lowPower,            /*!< Sattelitte  has low battery or power. */
  noRadioTX,           /*!< No more radio transmissions. */
  needsRestart         /*!< Too many issues. The system is going to restart. */
}state_t;


//! attention list enum.
/*! This list all the events and activities that would require C3's attention.*/
typedef enum{
  30minTimer,          /*!< 30 minute passed since ejection */
  lowPower,            /*!< Sattelitte  has low battery or power. */
  radioCommand,        /*!< For state changes from Radio command. */
  CANCommand,          /*!< For state changes from CAN Command. */
}attention_t;


//! Structure to hold the current state and other parameters of the system
typedef struct{
  state_t current_state;
  state_t last_state; 
  attention_t attention;
  uint8_t shortMessage;
  uint8_t longMessage[32];
  uint8_t status;
}state_machine_t;

//! function to update the state machine
void serviceStateMachine(state_machine_t * fsm);

#endif

//! @}
