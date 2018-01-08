/*RC-TX recieve example
*/

#include <SoftwareSerial.h>

SoftwareSerial Serial1(3, 2); //recieve communication on ports 3&2 Tx&Rx

void setup()
  {  
    Serial.begin(9600);
    Serial1.begin(9600);
  }

void loop()
  {
     if (Serial1.available())
         { 
          char c = Serial1.read() ;
          Serial.print(c); //print the recieved stream
         }
     if (Serial.available())
         {  
          char c = Serial.read();
          Serial1.print(c); //print the recieved stream
         }
   }
