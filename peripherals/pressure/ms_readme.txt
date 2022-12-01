---------------------------------
Activate SPI

----------------------------------

Please pass altitude in meter or contact your local library creator
----------------------------------

Example:

#include "MS5607.h"

ms5607_type ms5607 = constructMS5607(
 HAL_Delay,
 spi_write,
 spi_select_ms5607,
 spi_deselect_ms5607);
 
MS5607_Init(&ms5607);
PressureData p_data;
uint32_t current_altitude = gps_data.height;

while (1)
{
 MS5607_Measure(&ms5607, &p_data, current_altitude); // Every 0.1 seconds is good frequency.
}

 void spi_write(uint8_t *pTxData, uint8_t *pRxData, uint16_t size){
 HAL_StatusTypeDef ret = HAL_SPI_TransmitReceive(&hspi1, pTxData, pRxData, size, 20);
}