#include <Wire.h>

void setup() {
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  Serial.begin(115200);         /* start serial for debug */

  pinMode(3, OUTPUT);

  Serial.println("Setting up the Arduino");
}

void loop() {
  delay(100);
}

// function that executes when data is received from master
void receiveEvent(int howMany) {
  String data="";
  while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    data += c;
  }
  Serial.println(data);           /* print the request data */
    // processCall(data);         /* to newline */
}
