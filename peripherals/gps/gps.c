/*
 * gps.c
 *
 *  Created on: 26.09.2021
 *      Author: Julius
 */

#include "gps.h"
#include "string.h"
#include "gps_helper.h"

uint8_t watchdog_cntr;
static PhoenixReceiver phoenix_receiver;
static GPS_Data gps_data;

void GPS_Init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_uart) {
	phoenix_receiver.uartLane = huart;
	// Receive until expected amount of data is received or idle event occurs
	HAL_UARTEx_ReceiveToIdle_DMA(huart, RxBuf, RxBuf_SIZE);
	// Disable Half Transfer Interrupt
	__HAL_DMA_DISABLE_IT(hdma_uart, DMA_IT_HT);
	gps_data.errors = 0;
	watchdog_cntr = 0;
}

void GPS_USART_Callback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart == phoenix_receiver.uartLane) {
		switch (Size) {
		case f00_SIZE: // 5 Hz
			memcpy(f00Buf, RxBuf, Size);
			break;
		case f48_SIZE: // 0.1 Hz
			memcpy(f48Buf, RxBuf, Size);
			break;
		case f62_SIZE: // 1 Hz
			memcpy(f62Buf, RxBuf, Size);
			break;
		default:
			gps_data.errors = gps_data.errors & WRONG_MSG_SIZE;
		}
		HAL_UARTEx_ReceiveToIdle_DMA(phoenix_receiver.uartLane, RxBuf, RxBuf_SIZE);
		watchdog_cntr = 0;
	}
}

float f00_float(char *ascii_string, uint16_t pos, uint16_t size) {
	memcpy(ascii_string, &f00Buf[f00_cumsum[pos]], size);
	return atoff(ascii_string);
}

int f00_int(char *ascii_string, uint16_t pos, uint16_t size) {
	memcpy(ascii_string, &f00Buf[f00_cumsum[pos]], size);
	return atoi(ascii_string);
}

float f62(char *ascii_string, uint16_t pos1, uint16_t size, uint8_t cntr) {
	memcpy(ascii_string, &f62Buf[(int) (pos1 + 47 * cntr)], size);
	return atoff(ascii_string);
}

void parse_f00() {
	gps_data.latitude = f00_float(char_lat, POS_lat, SIZE_lat);
	gps_data.longitue = f00_float(char_long, POS_long, SIZE_long);
	gps_data.height = f00_float(char_height, POS_height, SIZE_height);
	gps_data.speed = f00_float(char_speed, POS_speed, SIZE_speed);
	gps_data.heading = f00_float(char_head, POS_head, SIZE_head);
	gps_data.roc = f00_float(char_roc, POS_roc, SIZE_roc);
	gps_data.gdop = f00_float(char_gdop, POS_gdop, SIZE_gdop);
	gps_data.pdop = f00_float(char_pdop, POS_pdop, SIZE_pdop);
	gps_data.vdop = f00_float(char_vdop, POS_vdop, SIZE_vdop);
	gps_data.nsat = (uint8_t) f00_int(char_nsat, POS_nsat, SIZE_nsat);
	gps_data.nstatus = (uint8_t) f00_int(char_nstatus, POS_nstatus, SIZE_nstatus);
	gps_data.trackmode = (uint8_t) f00_int(char_trackmode, POS_trackmode, SIZE_trackmode);
	gps_data.herror = f00_float(char_herror, POS_herror, SIZE_herror);
	gps_data.verror = f00_float(char_verror, POS_verror, SIZE_verror);
	gps_data.doffset = f00_float(char_doffset, POS_doffset, SIZE_doffset);
	gps_data.date = (uint16_t) f00_int(char_date, POS_date, SIZE_date);
	gps_data.time = (uint16_t) f00_int(char_time, POS_time, SIZE_time);
	gps_data.sstatus = (uint8_t) f00_int(char_sstatus, POS_sstatus, SIZE_sstatus);
}

void parse_f62() {
	gps_data.raw_time = f62(char_raw_time, POS_raw_time, SIZE_raw_time, 0);
	for (uint8_t i = 0; i < 12; i++) {

		gps_data.raw_cnr[i] = f62(char_cnrsat, POS_cnrsat_1, SIZE_cnrsat, i);
		gps_data.raw_doppler[i] = f62(char_raw_doppler, POS_raw_doppler_1, SIZE_raw_doppler, i);
		gps_data.raw_phase[i] = f62(char_raw_phase, POS_raw_phase_1, SIZE_raw_phase, i);
		gps_data.raw_range[i] = f62(char_raw_range, POS_raw_range_1, SIZE_raw_range, i);
		gps_data.raw_prn[i] = f62(char_raw_prn, POS_raw_prn_1, SIZE_raw_prn, i);

		// Clean char buffer
		for (uint8_t i = 0; i < 5; i++) {
			char_cnrsat[i] = '0';
		}
		for (uint8_t i = 0; i < 9; i++) {
			char_raw_doppler[i] = '0';
		}
		for (uint8_t i = 0; i < 14; i++) {
			char_raw_phase[i] = '0';
		}
		for (uint8_t i = 0; i < 13; i++) {
			char_raw_range[i] = '0';
		}
		for(uint8_t i=0; i<13; i++){
			char_raw_prn[i] = '0';
		}
	}
	// Clean F62 buffer
	for (uint16_t i = 0; i < 601; i++) {
		f62Buf[i] = 0;
	}
}

GPS_Data* GPS_Measure() {
	parse_f00();
	parse_f62();
	return &gps_data;
}

void watchdog() {
	// TODO: Implement Time cntr
	watchdog_cntr += 1;
	if (watchdog_cntr == 10) {
		HAL_UARTEx_ReceiveToIdle_DMA(phoenix_receiver.uartLane, RxBuf, RxBuf_SIZE);
		gps_data.errors = gps_data.errors & NO_CALLBACK;
	}
}
