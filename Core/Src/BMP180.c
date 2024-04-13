/*
 * BMP180.c
 *
 *  Created on: Mar 30, 2024
 *      Author: Emrec
 */
#include "main.h"
#include "stdlib.h"
#include "BMP180.h"

extern I2C_HandleTypeDef hi2c1;
void BMP180_Read_Id(){
	uint8_t Data[1];
	HAL_I2C_Mem_Read(&hi2c1, BMP_180_Read ,BMP_180_ID , 1, &Data, 1, 1000);
}

void BMP180_Read_Cali_Data(Bmp180_t *BMP_180){
	uint8_t Data[22];
	HAL_I2C_Mem_Read(&hi2c1, BMP_180_Read ,BMP_180_Cali_Dat , 1, &Data, 22 , 1000);
	BMP_180->AC1 = ((Data[0]<<8)|Data[1]);
	BMP_180->AC2 = ((Data[2]<<8)|Data[3]);
	BMP_180->AC3 = ((Data[4]<<8)|Data[5]);
	BMP_180->AC4 = ((Data[6]<<8)|Data[7]);
	BMP_180->AC5 = ((Data[8]<<8)|Data[9]);
	BMP_180->AC6 = ((Data[10]<<8)|Data[11]);
	BMP_180->B1 = ((Data[12]<<8)|Data[13]);
	BMP_180->B2 = ((Data[14]<<8)|Data[15]);
	BMP_180->MB = ((Data[16]<<8)|Data[17]);
	BMP_180->MC = ((Data[18]<<8)|Data[19]);
	BMP_180->MD = ((Data[20]<<8)|Data[21]);

}

void BMP180_Read_Data(Bmp180_t *BMP_180){
	uint8_t Data[1];
	int32_t x1,x2,x3,b3,b5,b6,t,Temp,p;
	uint32_t b4,b7;
	Data[0] = 0x2E;
	HAL_I2C_Mem_Write(&hi2c1, BMP_180_Write, BMP_180_TEMP_Write, 1, &Data, 1, 100);
	HAL_Delay(5);
	int8_t Data_Temp[2];
	HAL_I2C_Mem_Read(&hi2c1, BMP_180_Read ,BMP_180_TEMP_Read , 1, Data_Temp, 2 , 100);
	int32_t T=((Data_Temp[0]<<8)+Data_Temp[1]);
	Data[0] = 0x34+(3<<6);
	HAL_I2C_Mem_Write(&hi2c1, BMP_180_Write, BMP_180_TEMP_Write, 1, &Data, 1, 100);
	HAL_Delay (26);
	uint8_t Data_Press[3];
	HAL_I2C_Mem_Read(&hi2c1, BMP_180_Read ,BMP_180_TEMP_Read , 1, Data_Press, 3 , 100);
	int32_t UP= ((Data_Press[0]<<16)+(Data_Press[1]<<8)+Data_Press[2])>>5;

	x1=(T-BMP_180->AC6 )*BMP_180->AC5/pow(2,15); // Temp calculate Part
	x2=BMP_180->MC*pow(2,11)/(x1+BMP_180->MD);
	b5=x1+x2;
	Temp=(b5+8)/pow(2,4);
	BMP_180->Temperature=Temp/10;

	b6=b5-4000;											//get pressure
	x1=(BMP_180->B2*(b6*b6/pow(2,12))/pow(2,11));
	x2=BMP_180->AC2*b6/pow(2,11);
	x3=x1+x2;
	b3=(((BMP_180->AC1*4+x3)<<3)+2)/4;
	x1=BMP_180->AC3*b6/pow(2,13);
	x2=(BMP_180->B1*(b6*b6/pow(2,12)))/pow(2,16);
	x3=((x1+x2)+2)/4;
	b4=BMP_180->AC4*(uint32_t)(x3+32768)/pow(2,15);
	b7=(uint32_t)(UP-b3)*(50000>>3);
	if(b7<0x80000000){
		p=(b7*2)/b4;
	}else{
		p=(b7/b4)*2;
	}
	x1=(p/pow(2,8)*p/pow(2,8));
	x1=(x1*3038)/pow(2,16);
	x2=(-7357*p)/pow(2,16);
	p=(p+(x1+x2+3791)/pow(2,4));
	BMP_180->Pressure=p;

	BMP_180->Altitude=44330*(1-(pow(((p/100)/1013.25),(1/5.255)))); //altitude Calculate

}


