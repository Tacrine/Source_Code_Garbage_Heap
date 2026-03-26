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

#endif