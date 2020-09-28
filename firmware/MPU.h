#ifndef MPU_h
#define MPU_h

#include <Wire.h>
#include <I2Cdev.h>
#include <inv_mpu.h>
#include <inv_mpu_dmp_motion_driver.h>

#define QUAT_W 0
#define QUAT_X 1
#define QUAT_Y 2
#define QUAT_Z 3

#define VEC_X 0
#define VEC_Y 1
#define VEC_Z 2

#define PIN_D0 16

#define MPU_HZ 20
#define USE_DMP 0

void setupMPU();
void MPUloop();
void InitMPU();

#endif
