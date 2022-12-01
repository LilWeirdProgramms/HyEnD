#include <stdlib.h>

#define RxBuf_SIZE 601
#define f00_SIZE 131
#define f48_SIZE 48
#define f62_SIZE 601

// F00:

#define POS_lat 2
#define SIZE_lat 10
#define POS_long 3
#define SIZE_long 11
#define POS_height 4
#define SIZE_height 11
#define POS_speed 5
#define SIZE_speed 9
#define POS_head 6
#define SIZE_head 5
#define POS_roc 7
#define SIZE_roc 9
#define POS_gdop 8
#define SIZE_gdop 5
#define POS_pdop 9
#define SIZE_pdop 5
#define POS_vdop 10
#define SIZE_vdop 5
#define POS_nsat 11
#define SIZE_nsat 2
#define POS_nstatus 12
#define SIZE_nstatus 1
#define POS_trackmode 13
#define SIZE_trackmode 1
#define POS_herror 14
#define SIZE_herror 11
#define POS_verror 15
#define SIZE_verror 10
#define POS_doffset 16
#define SIZE_doffset 10
#define POS_date 17
#define SIZE_date 6
#define POS_time 18
#define SIZE_time 6
#define POS_sstatus 20
#define SIZE_sstatus 1

#define F00_num_entries 24

char char_lat[SIZE_lat];
char char_long[SIZE_long];
char char_height[SIZE_height];
char char_speed[SIZE_speed];
// Heading : In regards to north pole in degree (0° is north) (WGS84)
char char_head[SIZE_head];
// Rate of Climb
char char_roc[SIZE_roc];
// G P V DOP, Geometric Genauigkeit (3D und Zeit), Positional DOP (3D Positionsgenauigkeit), Vertical Genauigkeit
char char_gdop[SIZE_gdop];
char char_pdop[SIZE_pdop];
char char_vdop[SIZE_vdop];
char char_nsat[SIZE_nsat];
// Navigation Status
char char_nstatus[SIZE_nstatus];

char char_trackmode[SIZE_trackmode];
char char_herror[SIZE_herror];
char char_verror[SIZE_verror];
char char_doffset[SIZE_doffset];
char char_date[SIZE_date];
char char_time[SIZE_time];
// System Status (Tryinig to reach certain Navigation Status (trying to reach 3D Nav))
char char_sstatus[SIZE_sstatus];
uint8_t RxBuf[RxBuf_SIZE];
char f00Buf[f00_SIZE];
char f48Buf[f48_SIZE];
char f62Buf[f62_SIZE];

typedef struct PhoenixReceiver
{
	UART_HandleTypeDef *uartLane;
	void (*DelayMs)(uint32_t delayMs);
} PhoenixReceiver;

uint16_t f00_cumsum[F00_num_entries] = {0, 1, 4, 14, 25, 36, 45, 50, 59, 64, 69, 74,
		76, 77, 78, 89, 99, 109, 115, 121, 122, 123, 128, 130};

void calc_f00_cumsum(){
	uint16_t f00_structure[F00_num_entries] = {1, 3, 10, 11, 11, 9, 5, 9, 5, 5, 5, 2, 1,
			1, 11, 10, 10, 6, 6, 1, 1, 5, 2, 1};
	for (uint8_t i = 0; i < F00_num_entries; i++) {
	    f00_cumsum[i] = 0;
	}
	for (uint8_t i = 1; i < F00_num_entries; i++) {
	    f00_cumsum[i] = f00_structure[i - 1] + f00_cumsum[i - 1];
	}
}

char test_char1[7] = {'+', '0', '1', '.', '0', '0', '0'};
char test_char2[7] = {'+', '2', '1', '.', '1', '3', '0'};
char test_char3[7] = {'-', '0', '1', '.', '1', '3', '0'};

void test_parsing(){
	float latitude;
	int latitude2;
	latitude = atoff(test_char1);
	latitude = atoff(test_char2);
	latitude = atoff(test_char3);
	latitude2 = atoi(test_char1);
	latitude2 = atoi(test_char2);
	latitude2 = atoi(test_char3);
}

// F62:
#define POS_cnrsat_1 73
#define SIZE_cnrsat 5
char char_cnrsat[SIZE_cnrsat];

#define POS_raw_doppler_1 64
#define SIZE_raw_doppler 9
char char_raw_doppler[SIZE_raw_doppler];

#define POS_raw_phase_1 50
#define SIZE_raw_phase 14
char char_raw_phase[SIZE_raw_phase];

#define POS_raw_range_1 37
#define SIZE_raw_range 13
char char_raw_range[SIZE_raw_range];

#define POS_raw_prn_1 35
#define SIZE_raw_prn 2
char char_raw_prn[SIZE_raw_prn];

#define POS_raw_time 8
#define SIZE_raw_time 12
char char_raw_time[SIZE_raw_time];
