/*
*	MPU-9250
*	Datasheet: https://www.invensense.com/download-pdf/mpu-9250-datasheet/
*	Register map: https://www.invensense.com/download-pdf/mpu-9250-register-map/
*
*	The "SparkFun IMU Breakout - MPU-9250" was used: https://www.sparkfun.com/products/13762
*	Breakout board schematic: https://cdn.sparkfun.com/datasheets/Sensors/IMU/SparkFun_MPU-9250_Breakout.pdf
*
*	Author: Jakob Lunde
*/
#ifndef _MPU_9250_H_
#define _MPU_9250_H_

#include <stdint.h>


class MPU_9250
{
	private:
		void wake();
		void writeToRegister(uint8_t registerAddr, uint8_t data);
		void getBytes(uint8_t registerStart, uint8_t amount, uint8_t * receiver);
		void writeToAK8963(uint8_t AK8963Register, uint8_t data);
		void getFromAK8963(uint8_t registerStart, uint8_t amount, uint8_t * receiver);
		uint8_t _gyroFS;
		uint8_t _accelFS;

		// transformation matrix so the gyro and accel axes match the magnetometers axes
		const int16_t trans_x[3] = {0, 1, 0};
		const int16_t trans_y[3] = {1, 0, 0};
		const int16_t trans_z[3] = {0, 0, -1};
		int16_t _buffer[3];

	public:
		MPU_9250();
		void readAcc();
		void readGyr();
		void readMag();
		void readTemp();
		int16_t accel[3];
		int16_t gyro[3];
		int16_t mag[3];
		int16_t temp;

		void setGyro_FS_SEL(uint8_t fs);
		void setAccel_FS_SEL(uint8_t fs);
		uint8_t getGyro_FS();
		uint8_t getAccel_FS();

	private:
		//I2C addresses of the devices
		const uint8_t mpu9250_address = 0x68;
		const uint8_t ak8963_address = 0x0C;
		//some useful bits
		const uint8_t read_flag = 1<<7;
		const uint8_t i2c_slv0_en = 1<<7;
		const uint8_t i2c_mst_en = 1<<5;
		const uint8_t i2c_mst_clk = 13;

