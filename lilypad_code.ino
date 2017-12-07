/*************************************************************
  Download latest:
  https://github.com/OvidiuMM/runningHeadBandLilypad

  Code is for a Smart Head Band buitl with LilyPad board and
  complements
 *************************************************************
  This example runs directly on LilyPad chip.

  Please be sure to select the right LilyPad module
  in the Tools -> Board menu!
 *************************************************************/

#define BANDGAPREF 14   // special indicator that we want to measure the bandgap
#define VIBRA 6
#define LED_W 9 
#define LED_R  11 
#define TEMP_SENSOR A3 

const int temp_min_vibr = 6 ;
int start_time = 0;
int end_time = 0;
int time_elapsed = 0;
void setup() {
   Serial.println("Start Smart Band...");
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_W, OUTPUT); 
  pinMode(LED_R, OUTPUT);  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TEMP_SENSOR, INPUT);
   pinMode(VIBRA, OUTPUT);
  digitalWrite(VIBRA,LOW);
   Serial.println("Blink board led 10 times");
  led_control(true, 10);
  start_time =  millis();
  
}

void loop() {
  // read milliseconds since program is running (50 days max)
   end_time = millis();
   Serial.println("Reading temperature...");
   // Get the current temperature read
   int  tempRead= analogRead(TEMP_SENSOR);
   int tempValue = temperature_calculator(tempRead);
  
   // Print some descriptive text and then the value from the sensor
    Serial.println("Temperature value is:");
    Serial.println(tempValue);

     Serial.println("Check vibration needed...");
    //test if temperature is bellow the value we set and vibrate if so
    vibrate(tempValue);
    
    // start lights blinking
    // light sensor will be added to blink only when the darkness is near
    Serial.println("Blink leds");
    led_control(false, 255);
    delay(2000);
    led_control(false, 0);
    time_elapsed = end_time - start_time;
    delay(2000);
}



/*
 Control all leds
 If digital is selected, pwm_led bool var, board led will blik for "limit" var define number
 Otherwise the external leds will be powered from 0 to 255 to "limit" var 
*/
void led_control(bool pwm_led, int limit){
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
  else{
   analogWrite(LED_W, limit);
   analogWrite(LED_R, limit);
   }
 }
}

void vibrate(int temperature){
  if (temperature <= temp_min_vibr){
    for (int i=0; i <= 5; i++){
     digitalWrite(VIBRA, HIGH);
     led_control(false, 255);
     delay(1000);
     digitalWrite(VIBRA, LOW);  
     led_control(false, 0);
   }    
 }
}
  
int temperature_calculator(int temp_read){
   // get voltage reading from the secret internal 1.05V reference
  int refReading = analogRead(BANDGAPREF);  
   
  // now calculate our power supply voltage from the known 1.05 volt reading
  float supplyvoltage = (1.05 * 1024) / refReading;
  
  //getting the voltage reading from the temperature sensor
  int reading = temp_read;//analogRead(sensorPin);  

  // converting that reading to voltage
  float voltage = reading * supplyvoltage / 1024; 
  float temperatureC = (voltage - 0.5) * 100 ;   
  //converting from 10 mv per degree wit 500 mV offset
  //to degrees ((volatge - 500mV) times 100)
                                               
  return temperatureC;
}
  
