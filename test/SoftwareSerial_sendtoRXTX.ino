/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX, TX
const String temp =  "tmp";
const String light = "lgt";
const String speed_ = "ms";
String send_mess = temp + "=4;" + light + "=5;" + speed_ + "=6";

 
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
 
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

   pinMode(LED_BUILTIN, OUTPUT);
}

void loop() { // run over and over
  if (mySerial.available()) {
    mySerial.println(send_mess);
    Serial.println("Sent: ");
    Serial.println(send_mess);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
    
  }
  else {
    Serial.println("Not connected!");
    delay(5000);
    }
}

