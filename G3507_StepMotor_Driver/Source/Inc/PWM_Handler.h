#ifndef PWM_HANDLER_H
#define PWM_HANDLER_H

#include <stdint.h>
#include <stdbool.h>
#include "ti_msp_dl_config.h"

extern volatile uint32_t pulseCount;
extern volatile uint32_t targetPulseCount;
extern volatile bool pwmEnabled;

void Set_Target_Pulse(uint32_t target);
void Stop_PWM(void);
void Set_PWM_Frequency(uint32_t freqHz);
uint32_t Get_PWM_Frequency(void);

#endif