/*
 ESP8266 Blink by Simon Peter
 Blink the blue LED on the ESP-01 module
 This example code is in the public domain
 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

#define ON 12
#define OFF 13
#define SENSOR 14
int PREVIOUS;


void click(int pin) {
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
	delay(200);
	digitalWrite(pin, LOW);
	pinMode(pin, INPUT);
}

void setup() {
	Serial.begin(115200);
	//pinMode(ON, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
	//pinMode(OFF, OUTPUT);
	pinMode(SENSOR, INPUT_PULLUP);
	click(ON);
	delay(2000);
	click(OFF);
}


void sensor() {
	if(digitalRead(SENSOR) == 1){
		click(ON);
		PREVIOUS = 1;
	}
	else if(digitalRead(SENSOR) == 0) {
		click(OFF);
		PREVIOUS = 0;
	}
}

// the loop function runs over and over again forever
void loop() {
	int val = digitalRead(SENSOR);
	if(PREVIOUS != val) {
		click(val?ON:OFF);
		PREVIOUS=val;
	}
		//sensor();
	delay(1000);
	/*click(ON);              // Wait for two seconds (to demonstrate the active low LED)
	delay(2000);
	click(OFF);
	delay(2000);*/
}
