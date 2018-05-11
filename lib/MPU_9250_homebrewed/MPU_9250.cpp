/*
*	MPU_9250
*
*/

#include "MPU_9250.h"
#include <Wire.h>


void MPU_9250::wake()
{
	Serial.println("Waking MPU-9250");
	writeToRegister(pwr_mgmt_1, 0b00000000);	//	Wakes the MPU-9250
	delay(10);
	writeToRegister(smplrt_div, 0);
	delay(1);
	writeToRegister(user_ctrl, 0);
	delay(1);
	writeToRegister(user_ctrl, i2c_mst_en);
	delay(1);
	writeToRegister(i2c_mst_ctrl, i2c_mst_clk);
	delay(100);

	writeToRegister(i2c_slv0_addr, ak8963_address | read_flag);
	writeToRegister(i2c_slv0_reg, hxl);
	writeToRegister(i2c_slv0_ctrl, i2c_slv0_en | 7);
	delay(1);
}

void MPU_9250::writeToRegister(uint8_t registerAddr, uint8_t data)
{
	Wire.beginTransmission(mpu9250_address);
	Wire.write(registerAddr);
	Wire.write(data);
	Wire.endTransmission();
	//Serial.print("Wrote 0x"); Serial.print(data, HEX); Serial.print(" to register 0x"); Serial.println(registerAddr, HEX);
}

void MPU_9250::getBytes(uint8_t registerStart, uint8_t amount, uint8_t * receiver)
{
	Wire.beginTransmission(mpu9250_address);
	Wire.write(registerStart);
	Wire.endTransmission(false);
	Wire.requestFrom(mpu9250_address, amount);
	for(uint8_t i = 0; i < amount; ++i)
	{
		receiver[i] = Wire.read();
	}
	//Serial.print("Getting "); Serial.print(amount); Serial.print(" bytes, starting from register 0x"); Serial.println(registerStart, HEX);
}

void MPU_9250::writeToAK8963(uint8_t AK8963Register, uint8_t data)
{
	writeToRegister(i2c_slv0_addr, ak8963_address);
	writeToRegister(i2c_slv0_reg, AK8963Register);
	writeToRegister(i2c_slv0_do, data);
	writeToRegister(i2c_slv0_ctrl, i2c_slv0_en | static_cast<uint8_t>(1));
}

void MPU_9250::getFromAK8963(uint8_t registerStart, uint8_t amount, uint8_t * receiver)
{
	uint8_t temp[amount];
	writeToRegister(i2c_slv0_addr, ak8963_address | read_flag);		//	Sets the magnetometer as slave 0 and that the transfer is a read
	writeToRegister(i2c_slv0_reg, registerStart);
	writeToRegister(i2c_slv0_ctrl, i2c_slv0_en | amount);
	delay(1);
	getBytes(ext_sens_data_00, amount, temp);		//	The data from the AK8963 is stored in the MPU9250 in register EXT_SENS_DATA_xx
	for (int i = 0; i < amount; ++i)
	{
		receiver[i] = temp[i];
	}
}


MPU_9250::MPU_9250()
:_gyroFS(0), _accelFS(0)
{
	delay(1);
	Wire.begin();
	delay(1);
	Wire.setClock(400000);
	delay(10);
	Serial.begin(115200);
	delay(10);
	Serial.println("MPU-9250 constructor");
	delay(10);
	wake();
	delay(10);
	setGyro_FS_SEL(_gyroFS);
	setAccel_FS_SEL(_accelFS);
}

