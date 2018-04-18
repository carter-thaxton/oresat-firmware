typedef enum{
  STATE0,
  STATE1,
  STATE2,
  //...STATEn
}state_t;

typedef struct{
  state_t current_state;
  state_t last_state; //helpful for debugging, not necessary tho
  //you might want to put other things in here too
  //example below
  bool some_important_flag;
}state_machine_t;


void serviceStateMachine(state_machine_t * fsm);
