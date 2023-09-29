#include <Arduino.h>


class BTSWheels {
private:
	int LEFT_R_PWM = 6;
	int LEFT_L_PWM = 5;
	int RIGHT_R_PWM = 10;
	int RIGHT_L_PWM = 9;

	int MAX_SPEED = 255;

	int lowStopBound = 472;
	int highStopBound = 552;

	void leftForward(int a) {
		a = min(a, MAX_SPEED);
		a = max(a, 0);

		// Serial.print("left motor running forward at speed: ");
		// Serial.println(a);
		
		analogWrite(LEFT_R_PWM, a);
		analogWrite(LEFT_L_PWM, 0);
	}
	void leftBackward(int a) {
		a = min(a, MAX_SPEED);
		a = max(a, 0);

		// Serial.print("left motor running backward at speed: ");
		// Serial.println(a);
		
		analogWrite(LEFT_R_PWM, 0);
		analogWrite(LEFT_L_PWM, a);
	}
	void rightForward(int a) {
		a = min(a, MAX_SPEED);
		a = max(a, 0);

		// Serial.print("right motor running forward at speed: ");
		// Serial.println(a);
		
		analogWrite(RIGHT_R_PWM, 0);
		analogWrite(RIGHT_L_PWM, a);
	}
	void rightBackward(int a) {
		a = min(a, MAX_SPEED);
		a = max(a, 0);
		
		// Serial.print("right motor running backward at speed: ");
		// Serial.println(a);

		analogWrite(RIGHT_R_PWM, a);
		analogWrite(RIGHT_L_PWM, 0);
	}

public:
	int leftSpeed=100;
	int rightSpeed=100;

	void init( int left_r=5, int left_l=6, int right_r=9, int right_l=10 ) {
        this->RIGHT_L_PWM = right_l;
        this->RIGHT_R_PWM = right_r;
        this->LEFT_L_PWM = left_l;
        this->LEFT_R_PWM = left_r;

		pinMode(RIGHT_R_PWM, OUTPUT);
		pinMode(RIGHT_L_PWM, OUTPUT);
		pinMode(LEFT_R_PWM, OUTPUT);
		pinMode(LEFT_L_PWM, OUTPUT);
	}

	void start(int a){
		this->leftSpeed = a;
		this->rightSpeed = a;
	}

	void leftMotor(int a) {
		if (a >= 0) this->leftForward(a);
		else leftBackward(-a);
	}

	void rightMotor(int a) {
		// this->rightSpeed = a;
		if (a >= 0) this->rightForward(a);
		else this->rightBackward(-a);
	}

	void rotateRight(){
		this->leftForward(100);
		this->rightBackward(100);
	}

	void rotateLeft(){
		this->leftBackward(100);
		this->rightForward(100);
	}
};