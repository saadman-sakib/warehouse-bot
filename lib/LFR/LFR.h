#include "BTSWheels.h"
#include "PID.h"

class LFR {
private:
    int weights[8] = { 10, 20, 30, 40, 50, 60, 70, 80 };
    int threshold = 300;
    int setPoint = 45;
    int lastPosition = 10;
    int lastSensorSum = 0;
    int longLastPosition = 10;
    bool turnOff = 0;
    
    int startVelocity = 100;
public:

    LFR(int startVelocity){
        this->startVelocity = startVelocity;
    }

    bool is_on(){
        return !turnOff;
    }

    int linePos(int sensorReadings[8]) {
        int pos = 0;
        int sensorSum = 0;
        for (int i = 0; i < 8; i++) {
            sensorReadings[i] = (sensorReadings[i] > threshold);
            pos += sensorReadings[i] * weights[i];
            sensorSum += sensorReadings[i];
        }
        if (sensorSum == 0) return 500;
        if (sensorSum == 6) return 600;
        pos /= sensorSum;
        this->lastPosition = (5 * lastPosition + pos * sensorSum) / (5 + sensorSum);
        this->longLastPosition = (22 * longLastPosition + pos * sensorSum) / (22 + sensorSum);
        return pos;
    }

    void move(int pos, BTSWheels wheels, PID pid) {
        if (pos == 600) {
            wheels.leftMotor(0);
            wheels.rightMotor(0);
            turnOff = 1;
        } else if (pos == 500) {
            // if (longLastPosition < 55 and longLastPosition > 35) {
            //     wheels.leftMotor(0);
            //     wheels.leftMotor(0);
            // }
            if (lastPosition >= setPoint) {
                wheels.rotateRight();
            }
            else if (lastPosition < setPoint) {
                wheels.rotateLeft();
            }
        } else {
            int error = pos - setPoint;
            int speedChange = pid.output(error);
            wheels.rightMotor(-(wheels.leftSpeed - speedChange));
            wheels.leftMotor(-(wheels.rightSpeed + speedChange));

            Serial.print(pos);Serial.print("\t");
            Serial.print(speedChange);Serial.print("\t");

            Serial.print(wheels.leftSpeed - speedChange); Serial.print("\t");
            Serial.println(wheels.rightSpeed + speedChange);

        }
    }
};
