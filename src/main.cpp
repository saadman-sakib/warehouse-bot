#include <Arduino.h>
#include <HCSR04.h>
// #include "BTSWheels.h" // included in lfr.h
// #include "PID.h" // included in lfr.h
#include "LFR.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define TACH_PIN 48

int jack_up = 52;
int jack_down = 53;
bool up = 0;
bool tach = 0;
int ticks = 0;

int oldTickValue = 0;
int newTickValue = 0;

int startVelocity = 100;

bool DEBUG = false;
bool LFR_ON = true;
bool JACK_IS_UP = false;
bool LFR_MODE = false;
bool wheel_turned_off = false;

// HCSR04 hc(trig, new int[num_of_pins]{echo_pins}, num_of_pins);
HCSR04 hc(46, new int[7] {14, 15, 16, 17, 18, 20, 21}, 7);
BTSWheels wheels;
PID pid(2, 0, 3);
LFR lfr(startVelocity);
Adafruit_MPU6050 mpu;

void jack_off() {
	digitalWrite(jack_up, LOW);
	digitalWrite(jack_down, LOW);
}

void jack_upward() {
	if(!JACK_IS_UP){
		digitalWrite(jack_down, LOW);
		digitalWrite(jack_up, HIGH);
		delay(6000);
		jack_off();
		JACK_IS_UP = 1;
	}
}

void jack_downward(){
	digitalWrite(jack_up, LOW);
	digitalWrite(jack_down, HIGH);
	delay(8000);
	jack_off();
	JACK_IS_UP = 0;
}

void mpu_config_setup(Adafruit_MPU6050& mpu) {
	mpu.setHighPassFilter(MPU6050_HIGHPASS_DISABLE);
	mpu.setMotionDetectionThreshold(0);
	mpu.setMotionDetectionDuration(20);
	mpu.setInterruptPinLatch(true);
	mpu.setInterruptPinPolarity(true);
	// mpu.setMotionInterrupt(true);
}

void car_jack_test();

void motor_test();

void mpu6050_test();

void tachometer_test();




void setup() {
	Serial.begin(9600);
	Serial.setTimeout(20);
	wheels.init(5, 6, 9, 10);
	pinMode(jack_up, OUTPUT);
	pinMode(jack_down, OUTPUT);

	pinMode(TACH_PIN, INPUT);
	newTickValue = digitalRead(TACH_PIN);

	// while (!mpu.begin()) {}
	// Serial.println("MPU6050 Found!");
	// mpu_config_setup(mpu);

	pinMode(13, INPUT_PULLUP);
	pinMode(46, INPUT_PULLUP);


}



void loop() {

	LFR_MODE = !digitalRead(13);
	bool JACK_DOWN = !digitalRead(46);


	int sr[8] = {
		analogRead(A0), 
		analogRead(A1), 
		analogRead(A2), 
		analogRead(A3), 
		analogRead(A4), 
		analogRead(A5),
		analogRead(A6),
		analogRead(A7),
	};

	// Serial.print(sr[0]);Serial.print("\t");
	// Serial.print(sr[1]);Serial.print("\t");
	// Serial.print(sr[2]);Serial.print("\t");
	// Serial.print(sr[3]);Serial.print("\t");
	// Serial.print(sr[4]);Serial.print("\t");
	// Serial.print(sr[5]);Serial.print("\t");
	// Serial.print(sr[6]);Serial.print("\t");
	// Serial.println(sr[7]);

	// Serial.println(digitalRead(46));

	if(LFR_ON and LFR_MODE){
		wheel_turned_off = 0;
		int pos = lfr.linePos(sr);
		lfr.move(pos, wheels, pid);
	}

	if(!LFR_MODE and !wheel_turned_off){
		wheels.leftMotor(0);
		wheels.rightMotor(0);
		wheel_turned_off = 1;
	}
	
	if(lfr.is_on()==0 and LFR_ON and LFR_MODE){
		delay(500);
		LFR_ON = false;
		jack_upward();
	}

	if(!LFR_MODE and JACK_DOWN){
		jack_downward();
	}

	if(Serial.available() and !LFR_MODE){
		String input = Serial.readStringUntil('\n');
		input.trim();
		
		int index = input.indexOf(' ');

		String left = input.substring(0, index);
		String right = input.substring(index+1);

		int left_speed = left.toInt();
		int right_speed = right.toInt();

		wheels.leftMotor(left_speed);
		wheels.rightMotor(right_speed);
	}
}


void tachometer_test() {
	oldTickValue = newTickValue;
	newTickValue = digitalRead(TACH_PIN);

	if ((newTickValue == 0) && (oldTickValue == 1)) {
		ticks++;
	}

	Serial.print(oldTickValue);
	Serial.print("\t");
	Serial.print(newTickValue);
	Serial.print("\t");
	Serial.println(ticks);
}



void mpu6050_test() {
	// if (mpu.getMotionInterruptStatus()) {
		/* Get new sensor events with the readings */
	sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);

	/* Print out the values */
	// Serial.print("AccelX:");
	Serial.print(a.acceleration.x);
	Serial.print(" ");
	// Serial.print("AccelY:");
	Serial.print(a.acceleration.y);
	Serial.print(" ");
	// Serial.print("AccelZ:");
	Serial.print(a.acceleration.z);
	Serial.print(" ");
	// Serial.print("GyroX:");
	Serial.print(g.gyro.x);
	Serial.print(" ");
	// Serial.print("GyroY:");
	Serial.print(g.gyro.y);
	Serial.print(" ");
	// Serial.print("GyroZ:");
	Serial.print(g.gyro.z);
	Serial.println("");

	// }

	delay(3);
}


void car_jack_test() {
	while (Serial.available() == 0) {}
	String s = Serial.readString();
	s.trim();

	if (s == "u" and up == 0) {
		Serial.println("going up");
		jack_upward();
		up = 1;
	}
	if (s == "d") {
		Serial.println("going down");
		jack_downward();
		up = 0;
	}
}


void motor_test() {
	if (Serial.available() > 0){
		String s = Serial.readStringUntil('\n');
		s.trim();

		int index = s.indexOf(' ');

		String left = s.substring(0, index);
		String right = s.substring(index+1);

		int left_speed = left.toInt();
		int right_speed = right.toInt();

		if(DEBUG){
			Serial.println(left_speed);
			Serial.println(right_speed);
			Serial1.println("-----");
		}

		wheels.leftMotor(left_speed);
		wheels.rightMotor(right_speed);
	}

}