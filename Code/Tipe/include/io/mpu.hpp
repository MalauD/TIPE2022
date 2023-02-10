#ifndef IO_MPU_HPP
#define IO_MPU_HPP

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <array>
#include <iostream>

class MpuReading;

class Mpu {
  private:
    Adafruit_MPU6050 mpu;

  public:
    void begin();
    void read(MpuReading &reading);
};

class MpuReading {
  private:
    std::array<float, 3> accel;
    std::array<float, 3> gyro;

  public:
    void serialize(std::ostream &os);
    void print();
    friend class Mpu;
};

void Mpu::begin() { mpu.begin(); }

void Mpu::read(MpuReading &reading) {
    sensors_event_t accel_event;
    sensors_event_t gyro_event;
    sensors_event_t temp_event;

    mpu.getEvent(&accel_event, &gyro_event, &temp_event);

    reading.accel[0] = accel_event.acceleration.x;
    reading.accel[1] = accel_event.acceleration.y;
    reading.accel[2] = accel_event.acceleration.z;

    reading.gyro[0] = gyro_event.gyro.x;
    reading.gyro[1] = gyro_event.gyro.y;
    reading.gyro[2] = gyro_event.gyro.z;
}

void MpuReading::print() {
    Serial.println("Accel: " + String(accel[0]) + ", " + String(accel[1]) +
                   ", " + String(accel[2]));
    Serial.println("Gyro: " + String(gyro[0]) + ", " + String(gyro[1]) + ", " +
                   String(gyro[2]));
}

void MpuReading::serialize(std::ostream &os) {
    os << accel[0] << "," << accel[1] << "," << accel[2];
    os << "," << gyro[0] << "," << gyro[1] << "," << gyro[2];
}

#endif