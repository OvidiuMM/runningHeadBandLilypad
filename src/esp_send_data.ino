 /*
  Simple example for receiving
  https://github.com/sui77/rc-switch/
*/

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//******Pushbullet code
const char* ssid = "*****"; //replace with your own SSID
const char* password = "*****"; //replace with your SSID password

const char* host = "api.pushbullet.com";
const int httpsPort = 443;
const char* PushBulletAPIKEY = "**************"; //get it from your pushbullet account

const char* temp = "tmp";
const char* light = "lgt";
const char* speed_ = "ms";

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "E7 06 F1 30 B1 5F 25 72 00 4D 99 F0 ED 90 4D F9 60 D3 FB 75"; //got it using https://www.grc.com/fingerprints.htm

 WiFiClientSecure client;
//***** end Pushbullet code

//******Serial code
SoftwareSerial Lilly1(3, 2); // RX | TX
String W =" ";
char w ;
//******end serial code


void setup() {
  Serial.begin(9600);
  //******Serial code
  Lilly1.begin(9600);
  //******end Serial code
  
  //******Pushbullet code
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
 delay(1000);
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  else
  { 
    Serial.println("connection succeed");
    
    }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }
 pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  
   //process data
  if (Lilly1.available())                
         { w = Lilly1.read() ;
           Serial.print(w);
           W = W + w ;                    // Vamos montando un String con lo que entra
         }
      if (Serial.available())             // Lo que entra por Serial à WIFI
         {  char s = Serial.read();
            Lilly1.print(s);
         }
      if ( w == '\n')                     // Sin han pulsado intro
         { if (( W.indexOf(temp) > 0 )  &&  ( W.indexOf(light) > 0 )  &&  ( W.indexOf(speed_) > 0 ))
              { 
                //complete data recieved from lilypad              
                 int end_1 = W.indexOf(";");
                 int end_2 = W.indexOf(";", end_1);
                 int end_3 = W.indexOf(";", end_2);
                 Serial.println("Data recieved");
                 
                 String _temp_var = W.substring(W.indexOf(temp), end_1) ;
                 String _light_var = W.substring(W.indexOf(light), end_2) ;
                 String _speed_var = W.substring(W.indexOf(temp), end_3) ;
                 send_data(_temp_var,_light_var, _speed_var);
               }
           W = "" ;  w = ' ' ;                    // Limpiamos las variables
         }
    
}
void send_data(String _temp_var, String _light_var, String _speed_var){
 
       String messagebody = "{\"type\": \"note\", \"title\": \"RunningBand\", \"body\": \"";
       String final_mess = "\"}\r\n";
       String body_value = " Temp value: " + _temp_var + " Light value: " + _light_var + " Speed: " + _speed_var ;
       String all = messagebody+body_value+final_mess;
       //print the response
       sendMessage(all);
       while (client.available() == 0);

       while (client.available()) {
       String line = client.readStringUntil('\n');
       Serial.println(line);
       }
  //***** end Pushbullet code
}

void sendMessage(String messagebody){
   String url = "/v2/pushes";
 
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer " + PushBulletAPIKEY + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " +
               String(messagebody.length()) + "\r\n\r\n");
  client.print(messagebody);
  Serial.println("request sent");
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)
  delay(1000);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}
