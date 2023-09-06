#include <Arduino.h>
#include <HCSR04.h>
#include "BTSWheels.h"
#include "PID.h"

int jack_up = 44;
int jack_down = 45;

// HCSR04 hc(trig, new int[num_of_pins]{echo_pins}, num_of_pins);
HCSR04 hc(46, new int[7]{14, 15, 16, 17, 18, 20, 21}, 7);
BTSWheels wheels;
PID pid(3, 0, 2);


void jack_off(){
	digitalWrite(jack_up, LOW);
	digitalWrite(jack_down, LOW);
}

void jack_upward(){
	digitalWrite(jack_down, LOW);
	digitalWrite(jack_up, HIGH);
	delay(20000);
}

void jack_downward(){
	digitalWrite(jack_up, LOW);
	digitalWrite(jack_down, HIGH);
	delay(20000);
}


void setup() {
	Serial.begin(9600);
	wheels.init(5, 6, 9, 10);
	pinMode(jack_up, OUTPUT);
	pinMode(jack_down, OUTPUT);
	pinMode(52, OUTPUT);

}

void loop() {
	digitalWrite(52, HIGH);
}
