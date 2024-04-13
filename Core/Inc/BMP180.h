/*
 * BMP180.h
 *
 *  Created on: Mar 31, 2024
 *      Author: Emrec
 */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#define BMP_180_Read 		0xEE
#define BMP_180_Write 		0xEF
#define BMP_180_ID 			0xD0
#define BMP_180_Cali_Dat 	0xAA
#define BMP_180_TEMP_Read  	0xF6
#define BMP_180_TEMP_Write  0xF4

typedef struct{
	int16_t AC1;
	int16_t AC2;
	int16_t AC3;
	uint16_t AC4;
	uint16_t AC5;
	uint16_t AC6;
	int16_t B1;
	int16_t B2;
	int16_t MB;
	int16_t MC;
	int16_t MD;
	int32_t Pressure;
	int16_t Temperature;
	float Altitude;

}Bmp180_t;

void BMP180_Read_Data(Bmp180_t *BMP_180);
void BMP180_Read_Cali_Data(Bmp180_t *BMP_180);
void BMP180_Read_Id();

#endif /* INC_BMP180_H_ */