		//Gyro & Accelerometer registers
		const uint8_t	self_test_x_gyro	=	0x00	;
		const uint8_t	self_test_y_gyro	=	0x01	;
		const uint8_t	self_test_z_gyro	=	0x02	;
		const uint8_t	self_test_x_accel	=	0x0D	;
		const uint8_t	self_test_y_accel	=	0x0E	;
		const uint8_t	self_test_z_accel	=	0x0F	;
		const uint8_t	xg_offset_h	=	0x13	;
		const uint8_t	xg_offset_l	=	0x14	;
		const uint8_t	yg_offset_h	=	0x15	;
		const uint8_t	yg_offset_l	=	0x16	;
		const uint8_t	zg_offset_h	=	0x17	;
		const uint8_t	zg_offset_l	=	0x18	;
		const uint8_t	smplrt_div	=	0x19	;
		const uint8_t	config	=	0x1A	;
		const uint8_t	gyro_config	=	0x1B	;
		const uint8_t	accel_config	=	0x1C	;
		const uint8_t	accel_config_2	=	0x1D	;
		const uint8_t	lp_accel_odr	=	0x1E	;
		const uint8_t	wom_thr	=	0x1F	;
		const uint8_t	fifo_en	=	0x23	;
		const uint8_t	i2c_mst_ctrl	=	0x24	;
		const uint8_t	i2c_slv0_addr	=	0x25	;
		const uint8_t	i2c_slv0_reg	=	0x26	;
		const uint8_t	i2c_slv0_ctrl	=	0x27	;
		const uint8_t	i2c_slv1_addr	=	0x28	;
		const uint8_t	i2c_slv1_reg	=	0x29	;
		const uint8_t	i2c_slv1_ctrl	=	0x2A	;
		const uint8_t	i2c_slv2_addr	=	0x2B	;
		const uint8_t	i2c_slv2_reg	=	0x2C	;
		const uint8_t	i2c_slv2_ctrl	=	0x2D	;
		const uint8_t	i2c_slv3_addr	=	0x2E	;
		const uint8_t	i2c_slv3_reg	=	0x2F	;
		const uint8_t	i2c_slv3_ctrl	=	0x30	;
		const uint8_t	i2c_slv4_addr	=	0x31	;
		const uint8_t	i2c_slv4_reg	=	0x32	;
		const uint8_t	i2c_slv4_do	=	0x33	;
		const uint8_t	i2c_slv4_ctrl	=	0x34	;
		const uint8_t	i2c_slv4_di	=	0x35	;
		const uint8_t	i2c_mst_status	=	0x36	;
		const uint8_t	int_pin_cfg	=	0x37	;
		const uint8_t	int_enable	=	0x38	;
		const uint8_t	int_status	=	0x3A	;
		const uint8_t	accel_xout_h	=	0x3B	;
		const uint8_t	accel_xout_l	=	0x3C	;
		const uint8_t	accel_yout_h	=	0x3D	;
		const uint8_t	accel_yout_l	=	0x3E	;
		const uint8_t	accel_zout_h	=	0x3F	;
		const uint8_t	accel_zout_l	=	0x40	;
		const uint8_t	temp_out_h	=	0x41	;
		const uint8_t	temp_out_l	=	0x42	;
		const uint8_t	gyro_xout_h	=	0x43	;
		const uint8_t	gyro_xout_l	=	0x44	;
		const uint8_t	gyro_yout_h	=	0x45	;
		const uint8_t	gyro_yout_l	=	0x46	;
		const uint8_t	gyro_zout_h	=	0x47	;
		const uint8_t	gyro_zout_l	=	0x48	;
		const uint8_t	ext_sens_data_00	=	0x49	;
		const uint8_t	ext_sens_data_01	=	0x4A	;
		const uint8_t	ext_sens_data_02	=	0x4B	;
		const uint8_t	ext_sens_data_03	=	0x4C	;
		const uint8_t	ext_sens_data_04	=	0x4D	;
		const uint8_t	ext_sens_data_05	=	0x4E	;
		const uint8_t	ext_sens_data_06	=	0x4F	;
		const uint8_t	ext_sens_data_07	=	0x50	;
		const uint8_t	ext_sens_data_08	=	0x51	;
		const uint8_t	ext_sens_data_09	=	0x52	;
		const uint8_t	ext_sens_data_10	=	0x53	;
		const uint8_t	ext_sens_data_11	=	0x54	;
		const uint8_t	ext_sens_data_12	=	0x55	;
		const uint8_t	ext_sens_data_13	=	0x56	;
		const uint8_t	ext_sens_data_14	=	0x57	;
		const uint8_t	ext_sens_data_15	=	0x58	;
		const uint8_t	ext_sens_data_16	=	0x59	;
		const uint8_t	ext_sens_data_17	=	0x5A	;
		const uint8_t	ext_sens_data_18	=	0x5B	;
		const uint8_t	ext_sens_data_19	=	0x5C	;
		const uint8_t	ext_sens_data_20	=	0x5D	;
		const uint8_t	ext_sens_data_21	=	0x5E	;
		const uint8_t	ext_sens_data_22	=	0x5F	;
		const uint8_t	ext_sens_data_23	=	0x60	;
		const uint8_t	i2c_slv0_do	=	0x63	;
		const uint8_t	i2c_slv1_do	=	0x64	;
		const uint8_t	i2c_slv2_do	=	0x65	;
		const uint8_t	i2c_slv3_do	=	0x66	;
		const uint8_t	i2c_mst_delay_ctrl	=	0x67	;
		const uint8_t	signal_path_reset	=	0x68	;
		const uint8_t	mot_detect_ctrl	=	0x69	;
		const uint8_t	user_ctrl	=	0x6A	;
		const uint8_t	pwr_mgmt_1	=	0x6B	;
		const uint8_t	pwr_mgmt_2	=	0x6C	;
		const uint8_t	fifo_counth	=	0x72	;
		const uint8_t	fifo_countl	=	0x73	;
		const uint8_t	fifo_r_w	=	0x74	;
		const uint8_t	who_am_i	=	0x75	;
		const uint8_t	xa_offset_h	=	0x77	;
		const uint8_t	xa_offset_l	=	0x78	;
		const uint8_t	ya_offset_h	=	0x7A	;
		const uint8_t	ya_offset_l	=	0x7B	;
		const uint8_t	za_offset_h	=	0x7D	;
		const uint8_t	za_offset_l	=	0x7E	;
		//magnetometer registers
		const uint8_t	wia	=	0x00	;
		const uint8_t	info	=	0x01	;
		const uint8_t	st1	=	0x02	;
		const uint8_t	hxl	=	0x03	;
		const uint8_t	hxh	=	0x04	;
		const uint8_t	hyl	=	0x05	;
		const uint8_t	hyh	=	0x06	;
		const uint8_t	hzl	=	0x07	;
		const uint8_t	hzh	=	0x08	;
		const uint8_t	st2	=	0x09	;
		const uint8_t	cntl	=	0x0A	;
		const uint8_t	rsv	=	0x0B	;
		const uint8_t	astc	=	0x0C	;
		const uint8_t	ts1	=	0x0D	;
		const uint8_t	ts2	=	0x0E	;
		const uint8_t	i2cdis	=	0x0F	;
		const uint8_t	asax	=	0x10	;
		const uint8_t	asay	=	0x11	;
		const uint8_t	asaz	=	0x12	;
};

#endif // _MPU_9250_H_
