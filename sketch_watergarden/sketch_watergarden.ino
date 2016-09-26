#include <SimpleTimer.h>

//define all variables

/*
  define Pin Configuration
*/
int moistureProbeInputPin = A0;
int waterPumpPin = 2;
int loopPumpPin = 3;

/*
   define soil dry tolerances min & max
   (Max)  1023  = No Moisture - completely dry
          800   = start the water cycle
          600   = Very Damp - stop watering
          <300  = Completely Saturated - call 911
*/
int waterMoistureLevel = 823;

/*
  delay the time between checking moisture value
  and to send the reading to the server
*/
unsigned long pollMillis = 6000; //10 minutes(600000 (10 seconds for testing6000)

/*
   When soil is dry enough to water
   keep the pump on for the time defined below in milliseconds
*/
int pumpOnMillis = 3000; //water for 3 seconds

/*
     Assign SimpleTimer function to a variable
*/
SimpleTimer timer;

void setup() {
  // put your setup code here, to run once:

  /*
    Setup the pin modes for each pin declared
  */
  pinMode(moistureProbeInputPin, INPUT);
  pinMode(waterPumpPin, OUTPUT);
  pinMode(loopPumpPin, OUTPUT);

/*
 * Send the variables to the timer.run funtion
 */
  timer.setInterval(pollMillis, poll);

  /*
     initialize serial communications at 9600 bps
  */
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  timer.run(); //start the timer funtion
}

/*
   Main funtion that is run once per poll
   called from timer.run
*/
void poll() {
  Serial.print("Current mosture level ");
  int moisture = getSoilMoisture();
  int watered = 0;
  Serial.println(moisture);
  if (moisture > waterMoistureLevel) {
    waterThePlant();
    watered = 1;
    Serial.println("Plants are being watered.");
  }

  /*
     If the moisture level is OK
     do not water and pause to check the moisture level again in 2 hours
  */
  if (moisture < waterMoistureLevel) {
    Serial.println("The soil is still wet, not watering yet");
    delay(7200);
  }
}

/*
   Read the current moisture level in the soil
   through pin A0
*/
int getSoilMoisture() {
  int reading = analogRead(moistureProbeInputPin);
  return reading;
}

/*
   Turn on the pump (led for testing) to
   water the plant
*/
void waterThePlant() {
  Serial.println("Watering the plant for 3 seconds.");
  digitalWrite(waterPumpPin, HIGH); //turn the pump on
  delay(pumpOnMillis); //leave the pump on for pumpOnMillis(3 secs)
  digitalWrite(waterPumpPin, LOW); //turn the pump off
  Serial.println("Waiting 10 seconds for water to absorb, before measuring again.");
  delay(10000); //wait 10 seconds
  int moisture = getSoilMoisture();
  if (moisture > waterMoistureLevel) {
    Serial.print("Soil is too dry, ");
    Serial.print(moisture);
    Serial.println(", keep watering");
    digitalWrite(loopPumpPin, HIGH);
    waterThePlant();
    digitalWrite(loopPumpPin, LOW);
    Serial.println(" We have watered enough, turning pump off");
    delay(5000); //wait 5 seconds
  }
}

