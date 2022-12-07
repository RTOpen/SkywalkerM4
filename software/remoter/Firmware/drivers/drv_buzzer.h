#ifndef __DRV_BUZZER_H
#define __DRV_BUZZER_H
#include "CH58x_common.h"


void buzzer_hw_init(void);

void buzzer_hw_deinit(void);

void buzzer_set_volume(uint8_t volume);

void buzzer_set_freq(uint16_t freq);

void buzzer_start(void);

void buzzer_stop(void);

#endif /* __DRV_BUZZER_H */
