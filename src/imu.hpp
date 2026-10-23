#ifndef IMU_HPP
#define IMU_HPP

#include <stdint.h>

typedef class Imu {
private:
    void read_accel();
    void read_gyro();
    void imu_reset();
    int i2c_read(uint8_t reg, uint8_t *data, size_t length);
    int i2c_write(uint8_t reg, uint8_t *data, size_t length);
    int i2c_write(uint8_t reg, uint8_t data);
    int i2c_write(uint8_t reg);
    int16_t accel[3];   // X, Y, Z acceleration values
    int16_t gyro[3];    // X, Y, Z gyroscope values
public:
    Imu();
    ~Imu();

    void init();
    void update();
    void get_accel(int16_t *);
    void get_gyro(int16_t *);
} Imu;

#endif