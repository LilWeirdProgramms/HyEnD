WARNING:

-> Software fires callbacks on GPS receivers UART idle line detection (signalling full message received). Plz dont kill this callback.
-> Includes "string.h" Library

---------------------------------
Activate UART and DMA_Rx

---------------------------------
Implement:

- Init:
GPS_Init()

- While:
GPS_Measure()
-> Call (at least) every 0.2 seconds

- HAL_UARTEx_RxEventCallback:
GPS_USART_Callback()

----------------------------------
Appendix:
Raw Data is updated with 1 Hz, Processed Data with 5 Hz

----------------------------------
Example:

#include "gps.h"

GPS_Data gps_data;
GPS_Init(&huart6, &hdma_usart6_rx);

while (1)
{
  gps_data = GPS_Measure();
}
  
/* USER CODE BEGIN 4 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	GPS_USART_Callback(huart, Size);
}

Verify correct implementation by monitoring gps_data.raw_time stepping up once per second