void MPU_9250::readAcc()
{
	//Serial.println("readAcc()");
	uint8_t data[6];
	getBytes(accel_xout_h, 6, data);
	_buffer[0] = static_cast<int16_t>(data[0]) << 8 | static_cast<int16_t>(data[1]);
	_buffer[1] = static_cast<int16_t>(data[2]) << 8 | static_cast<int16_t>(data[3]);
	_buffer[2] = static_cast<int16_t>(data[4]) << 8 | static_cast<int16_t>(data[5]);
	accel[0] = (trans_x[0] * _buffer[0]) + (trans_x[1] * _buffer[1]) + (trans_x[2] * _buffer[2]);
	accel[1] = (trans_y[0] * _buffer[0]) + (trans_y[1] * _buffer[1]) + (trans_y[2] * _buffer[2]);
	accel[2] = (trans_z[0] * _buffer[0]) + (trans_z[1] * _buffer[1]) + (trans_z[2] * _buffer[2]);
	//Serial.println();
}
void MPU_9250::readGyr()
{
	//Serial.println("readGyr()");
	uint8_t data[6];
	getBytes(gyro_xout_h, 6, data);
	_buffer[0] = static_cast<int16_t>(data[0]) << 8 | static_cast<int16_t>(data[1]);
	_buffer[1] = static_cast<int16_t>(data[2]) << 8 | static_cast<int16_t>(data[3]);
	_buffer[2] = static_cast<int16_t>(data[4]) << 8 | static_cast<int16_t>(data[5]);
	gyro[0] = (trans_x[0] * _buffer[0]) + (trans_x[1] * _buffer[1]) + (trans_x[2] * _buffer[2]);
	gyro[1] = (trans_y[0] * _buffer[0]) + (trans_y[1] * _buffer[1]) + (trans_y[2] * _buffer[2]);
	gyro[2] = (trans_z[0] * _buffer[0]) + (trans_z[1] * _buffer[1]) + (trans_z[2] * _buffer[2]);
	//Serial.println();
}
void MPU_9250::readMag()
{
	//Serial.println("readMag()");
	uint8_t data[7];
	getFromAK8963(hxl, 7, data);
	//getBytes(ext_sens_data_00, 6, data);
	mag[0] = static_cast<int16_t>(data[1]) << 8 | static_cast<int16_t>(data[0]);	//	The magnetometer orders its registers with the low byte first
	mag[1] = static_cast<int16_t>(data[3]) << 8 | static_cast<int16_t>(data[2]);
	mag[2] = static_cast<int16_t>(data[5]) << 8 | static_cast<int16_t>(data[4]);
	//Serial.println();
}
void MPU_9250::readTemp()
{
	//Serial.println("readTemp()");
	uint8_t data[2];
	getBytes(temp_out_h, 2, data);
	temp = static_cast<int16_t>(data[0]) << 8 | static_cast<int16_t>(data[1]);
	//Serial.println();
}

void MPU_9250::setGyro_FS_SEL(uint8_t fs)
{
	//Serial.print("setGyro_FS_SEL("); Serial.print(fs); Serial.println(")");
	if (fs > 3)
	{
		Serial.println("Invalid input argument in function setGyro_FS_SEL");
		Serial.println("Full Scale Select set to 0");
		fs = 0;
	}
	_gyroFS = fs;
	uint8_t reg;
	getBytes(gyro_config, 1, &reg);
	reg &= !(0b00011000);				//erase the current FS
	reg |= (_gyroFS << 3);
	writeToRegister(gyro_config, reg);
}

void MPU_9250::setAccel_FS_SEL(uint8_t fs)
{
	//Serial.print("setAccel_FS_SEL("); Serial.print(fs); Serial.println(")");
	if (fs > 3)
	{
		Serial.println("Invalid input argument in function setAccel_FS_SEL");
		Serial.println("Full Scale Select set to 0");
		fs = 0;
	}
	_accelFS = fs;
	uint8_t reg;
	getBytes(accel_config, 1, &reg);
	reg &= !(0b00011000);				//erase the current FS
	reg |= (_accelFS << 3);
	writeToRegister(accel_config, reg);
	//Serial.println();
}

uint8_t MPU_9250::getGyro_FS()	{return _gyroFS;}
uint8_t MPU_9250::getAccel_FS()	{return _accelFS;}
