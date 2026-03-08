#include "stm32f407xx.h"
#define HIGH ENABLE
#define BUTTON_PRESSED HIGH
// Software delay
void soft_delay()
{
	for(uint32_t i = 0;i < 999999;i++);
}
int main(void)
{
	GPIO_Handle_t GPIOLed;

	GPIOLed.pGPIOx = GPIOD;
	GPIOLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIOLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIOLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_PeriClockControl(GPIOD, ENABLE); //Enable peripheral clock.
  GPIO_Init(&GPIOLed);    //Initialize GPIO Pin.
  GPIO_Handle_t GPIOBtn;

	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_PeriClockControl(GPIOA, ENABLE); //Enable peripheral clock.
    GPIO_Init(&GPIOBtn);    //Initialize GPIO Pin.
    while(1)
    {
    	if(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == BUTTON_PRESSED)
      {
        soft_delay();//debounce time delay
        GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);
      }
    }
return 0;
}
