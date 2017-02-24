#include "driverlib.h"
#include "mymsp430.h"

void Timer_Init(void);


void main( void )
{
	/* Stop watchdog timer */
	WDT_A_hold(WDT_A_BASE);

	Clk_Using_DCO_Init(16000,4000,SMCLK_CLOCK_DIVIDER_2);
	Timer_Init();
        __enable_interrupt();
        
	while(1)
	{

	}
}


void Timer_Init(void)
{
	/* P1.2 and P1.3 as Capture/Compare Output */
	/* P1.2 and P1.3 are PWM output signal */
	/* PWM signal has frequency = 10Khz */
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1,GPIO_PIN2|GPIO_PIN3);

	/* Configure TimerA 0 run in up mode to generate PWM output signal */
	Timer_A_initUpModeParam para;
	para.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
	para.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_2;
	/* Fcpu = FMCLK = 16E6 Hz, F TimerA= F SMCLK/2 = (Fcpu/2)/2 = 4E6 Hz*/
	para.timerPeriod = 399; /* => PWM Period = (399+1)/4E6 = 1E-4 (s) => PWM Frequency = 1/1E-4 = 10 (KHz) */
	para.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
	para.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
	para.timerClear = TIMER_A_DO_CLEAR;
	para.startTimer = true;

	Timer_A_initUpMode(TIMER_A0_BASE,&para);



	//Initialize compare mode to generate PWM1
	Timer_A_initCompareModeParam initComp1Param;
	initComp1Param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
	initComp1Param.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
	initComp1Param.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
	initComp1Param.compareValue = 200;
	Timer_A_initCompareMode(TIMER_A0_BASE, &initComp1Param);

	//Initialize compare mode to generate PWM2
	Timer_A_initCompareModeParam initComp2Param;
	initComp2Param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
	initComp2Param.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
	initComp2Param.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
	initComp2Param.compareValue = 300;
	Timer_A_initCompareMode(TIMER_A0_BASE, &initComp2Param);
}
