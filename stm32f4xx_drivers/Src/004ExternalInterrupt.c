/*
* EXERCISE: Connect an external button to PD5 pin and toggle the led whenever interrupt
* is triggered by the button press.
* Interrupt should be triggered during falling edge of button press.
*
* Steps to follow while GPIO interrupt configuration:
*
* 1. Configure the GPIO pin as input pin and enable the internal pull-up or pull-down resistor if required.
* 2. Configure the interrupt pin for the required edge trigger (rising, falling or both).
* 3. Enable the interrupt delivery from the peripheral to the processor (i.e. unmask the interrupt).
* 4. Identify the IRQ number on which the processor accepts the interrupt from that pin.
* 5.Configure the IRQ priority for the identified IRQ number (Processor specific).
* 6. Enable Interrupt reception on that IRQ number(processor side).
* 7.Implement the IRQ handler.
*/
#include "stm32f407xx.h"
#include <string.h>

// Software delay
void soft_delay()
{
	//this will introduce ~200ms delay when system cloc is 16mhz
	for(uint32_t i = 0;i <500000/2;i++);
}

int main(void)
{
	GPIO_Handle_t GPIOBtn, GPIOLed;
	memset(&GPIOBtn , 0 ,sizeof(GPIOBtn)); // need to do this to prevent register corruption bugs.
	memset(&GPIOLed , 0 ,sizeof(GPIOLed));
	//BUTTON
	GPIOBtn.pGPIOx = GPIOD;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    //LED
	GPIOLed.pGPIOx = GPIOD;
	GPIOLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIOLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIOLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	// peripheral Clock starts
	GPIO_PeriClockControl(GPIOD,ENABLE);
	GPIO_Init(&GPIOLed);
	GPIO_Init(&GPIOBtn);

	//IRQ configuration
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5,NVIC_IRQ_PRI15 );
	GPIO_IRQ_IT_Config(IRQ_NO_EXTI9_5 , ENABLE);

	while(1);
}
void EXTI9_5_IRQHandler(void)// defined the already existing ISR in the startup file.
{
	soft_delay();//prevents multiple interrupt debounce error in one press.
	GPIO_IRQHandling(GPIO_PIN_NO_5);//calling user defined IRQ Handler. It just resets the pending register.
	GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
}
