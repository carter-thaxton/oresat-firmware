/*! \file c3_fsm.c
 *
 * API for c3_fsm.c
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

#include "fsm.h"


//! function to update the state machine
/*!
  \param State machine structure state_machine_t.
  \param Next state from enum state_t.
  \return This does not return anything.
  \sa serviceStateMachine()
  This updates the current state of the C3. This is only called from 
  serviceStateMachine.
 */
static void stateUpdate(state_machine_t * fsm, state_t next_state){

  fsm->last_state = fsm->current_state;
  fsm->current_state = next_state;

}


//! function to update the state machine
/*!
  \param State machine structure.
  \return This does not return anything.
  This takes care of the event changes the state accordingly.
 */
void serviceStateMachine(state_machine_t * fsm){

  /* heart of state machine here */
  switch(fsm->current_state){

    case ejected:
    {
      /* check the thing that is responsible for initiating a state transition */
      if(fsm->attention == 30minTimer)
      {
        /* transition to next state */
        stateUpdate(fsm, deploy);
      }
      break;
    }

    case deploy:
    {
      /* check the thing that is responsible for initiating a state transition */
      if(!fsm->attention == CANCommand)
      {
        /* transition to next state */
        stateUpdate(fsm, sputnik);
      }
      break;
    }

    case sputnik:
    {

      /* check the thing that is responsible for initiating a state transition */
      if(fsm->attention == CANCommand)
      {
        /* transition to next state */
        stateUpdate(fsm, active);
      }

      break;

    }
    case default:
    {
      break;
    }
    

  }


}

//! @}
