#include "stm32f4xx_hal.h"
#include <math.h>

#define BMP180_ADDRESS_Write 0xEE
#define BMP180_ADDRESS_Read 0xEF
#define BMP180_I2C &hi2c1
long UT=0 ;
short oss=0;
long UP=0;
long X1=0;
long X2=0;
long X3=0;
long B3=0;
long B5=0;
unsigned long B4=0;
long B6=0;
unsigned long B7=0;
long Press=0;
long Temp;


extern I2C_HandleTypeDef hi2c1;

#define HAL_MAX_DELAY      0xFFFFFFFFU
#define atmPress 101325


void read_calibration_data(void) {
    uint8_t calibrationData[22] = {0};
    uint16_t Callib_start = 0xAA;
     HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS_Read, Callib_start, 1, calibrationData, 22, HAL_MAX_DELAY);


    }


uint32_t get_pressure(int oss) {
    uint16_t dataToWrite = 0x34 + (oss << 6);
   HAL_I2C_Mem_Write(BMP180_I2C, BMP180_ADDRESS_Write, 0xF4, 1, &dataToWrite, 1, 1000);



    switch (oss) {
        case 0:
            HAL_Delay(5);
            break;
        case 1:
            HAL_Delay(8);
            break;
        case 2:
            HAL_Delay(14);
            break;
        case 3:
            HAL_Delay(26);
            break;
    }

    uint8_t Press_RAW[3] = {0};
    HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS_Read, 0xF6, 1, Press_RAW, 3, 1000);



    uint32_t press_raw = ((Press_RAW[0] << 16) + (Press_RAW[1] << 8) + Press_RAW[2]) >> (8 - oss);
    return press_raw;
}

void bmp180_start(void) {
    read_calibration_data();

}
uint16_t Get_UTemp (void)
{
	uint8_t datatowrite = 0x2E;
	uint8_t Temp_RAW[2] = {0};
	HAL_I2C_Mem_Write(BMP180_I2C, BMP180_ADDRESS_Write, 0xF4, 1, &datatowrite, 1, 1000);
	HAL_Delay (5);
	HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS_Read, 0xF6, 1, Temp_RAW, 2, 1000);
	return ((Temp_RAW[0]<<8) + Temp_RAW[1]);
}

float BMP180_GetTemp (void)
{
	UT = Get_UTemp();
	X1 = 0xB5;
	X2 = ((((0xBC<<8)|0xBD)*(pow(2,11))) / (X1+((0xBE<<8)|0xBF)));
	B5 = X1+X2;
	Temp = (B5+8)/(pow(2,4));


}

