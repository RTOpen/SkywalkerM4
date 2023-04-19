#ifndef __THREADS_H
#define __THREADS_H

#ifdef __FREERTOS__
#include "freertos.h"
#include "task.h"
#include "semphr.h"
#endif
#ifdef __RTTHREAD__
#include <rtthread.h>
#endif

#define LED_THREAD_STACK_SIZE        (512)

#define LED_THREAD_PRIORITY          (5)

#define RADIO_THREAD_STACK_SIZE        (2048)

#define RADIO_THREAD_PRIORITY          (3)

int led_thread_init(void);

int radio_thread_init(void);

#endif /* __THREADS_H */
