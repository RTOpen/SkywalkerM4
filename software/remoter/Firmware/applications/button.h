#ifndef __BUTTON_H_
#define __BUTTON_H_

#include "stdint.h"
#include "string.h"

//According to your need to modify the constants.
#define TICKS_INTERVAL    20	//ms
#define DEBOUNCE_TICKS    2	//MAX 8
#define SHORT_TICKS       (300 /TICKS_INTERVAL)
#define LONG_TICKS        (1500 /TICKS_INTERVAL)


typedef void (*BtnCallback)(void*);


typedef enum {
	PRESS_DOWN = 0,
	PRESS_UP,
	PRESS_REPEAT,
	SINGLE_CLICK,
	DOUBLE_CLICK,
	LONG_RRESS_START,
	LONG_PRESS_HOLD,
	number_of_event,
	NONE_PRESS
}PressEvent;

typedef struct {
	uint16_t ticks;
	uint8_t  repeat : 4;
	uint8_t  event : 4;
	uint8_t  state : 3;
	uint8_t  debounce_cnt : 3; 
	uint8_t  key_state : 2;
	uint8_t  key_id;
	BtnCallback  cb[number_of_event];
	void *next;
}button_t;


#ifdef __cplusplus  
extern "C" {  
#endif  

void button_init(button_t* btn, uint8_t key_id);
void button_attach(button_t* btn, PressEvent event, BtnCallback cb);
PressEvent get_button_event(button_t* btn);
int  button_start(button_t* btn);
void button_stop(button_t* btn);
void button_process(uint8_t state);

#ifdef __cplusplus
} 
#endif

#endif
