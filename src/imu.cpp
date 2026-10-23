#ifndef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <string.h>
#include "imu.hpp"

#define IMU_ADDRESS 0x68 // I2C address for MPU6050


#define IMU_ACCEL_XOUT_H 0x3B
#define IMU_ACCEL_YOUT_H 0x3D
#define IMU_ACCEL_ZOUT_H 0x3F
#define IMU_GYRO_XOUT_H 0x43
#define IMU_GYRO_YOUT_H 0x45
#define IMU_GYRO_ZOUT_H 0x47

#define IMU_PWR_MGMT_1 0x6B

#define IMU_I2C i2c_default


void Imu::get_accel(int16_t *pAccel) {
    pAccel[0] = this->accel[0];
    pAccel[1] = this->accel[1];
    pAccel[2] = this->accel[2];
}

void Imu::get_gyro(int16_t *pGyro) {
    pGyro[0] = this->gyro[0];
    pGyro[1] = this->gyro[1];
    pGyro[2] = this->gyro[2];
}

void Imu::read_accel() {
    uint8_t buffer[6];
    this->i2c_read(IMU_ACCEL_XOUT_H, buffer, 6);
    this->accel[0] = (buffer[0] << 8) | buffer[1]; // X-axis
    this->accel[1] = (buffer[2] << 8) | buffer[3]; // Y-axis
    this->accel[2] = (buffer[4] << 8) | buffer[5]; // Z-axis
}

void Imu::read_gyro() {
    uint8_t buffer[6];
    this->i2c_read(IMU_GYRO_XOUT_H, buffer, 6);
    this->gyro[0] = (buffer[0] << 8) | buffer[1]; // X-axis
    this->gyro[1] = (buffer[2] << 8) | buffer[3]; // Y-axis
    this->gyro[2] = (buffer[4] << 8) | buffer[5]; // Z-axis
}

int Imu::i2c_read(uint8_t reg, uint8_t *data, size_t length)
{
    int ret;

    this->i2c_write(reg);
    ret = i2c_read_timeout_us(IMU_I2C, IMU_ADDRESS, data, length, false, 100000); // Read data
    return ret;
}

int Imu::i2c_write(uint8_t reg, uint8_t *data, size_t length)
{
    int ret;

    ret = i2c_write_timeout_us(IMU_I2C, IMU_ADDRESS, &reg, 1, true, 100000); // Write register address
    if (ret != PICO_ERROR_GENERIC) {
        ret = i2c_write_timeout_us(IMU_I2C, IMU_ADDRESS, data, length, false, 100000); // Write data
    }
    return ret;
}

int Imu::i2c_write(uint8_t reg, uint8_t data)
{
    int ret;

    ret = i2c_write_timeout_us(IMU_I2C, IMU_ADDRESS, &reg, 1, true, 100000); // Write register address
    if (ret != PICO_ERROR_GENERIC) {
        ret = i2c_write_timeout_us(IMU_I2C, IMU_ADDRESS, &data, 1, false, 100000); // Write data
    }
    return ret;
}

int Imu::i2c_write(uint8_t reg)
{
    return i2c_write_timeout_us(IMU_I2C, IMU_ADDRESS, &reg, 1, true, 100000);
}

void Imu::imu_reset() {
    // Reset the IMU sensor (e.g., MPU6050)
    this->i2c_write(IMU_PWR_MGMT_1, 0x80); // Write to the power management register to reset the device
    sleep_ms(100); // Allow device to reset and stabilize

    this->i2c_write(IMU_PWR_MGMT_1, 0x00);  // Clear sleep mode by writing 0x00 to the 0x6B register
    sleep_ms(30); // Allow stabilization after waking up
}

void Imu::update() {
    // Update IMU sensor readings
    this->read_accel();
    this->read_gyro();
}

void Imu::init() {
    // Initialize I2C interface for the IMU sensor
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Initialize the IMU sensor (e.g., MPU6050)
    this->imu_reset();
}


Imu::Imu() {
    // Constructor implementation
}

Imu::~Imu() {
    // Destructor implementation
}

#ifndef __cplusplus
}
#endif