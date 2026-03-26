#include "ti_msp_dl_config.h"

uint8_t PWMIRQ = 0;
uint8_t CAPTUREIRQ = 0;
volatile uint32_t pulseCount = 0;
volatile uint32_t targetPulseCount = 0;
volatile bool pwmEnabled = true;

void Set_Target_Pulse(uint32_t target)
{
    targetPulseCount = target;
    pulseCount = 0;
    pwmEnabled = true;
    DL_TimerG_startCounter(PWM_0_INST);
}

void Stop_PWM(void)
{
    pwmEnabled = false;
    DL_TimerG_stopCounter(PWM_0_INST);
}

void PWM_0_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(PWM_0_INST))
    {
        case DL_TIMERG_IIDX_ZERO:
            PWMIRQ = 1;
            break;
        default:
            break;
    }
}

void CAPTURE_0_INST_IRQHandler(void)
{
    switch (DL_TimerA_getPendingInterrupt(CAPTURE_0_INST))
    {
        case DL_TIMERA_IIDX_CC0_DN:
            CAPTUREIRQ = 1;
            if(pwmEnabled)
            {
                pulseCount++;
                if(targetPulseCount > 0 && pulseCount >= targetPulseCount)
                {
                    Stop_PWM();
                }
            }
            break;
        default:
            break;
    }
}  



