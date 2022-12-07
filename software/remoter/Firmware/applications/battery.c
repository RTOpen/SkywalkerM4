#include <rtthread.h>
#include "battery.h"

/**
 * Loads each voltage value in its matching charge element (index)
 */
static const float batt_volts_array[101] = {
    3.200f, 
    3.250f, 3.300f, 3.350f, 3.400f, 3.450f,
    3.500f, 3.550f, 3.600f, 3.650f, 3.700f,
    3.703f, 3.706f, 3.710f, 3.713f, 3.716f,
    3.719f, 3.723f, 3.726f, 3.729f, 3.732f,
    3.735f, 3.739f, 3.742f, 3.745f, 3.748f,
    3.752f, 3.755f, 3.758f, 3.761f, 3.765f,
    3.768f, 3.771f, 3.774f, 3.777f, 3.781f,
    3.784f, 3.787f, 3.790f, 3.794f, 3.797f,
    3.800f, 3.805f, 3.811f, 3.816f, 3.821f,
    3.826f, 3.832f, 3.837f, 3.842f, 3.847f,
    3.853f, 3.858f, 3.863f, 3.868f, 3.874f,
    3.879f, 3.884f, 3.889f, 3.895f, 3.900f,
    3.906f, 3.911f, 3.917f, 3.922f, 3.928f,
    3.933f, 3.939f, 3.944f, 3.950f, 3.956f,
    3.961f, 3.967f, 3.972f, 3.978f, 3.983f,
    3.989f, 3.994f, 4.000f, 4.008f, 4.015f,
    4.023f, 4.031f, 4.038f, 4.046f, 4.054f,
    4.062f, 4.069f, 4.077f, 4.085f, 4.092f,
    4.100f, 4.111f, 4.122f, 4.133f, 4.144f,
    4.156f, 4.167f, 4.178f, 4.189f, 4.200f,
};

/**
 * Performs a binary search to find the index corresponding to a voltage.
 * The index of the array is the charge %
*/
rt_uint8_t battery_get_level(float volts){
  rt_uint8_t index = 50;
  rt_uint8_t prev = 0;
  rt_uint8_t half = 0;
  if (volts >= 4.2f){
    return 100;
  }
  if (volts <= 3.2f){
    return 0;
  }
  while(1){
    if(index >= prev)
    {
    half = (index - prev) / 2;
    }else
    {
    half = (prev - index) / 2;
    }
    prev = index;
    if(volts >= batt_volts_array[index]){
      index = index + half;
    }else{
      index = index - half;
    }
    if (prev == index){
      break;
    }
  }
  return index;
}
