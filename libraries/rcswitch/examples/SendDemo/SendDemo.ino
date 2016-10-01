/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(115200);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(16);

  // Optional set pulse length.
    
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}

void loop() {

  /* Same switch as above, but using decimal code */
  mySwitch.send(1381827, 24);
  delay(2000);  
  mySwitch.send(1381836, 24);
  delay(2000);  
}
