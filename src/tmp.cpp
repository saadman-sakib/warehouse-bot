// #include <Arduino.h>
// #include "BTSWheels.h"
// #include "PID.h"

// int Kp = 8, Ki = 0, Kd = 4;
// PID pid(Kp, Ki, Kd);

// BTSWheels wheels;

// int weights[8] = { 10, 20, 30, 40, 50, 60, 70, 80 };
// int threshold = 700;
// int setPoint = 45;
// int lastPosition = 10;
// int startVelocity = 200;
// int lastSensorSum = 0;
// int longLastPosition = 10;

// int linePos(int sensorReadings[8]);

// void setup() {
// 	Serial.begin(9600);
// 	wheels.init();
// 	wheels.start(startVelocity);
// }

// void loop() {
// 	int sensorReadings[8] = {
// 		analogRead(A0), 
// 		analogRead(A1), 
// 		analogRead(A2), 
// 		analogRead(A3), 
// 		analogRead(A4), 
// 		analogRead(A5),
// 		analogRead(A6),
// 		analogRead(A7),
// 	};

// 	int pos = linePos(sensorReadings);

// 	// Serial.println(lastPosition);
// 	if(pos==600){
// 		wheels.leftMotor(0);
// 		wheels.leftMotor(0);
// 	}
	
// 	if(pos==500){
// 		if(longLastPosition<55 and longLastPosition > 35){
// 			wheels.leftMotor(0);
// 			wheels.leftMotor(0);
// 		}
// 		else if(lastPosition >= setPoint){
// 			wheels.rotateRight();
// 		}
// 		else if(lastPosition < setPoint){
// 			wheels.rotateLeft();
// 		}
// 	} else {
// 		int error = pos - setPoint;
// 		int speedChange = pid.output(error);
// 		wheels.leftMotor(wheels.leftSpeed + speedChange);
// 		wheels.rightMotor(wheels.rightSpeed - speedChange);
// 	}
// }


// int linePos(int sensorReadings[8]) {
// 	int pos = 0;
// 	int sensorSum=0;
// 	for (int i = 0; i < 8; i++) {
// 		sensorReadings[i] = (sensorReadings[i] > threshold);
// 		pos += sensorReadings[i] * weights[i];
// 		sensorSum += sensorReadings[i];
// 	}
// 	if(sensorSum==0) return 500;
// 	if(sensorSum==6) return 600;
// 	pos /= sensorSum;
// 	lastPosition = (5*lastPosition + pos*sensorSum)/(5+sensorSum);
// 	longLastPosition = (22*longLastPosition + pos*sensorSum)/(22+sensorSum);
// 	return pos;
// }