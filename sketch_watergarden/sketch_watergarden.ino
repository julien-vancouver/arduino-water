#include <SimpleTimer.h>

//define all variables

/*
  define Pin Configuration
*/
int moistureProbeInputPin = A0;
int waterPumpPin = 2;

/*
   define soil dry tolerances min & max
   (Max)  1023  = No Moisture - completely dry
          800   = start the water cycle
          600   = Very Damp - stop watering
          <300  = Completely Saturated - call 911
*/
int waterMoistureLevel = 800;

/*
  delay the time between checking moisture value
  and to send the reading to the server
*/
unsigned long pollMillis = 3600000; //1 hour(3600000)

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
  pinMode(waterPumpPin, OUTPUT); //solenoid pin for the water pump
  digitalWrite(waterPumpPin, LOW); // make sure the pump is OFF

  /*
     Send the variables to the timer.run funtion
  */
  timer.setInterval(pollMillis, poll);

  /*
     initialize serial communications at 9600 bps
  */
  Serial.begin(9600);

  /*
     Data logging to be included later
     Use this section to setup the data logger
     thinking of connecting a wifi shield and storing data on NAS for graphing
     postToServer(water,moisture)

     send the water status (whether the plants got water or not)
     send the moisture level associated with water status
  */
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
     If the moisture level is wet.
     do not water and pause to check the moisture level again in 2 hours (7200000)
  */
  if (moisture < waterMoistureLevel) {
    Serial.println(moisture);
    Serial.println("The soil is still wet, not watering yet");
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
   Turn on the pump to water the plant
*/
void waterThePlant() {
  Serial.println("Watering the plant for 3 seconds.");
  digitalWrite(waterPumpPin, HIGH); //turn the pump on
  delay(pumpOnMillis); //leave the pump on for pumpOnMillis(3 secs)
  digitalWrite(waterPumpPin, LOW); //turn the pump off

  Serial.println("Waiting 10 seconds for water to absorb, before measuring again.");
  delay(10000); //wait 10 seconds

  /*
      Test if soil has been watered enough
      before leaving the watering cycle
  */
  int moisture = getSoilMoisture();
  if (moisture > waterMoistureLevel) {
    waterThePlant();
    delay(5000); //wait 5 seconds
  }
}

