#include "fsm.h"

#define PRIVATE static

/* make a function for this cause you will be doing it a lot */
PRIVATE void stateUpdate(state_machine_t * fsm, state_t next_state){

  fsm->last_state = fsm->current_state;
  fsm->current_state = next_state;

}

void serviceStateMachine(state_machine_t * fsm){

  /* heart of state machine here */
  switch(fsm->current_state){

    case STATE0:
    {

      /* check the thing that is responsible for initiating a state transition */
      if(some_important_flag)
      {

        /* transition to next state */
        stateUpdate(fsm, STATE1);

      }

      break;

    }

    case STATE1:
    {

      /* check the thing that is responsible for initiating a state transition */
      if(!some_important_flag)
      {

        /* transition to next state */
        stateUpdate(fsm, STATE2);

      }

      break;

    }

    case STATE2:
    {

      /* check the thing that is responsible for initiating a state transition */
      if(some_important_flag)
      {

        /* transition to next state */
        stateUpdate(fsm, STATE0);

      }

      break;

    }

  }
  /*********************/

}
