/*
 * MS5607.h
 *
 *  Created on: Jun 08, 2021
 *      Author: Julius
 */

#include "drivers/hyend_os_drivers.h"

// DEFINES:
/* MS5607 SPI COMMANDS */
#define RESET_COMMAND                 0x1E
#define PROM_READ(address)            (0xA0 | ((address) << 1))
#define CONVERT_D1_COMMAND            0x40 // Minimum conversion time, minimum precision
// maximum fidelity would be 0x46
#define CONVERT_D2_COMMAND            0x50
#define READ_ADC_COMMAND              0x00

// Structs:

/* MS5607 PROM Data Structure */
typedef struct {
  uint16_t reserved;
  uint16_t sens;
  uint16_t off;
  uint16_t tcs;
  uint16_t tco;
  uint16_t tref;
  uint16_t tempsens;
  uint16_t crc;
} promData_type;

/* MS5607 Pressure Data Structure */
typedef struct {
  int32_t pressure;
  int32_t temperature;
  uint32_t altitude;
} PressureData;

typedef struct {
  uint32_t uncomp_pressure;
  uint32_t uncomp_temperature;
  int32_t dT;
} UncompValues;

typedef struct InitData{
    float p0;
    float h0;
    float T0;
} InitData;

/* MS5607 Data Structure */
typedef struct {
	void (*DelayMs)(uint32_t delayMS);
	void (*Write)(uint8_t* pTxData, uint8_t* pRxData, uint16_t size);
	void (*Power)(void);
	void (*Select)(void);
	void (*Deselect)(void);
	promData_type PROM;
	UncompValues uncomp;
	uint8_t Error;
	uint8_t tropo_init;
	uint8_t strato_init;
	InitData init_data;
} ms5607_type;

// Prototypes:
ms5607_type constructMS5607(
	void (*delay)(uint32_t),
	void (*write)(uint8_t*, uint8_t*, uint16_t),
	void (*select)(void),
	void (*deselect)(void)
);

void MS5607_Init(ms5607_type * ms5607);
void MS5607_Measure(ms5607_type *ms5607, PressureData *data, uint32_t current_altitude);
