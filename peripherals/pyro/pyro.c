/*
 * pyro.c
 *
 *  Created on: 16.11.2022
 *      Author: Julius
 *
 *Activate GPIO EXTI at GPIO pin with triggering at rising and falling.
 *
 *  Put into:
 *      void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	Pyro_Measure(GPIOA, GPIO_PIN_9, GPIO_Pin, &pyro_status)
}


 */
#include "drivers/hyend_os_drivers.h"

void Pyro_Measure(GPIO_TypeDef* GPIO_Pyro_port, uint16_t GPIO_Pyro_Pin, uint16_t GPIO_Interrupt_Pin, uint8_t *pyro_status){
	// pyro_status = low means that the pyro is currently firering or already fired. High means continuity and not firering.
	//
    if(GPIO_Interrupt_Pin == GPIO_Pyro_Pin)
    {
    	*pyro_status = HAL_GPIO_ReadPin(GPIO_Pyro_port, GPIO_Pyro_Pin);
    }
}

