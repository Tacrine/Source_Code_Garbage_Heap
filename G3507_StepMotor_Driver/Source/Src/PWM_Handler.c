#include "ti_msp_dl_config.h"

#define PWM_CLK_FREQ    2000000

volatile uint8_t PWMIRQ = 0;
volatile uint8_t CAPTUREIRQ = 0;
volatile uint32_t pulseCount = 0;
volatile uint32_t targetPulseCount = 0;
volatile bool pwmEnabled = true;

volatile uint32_t Set_PWM_Period = 0;


void Set_Target_Pulse(uint32_t target)
{
    targetPulseCount = target;
    pulseCount = 0;
    pwmEnabled = true;
    DL_TimerA_startCounter(PWM_0_INST);
}

void Stop_PWM(void)
{
    pwmEnabled = false;
    DL_TimerA_stopCounter(PWM_0_INST);
}

void Set_PWM_Frequency(uint32_t freqHz)
{
    DL_TimerG_startCounter(CAPTURE_0_INST);
    if(freqHz > 0)
    {
        Set_PWM_Period = (PWM_CLK_FREQ / freqHz / 2);  // -1
        DL_TimerA_stopCounter(PWM_0_INST);
        DL_TimerA_setLoadValue(PWM_0_INST, Set_PWM_Period);
        DL_TimerA_setCaptureCompareValue(PWM_0_INST, Set_PWM_Period / 2, DL_TIMER_CC_0_INDEX);
        DL_TimerA_startCounter(PWM_0_INST);
    }
}

uint32_t Get_PWM_Frequency(void)
{
    uint32_t period = DL_TimerA_getLoadValue(PWM_0_INST);
    if(period > 0)
    {
        return PWM_CLK_FREQ / (period + 1) / 2;
    }
    return 0;
}

void PWM_0_INST_IRQHandler(void)
{
    uint32_t pending = DL_TimerA_getPendingInterrupt(PWM_0_INST);
    PWMIRQ = pending;
    switch (pending)
    {
        case DL_TIMERA_IIDX_ZERO:
            break;
        default:
            break;
    }
}

void CAPTURE_0_INST_IRQHandler(void)
{
    uint32_t pending = DL_TimerG_getPendingInterrupt(CAPTURE_0_INST);
    CAPTUREIRQ = pending;
    switch (pending)
    {
        case DL_TIMERG_IIDX_CC0_DN:
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