/*
 * gps.h
 *
 *  Created on: 26.09.2021
 *      Author: Julius
 */

#include "stm32f4xx_hal.h"

enum GPS_Error {
	WRONG_MSG_SIZE = 1, NO_CALLBACK = 1 << 1,
};

typedef struct GPS_Data {
	float latitude;
	float longitue;
	float height;
	float speed;
	float heading; // Respect to North Pole
	float roc; // Rate of Climb
	float gdop; // Geometric (Time and Space), Position (3D) and Vertical Genauigkeit
	float pdop;
	float vdop;

	uint8_t nsat;
	uint8_t nstatus;
	uint8_t trackmode; //cold start 0 or correction applied 2

	float herror;
	float verror;
	float doffset; // Doppler Offset

	uint16_t date;
	uint16_t time;
	uint8_t sstatus; // System Status: Trying to Reach Navigation Status (needs 3 bzw 2 for 3D)

	uint8_t errors;

	float raw_time;
	uint8_t raw_prn[12];
	float raw_range[12];
	float raw_phase[12];
	float raw_doppler[12];
	float raw_cnr[12];
} GPS_Data;

void GPS_Init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_uart);
void GPS_USART_Callback(UART_HandleTypeDef *huart, uint16_t Size);
GPS_Data* GPS_Measure();

//struct GPSLogger {
//	USART_TypeDef huart;
//	uint32_t date;
//	void (*dataConsumer)(DATA_ID, uint32_t);
//};

