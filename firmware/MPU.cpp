#include "MPU.h"

volatile unsigned char _dataReady;
void (*triggerCallback)(void);

char _orientation[9] = { 1, 0, 0,
                         0, 1, 0,
                         0, 0, 1 };

void setupMPU(void (*cb)(void))
{
    triggerCallback = cb;
    Serial.begin(115200);
    Wire.begin();
    delay(500);

    Serial.println("Hi there");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    InitMPU();

    _dataReady = 0;

    digitalWrite(LED_BUILTIN, LOW);
}

void MPUloop()
{
    if (_dataReady)
    {
        short gyro[3];
        short accel[3];
        unsigned long timestamp;
        unsigned char more;
        int ret;

        unsigned char sensors;

        if (0 == (ret = mpu_read_fifo(gyro, accel, &timestamp, &sensors, &more)))
        {
            if (!more)
            {
                _dataReady = 0;
            }

            if (accel[VEC_Z] > 17000) {
                triggerCallback();
            }
        }
        else
        {
            Serial.print("Error: ");Serial.println(ret);
        }
    }
}

void End()
{
    Serial.println("You came to the end. Please reset");

    while(1)
    {
        delay(500);
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
}

static void DataReadyCallback()
{
    _dataReady = 1;
}

unsigned short Row2Scale(const char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;

    return b;
}

unsigned short Matrix2Scalar(const char *mtx)
{
    unsigned short scalar;

    scalar = Row2Scale(mtx);
    scalar |= Row2Scale(mtx + 3) << 3;
    scalar |= Row2Scale(mtx + 6) << 6;

    return scalar;
}

void PrintMPUValues()
{
    unsigned short ushortValue;
    unsigned char ucharValue;
    float floatValue;

    mpu_get_accel_fsr(&ucharValue);
    Serial.print("Accel full-scale range rate: ");Serial.println(ucharValue);

    mpu_get_accel_sens(&ushortValue);
    Serial.print("Accel sensitivity: ");Serial.println(ushortValue);

    mpu_get_gyro_fsr(&ushortValue);
    Serial.print("Gyro full-scale range rate: ");Serial.println(ushortValue);

    mpu_get_gyro_sens(&floatValue);
    Serial.print("Gyro sensitivity: ");Serial.println(floatValue);

    mpu_get_lpf(&ushortValue);
    Serial.print("DLPF: ");Serial.println(ushortValue);

    dmp_get_fifo_rate(&ushortValue);
    Serial.print("FIFO rate: ");Serial.println(ushortValue);

    mpu_get_sample_rate(&ushortValue);
    Serial.print("Sample rate: ");Serial.println(ushortValue);
}

void NormalizeQuaternion(float *quat)
{
    float length = sqrt(quat[QUAT_W] * quat[QUAT_W] + quat[QUAT_X] * quat[QUAT_X] +
        quat[QUAT_Y] * quat[QUAT_Y] + quat[QUAT_Z] * quat[QUAT_Z]);

    if (length == 0)
        return;

    quat[QUAT_W] /= length;
    quat[QUAT_X] /= length;
    quat[QUAT_Y] /= length;
    quat[QUAT_Z] /= length;
}

void InitMPU()
{
    int ret;

    struct int_param_s int_param;
    int_param.cb = DataReadyCallback;
    int_param.pin = PIN_D0;

    Serial.println("Init MPU");

    if (0 != (ret = mpu_init(&int_param)))
    {
        Serial.print("Failed to init mpu: ");Serial.println(ret);
        End();
    }

    if (0 != (ret = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL)))
    {
        Serial.print("Failed to set sensor: ");Serial.println(ret);
        End();
    }

    if (0 != (ret = mpu_configure_fifo(INV_XYZ_ACCEL)))
    {
        Serial.print("Failed to set sensor: ");Serial.println(ret);
        End();
    }

    if (0 != (ret = mpu_set_sample_rate(MPU_HZ)))
    {
        Serial.print("Failed to set sensor: ");Serial.println(ret);
        End();
    }

    if (0 != (ret = dmp_load_motion_driver_firmware()))
    {
        Serial.print("Failed to load dmp firmware: ");Serial.println(ret);
        End();
    }

    if (0 != (ret = dmp_set_orientation(Matrix2Scalar(_orientation))))
    {
        Serial.print("Failed to set orientation: ");Serial.println(ret);
        End();
    }

    if (0 != (ret = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_SEND_RAW_ACCEL |
        DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL |
        DMP_FEATURE_TAP)))
    {
        Serial.print("Failed to enable feature: ");Serial.println(ret);
        End();
    }

    if (0 != (ret = dmp_set_fifo_rate(MPU_HZ)))
    {
        Serial.print("Failed to set fifo rate: ");Serial.println(ret);
        End();
    }

    if (0 != (ret = mpu_set_dmp_state(USE_DMP)))
    {
        Serial.print("Failed to set DMP state: ");Serial.println(ret);
        End();
    }

    PrintMPUValues();

    Serial.println("DONE!!!");

}


