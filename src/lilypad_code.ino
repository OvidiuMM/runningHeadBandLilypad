
/*************************************************************
  Download latest from github:
  https://github.com/OvidiuMM/runningHeadBandLilypad

  Code is used for controlling a LilyPad based head band.
  Check github description 

 *************************************************************
  This example runs directly on LilyPad chip.

  Please be sure to select the right LilyPad module
  in the Tools -> Board menu!
 *************************************************************/

#define BANDGAPREF 14   // special indicator that we want to measure the bandgap
#define VIBRA 6
#define LED_W 9 
#define LED_R  11 
#define SCL_PIN A5 
#define SDA_PIN A4
#define TEMP_SENSOR A3 
#define LIGHT_SENSOR A0


const int temp_min_vibr = 6 ;
int warning_repetitions = 5;
int light_reading = 0;
int led_brightness = 0;
int start_time = 0;
int end_time = 0;
int time_elapsed = 0;
int temp_actual = 0;
void setup() {
   Serial.println("Start Smart Band...");
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_W, OUTPUT); 
  pinMode(LED_R, OUTPUT);  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TEMP_SENSOR, INPUT);
  pinMode(LIGHT_SENSOR, INPUT);
   pinMode(VIBRA, OUTPUT);
  digitalWrite(VIBRA,LOW);
   Serial.println("Blink board led  ");
   Serial.print(warning_repetitions);
   Serial.println("  repetitions");
  led_control(true, warning_repetitions);
  start_time =  millis();
  
}

void loop() {
  // read milliseconds since program is running (50 days max)
   end_time = millis();
   Serial.println("Reading temperature...");
   // Get the current temperature read
   int  tempRead= analogRead(TEMP_SENSOR);
   delay(10000);  
   int tempValue = temperature_calculator(tempRead);
  delay(10000);  
   // Print some descriptive text and then the value from the sensor
    Serial.println("Temperature value is:");
    Serial.println(tempValue);

     Serial.println("Check vibration needed...");
    //test if temperature is bellow the value we set and vibrate if so
    vibrate(tempValue);
    temp_actual = tempValue;
    //get the light sensor readings and set the required leds value

    light_reading = analogRead(LIGHT_SENSOR);
     Serial.println("light_reading:");
    Serial.println(light_reading);
    light_reading = 1023 - light_reading ;
    //now we have to map 0-1023 to 0-255 since thats the range analogWrite uses

    led_brightness = map(light_reading, 0, 1023, 0, 255);
    Serial.println("led brghtness:");
    Serial.println(led_brightness);
    if (led_brightness <= 130){
      led_brightness = 0;
      }
    // start lights blinking
    // light sensor will be added to blink only when the darkness is near
    Serial.println("Blink leds");
     
    led_control(false, led_brightness);
   
    time_elapsed = end_time - start_time;
    delay(2000);
}



/*
 Control all leds
 If digital is selected, pwm_led bool var, board led will blik for "limit" var define number
 Otherwise the external leds will be powered from 0 to 255 to "limit" var 
*/
void led_control(bool pwm_led, int limit){
    Serial.println("*****");
  Serial.println("led brghtness 2:");
    Serial.println(limit);
    Serial.println("*****");
 if (pwm_led == true) {
  for (int i=0; i <= limit; i++){
     digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(200);                       // wait for a second
     digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
     delay(200);    
   }
  }
 else {
  if (limit > 0) {
     for (int i=0; i <= limit; i++){
    analogWrite(LED_W, i);
   analogWrite(LED_R, i);
     delay(10);    
   }  
    }
  
   analogWrite(LED_W, limit);
   analogWrite(LED_R, limit);
   
 }
}

void vibrate(int temperature){
  if ((temperature <= temp_min_vibr) && (warning_repetitions > 0)){
     warning_repetitions--;
     led_control(true, warning_repetitions);
    for (int i=0; i <= 5; i++){
     digitalWrite(VIBRA, HIGH);
     delay(1000);
     digitalWrite(VIBRA, LOW);  
     
   }    
 }
}
  
int temperature_calculator(int temp_read){
  Serial.println("*************");
   Serial.println("*************");
   Serial.println("***** temperature readings ********");
   // get voltage reading from the secret internal 1.05V reference
  int refReading = analogRead(BANDGAPREF);  
  refReading = analogRead(BANDGAPREF); 
  refReading = analogRead(BANDGAPREF); 
   Serial.print("***** refReading  ");
   Serial.println(refReading); 
  
  // now calculate our power supply voltage from the known 1.05 volt reading
  //float supplyvoltage = (1.05 * 1024) / refReading;
  float supplyvoltage = (1.1 * 1024) / refReading;
   Serial.print("supplyvoltage  ");
   Serial.println(supplyvoltage);
   if (supplyvoltage > 5.5) {
    return temp_actual;
    }

   
  //getting the voltage reading from the temperature sensor
  int reading = temp_read;//analogRead(sensorPin);  
     Serial.print("reading  ");
   Serial.println(reading);
  // converting that reading to voltage
  float voltage = reading * supplyvoltage / 1024; 
   Serial.print("voltage  ");
   Serial.println(voltage);
  float temperatureC = (voltage - 0.5) * 100 ;   
  //converting from 10 mv per degree wit 500 mV offset
  //to degrees ((volatge - 500mV) times 100)
   Serial.print("***** final temperature  ");
   Serial.println(temperatureC); 
   Serial.println("***** end temperature readings ********");     
   Serial.println("*************");
   Serial.println("*************");
   delay(10000);                         
  return temperatureC;
}
  
