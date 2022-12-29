#include "button.h"

#define EVENT_CB(ev)   if(btn->cb[ev])btn->cb[ev]((button_t*)btn)
	
//button handle list head.
static button_t* btn_list = NULL;

/**
  * @brief  Initializes the button struct handle.
  * @param  handle: the button handle strcut.
  * @param  pin_level: read the HAL GPIO of the connet button level.
  * @param  active_level: pressed GPIO level.
  * @retval None
  */
void button_init(button_t* btn, uint8_t key_id)
{
	memset(btn, sizeof(button_t), 0);
	btn->event = (uint8_t)NONE_PRESS;
	btn->key_state = 0;
	btn->key_id = key_id;
}

/**
  * @brief  Attach the button event callback function.
  * @param  handle: the button handle strcut.
  * @param  event: trigger event type.
  * @param  cb: callback function.
  * @retval None
  */
void button_attach(button_t* btn, PressEvent event, BtnCallback cb)
{
	btn->cb[event] = cb;
}

/**
  * @brief  Inquire the button event happen.
  * @param  handle: the button handle strcut.
  * @retval button event.
  */
PressEvent get_button_event(button_t* btn)
{
	return (PressEvent)(btn->event);
}

/**
  * @brief  Button driver core function, driver state machine.
  * @param  handle: the button handle strcut.
  * @param  state: the buttons  state.
  * @retval None
  */
void button_handler(button_t* btn,uint8_t state)
{
	uint8_t key_state = (state&btn->key_id)?PRESS_DOWN:PRESS_UP;

	//ticks counter working..
	if((btn->state) > 0) btn->ticks++;

	/*------------button debounce handle---------------*/
	if(key_state != btn->key_state) { //not equal to prev one
		//continue read 3 times same new level change
		if(++(btn->debounce_cnt) >= DEBOUNCE_TICKS) {
			btn->key_state = key_state;
			btn->debounce_cnt = 0;
		}
	} else { //leved not change ,counter reset.
		btn->debounce_cnt = 0;
	}

	/*-----------------State machine-------------------*/
	switch (btn->state) {
	case 0:
		if(btn->key_state == PRESS_DOWN) {	//start press down
			btn->event = (uint8_t)PRESS_DOWN;
			EVENT_CB(PRESS_DOWN);
			btn->ticks = 0;
			btn->repeat = 1;
			btn->state = 1;
		} else {
			btn->event = (uint8_t)NONE_PRESS;
		}
		break;

	case 1:
		if(btn->key_state != PRESS_DOWN) { //released press up
			btn->event = (uint8_t)PRESS_UP;
			EVENT_CB(PRESS_UP);
			btn->ticks = 0;
			btn->state = 2;

		} else if(btn->ticks > LONG_TICKS) {
			btn->event = (uint8_t)LONG_RRESS_START;
			EVENT_CB(LONG_RRESS_START);
			btn->state = 5;
		}
		break;

	case 2:
		if(btn->key_state == PRESS_DOWN) { //press down again
			btn->event = (uint8_t)PRESS_DOWN;
			EVENT_CB(PRESS_DOWN);
			btn->repeat++;
			btn->ticks = 0;
			btn->state = 3;
		} else if(btn->ticks > SHORT_TICKS) { //released timeout
			if(btn->repeat == 1) {
				btn->event = (uint8_t)SINGLE_CLICK;
				EVENT_CB(SINGLE_CLICK);
			} else if(btn->repeat == 2) {
				btn->event = (uint8_t)DOUBLE_CLICK;
				EVENT_CB(DOUBLE_CLICK); // repeat hit
			}
			if(btn->repeat > 1) {
				btn->event = (uint8_t)PRESS_REPEAT;
				EVENT_CB(PRESS_REPEAT); // repeat hit
			} 
			btn->state = 0;
		}
		break;

	case 3:
		if(btn->key_state != PRESS_DOWN) { //released press up
			btn->event = (uint8_t)PRESS_UP;
			EVENT_CB(PRESS_UP);
			if(btn->ticks < SHORT_TICKS) {
				btn->ticks = 0;
				btn->state = 2; //repeat press
			} else {
				btn->state = 0;
				if(btn->repeat > 1) {
				if(btn->repeat == 2) {
				btn->event = (uint8_t)DOUBLE_CLICK;
				EVENT_CB(DOUBLE_CLICK); // repeat hit
		   	}
				btn->event = (uint8_t)PRESS_REPEAT;
				EVENT_CB(PRESS_REPEAT); // repeat hit
        } 
			}
		}
		break;

	case 5:
		if(btn->key_state == PRESS_DOWN) {
			//continue hold trigger
			btn->event = (uint8_t)LONG_PRESS_HOLD;
			EVENT_CB(LONG_PRESS_HOLD);

		} else { //releasd
			btn->event = (uint8_t)PRESS_UP;
			EVENT_CB(PRESS_UP);
			btn->state = 0; //reset
		}
		break;
	}
}

/**
  * @brief  Start the button work, add the handle into work list.
  * @param  handle: target handle strcut.
  * @retval 0: succeed. -1: already exist.
  */
int button_start(button_t* btn)
{
	button_t* target = btn_list;
	while(target) {
		if(target == btn) return -1;	//already exist.
		target = target->next;
	}
	btn->next = btn_list;
	btn_list = btn;
	return 0;
}

/**
  * @brief  Stop the button work, remove the handle off work list.
  * @param  handle: target handle strcut.
  * @retval None
  */
void button_stop(button_t* btn)
{
	button_t **curr;
	for(curr = &btn_list; *curr; ) {
		button_t* entry = *curr;
		if (entry == btn) {
			*curr = entry->next;
		} else
			curr = (button_t **)&entry->next;
	}
}

/**
  * @brief  background ticks, timer repeat invoking interval 5ms.
  * @param  uint8_t buttons state.
  * @retval None
  */
void button_process(uint8_t state)
{
	button_t* target;
	for(target=btn_list; target; target=target->next) {
		button_handler(target,state);
	}
}

