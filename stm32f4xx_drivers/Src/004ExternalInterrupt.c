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
int main(void)
{

}
void EXTI0_IRQHandler(void)// defined the already existing ISR in the startup file.
{
	GPIO_IRQHandling();//calling user defined IRQ Handler.
}
