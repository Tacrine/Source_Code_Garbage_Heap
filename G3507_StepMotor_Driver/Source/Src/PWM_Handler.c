#include "ti_msp_dl_config.h"

#define PWM_CLK_FREQ    2000000

volatile uint8_t PWMIRQ = 0;
volatile uint8_t CAPTUREIRQ = 0;
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

void Set_PWM_Frequency(uint32_t freqHz)
{
    if(freqHz > 0)
    {
        uint32_t period = (PWM_CLK_FREQ / freqHz) - 1;
        DL_TimerG_setLoadValue(PWM_0_INST, period);
    }
}

uint32_t Get_PWM_Frequency(void)
{
    uint32_t period = DL_TimerG_getLoadValue(PWM_0_INST);
    if(period > 0)
    {
        return PWM_CLK_FREQ / (period + 1);
    }
    return 0;
}

void PWM_0_INST_IRQHandler(void)
{
    uint32_t pending = DL_TimerG_getPendingInterrupt(PWM_0_INST);
    PWMIRQ = pending;
    switch (pending)
    {
        case DL_TIMERG_IIDX_ZERO:
            break;
        default:
            break;
    }
}

void CAPTURE_0_INST_IRQHandler(void)
{
    volatile static uint8_t pending = 0;
    CAPTUREIRQ = pending;
    switch (DL_TimerA_getPendingInterrupt(CAPTURE_0_INST))
    {
        case DL_TIMERA_IIDX_CC0_DN:
            pulseCount++;
            if (targetPulseCount > 0 && pulseCount >= targetPulseCount)
            {
                Stop_PWM();
            }
            break;
            
        default:
            break;
    }
}