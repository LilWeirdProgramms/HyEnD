/*
 * MS5607.c
 *
 *  Created on: Jan 16, 2021
 *      Author: Julius
 */

#include "MS5607.h"
#include "Math.h"

ms5607_type constructMS5607(
		void (*delay)(uint32_t),
		void (*write)(uint8_t*, uint8_t*, uint16_t),
		void (*select)(void),
		void (*deselect)(void)) {
	ms5607_type ms5607;
	ms5607.DelayMs = delay;
	ms5607.Write = write;
	ms5607.Select = select;
	ms5607.Deselect = deselect;
	return ms5607;
}
s= 2
/* Performs a reading of the devices PROM. */
void prom_read(ms5607_type *ms5607) {
	promData_type promData;
	memset(&promData, 0, sizeof(promData_type));
	uint8_t address;
	uint8_t rxData[2];
	uint16_t promList[8];

	for (address = 0; address < 8; address++) {
		uint8_t txData = PROM_READ(address);
		ms5607->Select();
		ms5607->Write(&txData, rxData, 3);
		ms5607->Deselect();
		promList[address] = (rxData[0] << 8) | rxData[1];
		ms5607->DelayMs(10);
	}

	promData.reserved = promList[0];
	promData.sens = promList[1];
	promData.off = promList[2];
	promData.tcs = promList[3];
	promData.tco = promList[4];
	promData.tref = promList[5];
	promData.tempsens = promList[6];
	promData.crc = promList[7];
	ms5607->PROM = promData;
}

/** Reset and prepare for general usage.
 * This will reset the device and perform the PROM reading to find the conversion values and if
 * the communication is working.
 */
void MS5607_Init(ms5607_type *ms5607) {
	ms5607->tropo_init = 0;
	ms5607->strato_init = 0;
	uint8_t txData = RESET_COMMAND;
	uint8_t rxData[1];
	ms5607->Select();
	ms5607->Write(&txData, rxData, 1);
	ms5607->DelayMs(3);
	ms5607->Deselect();
	prom_read(ms5607);
	if (ms5607->PROM.reserved == 0x00 || ms5607->PROM.reserved == 0xff)
		ms5607->Error = 1;
}

uint32_t read_value(ms5607_type *ms5607, uint8_t read_from_register) {
	uint8_t txData1 = read_from_register;

	// Ready analog Value
	uint8_t rxData_trash[1];
	ms5607->Select();
	ms5607->Write(&txData1, rxData_trash, 1);
	ms5607->DelayMs(1);
	ms5607->Deselect();

	// Read from ADC
	uint8_t txData2 = READ_ADC_COMMAND;
	uint8_t rxData[3];
	ms5607->Select();
	ms5607->Write(&txData2, rxData, 4);
	ms5607->Deselect();
	return ((uint32_t) rxData[0] << 16) | ((uint32_t) rxData[1] << 8) | (uint32_t) rxData[2];
}

void read_uncomp(ms5607_type *ms5607) {
	UncompValues uncomp;
	uncomp.uncomp_pressure = read_value(ms5607, CONVERT_D1_COMMAND);
	uncomp.uncomp_temperature = read_value(ms5607, CONVERT_D2_COMMAND);
	ms5607->uncomp = uncomp;
}
void calculate_temperature(ms5607_type *ms5607, PressureData *data) {
	// Temperature from -4000 (=40°C) to 8500 = (85°C)
	ms5607->uncomp.dT = ms5607->uncomp.uncomp_temperature - ms5607->PROM.tref * 256.;
	data->temperature = 2000. + ms5607->uncomp.dT * ms5607->PROM.tempsens / 8388608.;
}
void calculate_pressure(ms5607_type *ms5607, PressureData *data) {
	// Pressure from 1000 (10 mBar = 25922 m Height) to 120000 (1200 mBar = 1.2 Bar)
	int32_t temp, temp2;
	int64_t off, off2, sens, sens2;
	temp = data->temperature;
	if (data->temperature > 20.) {
		temp2 = 0.;
		off2 = 0.;
		sens2 = 0.;
	}
	else {
		temp2 = ms5607->uncomp.dT * ms5607->uncomp.dT / 2147483648.;
		off2 = 61. * (data->temperature - 2000.) * (data->temperature - 2000.) / 16.;
		sens2 = 2. * (data->temperature - 2000.) * (data->temperature - 2000.);
		if (data->temperature < -15.) {
			off2 = off2 + 15. * (data->temperature + 1500.) * (data->temperature + 1500.);
			sens2 = sens2 + 8. * (data->temperature + 1500.) * (data->temperature + 1500.);
		}
	}
	off = ms5607->PROM.off * 131072. + ms5607->PROM.tco * ms5607->uncomp.dT / 64.;
	sens = ms5607->PROM.sens * 65536. + ms5607->PROM.tcs * ms5607->uncomp.dT / 128.;
	temp = temp - temp2;
	off = off - off2;
	sens = sens - sens2;
	data->pressure = (ms5607->uncomp.uncomp_pressure * sens / 2097152. - off) / 32768.;
}

// TODO: Put Hal get tick before and after and measure

float tropo_altitude(int32_t pressure, InitData *init_data) {
	// Standard Atmosphere from https://www.mide.com/air-pressure-at-altitude-calculator
	return (float) (init_data->h0 + (init_data->T0 / -0.0065) *
			(pow(pressure / init_data->p0, 0.19026) - 1.));
}

float strato_altitude(int32_t pressure, InitData *init_data) {
	return (float) init_data->h0 + (8.31432 * init_data->T0 *
			logf(pressure / init_data->p0)) / -0.284;
}

void initialize_altitude(ms5607_type *ms5607, PressureData *data, uint32_t current_altitude) {
	ms5607->init_data.T0 = data->temperature;
	ms5607->init_data.h0 = current_altitude;
	ms5607->init_data.p0 = data->pressure;
}

void calculate_altitude(ms5607_type *ms5607, PressureData *data, uint32_t current_altitude) {
	if (current_altitude < 11000) {
		if (!ms5607->tropo_init) {
			initialize_altitude(ms5607, data, current_altitude);
		}
		data->altitude = tropo_altitude(data->pressure, &ms5607->init_data);
	}
	else {
		if (!ms5607->strato_init) {
			initialize_altitude(ms5607, data, current_altitude);
		}
		data->altitude = strato_altitude(data->pressure, &ms5607->init_data);
	}
}

void MS5607_Measure(ms5607_type *ms5607, PressureData *data, uint32_t current_altitude) {
	// Current Altitude is at the start the GPS altitude (or zero) and than the filtered altitude
	// Please pass Altitude in meter (or dont I dont know)
	read_uncomp(ms5607);
	calculate_temperature(ms5607, data);
	calculate_pressure(ms5607, data);
	calculate_altitude(ms5607, data, current_altitude);
}
