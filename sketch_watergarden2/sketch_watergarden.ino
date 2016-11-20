/*
  20/11/2016
  An automated Arduino Garden system which check the moisture of the soil using two sensors and waters if the moisture level drops below a certain value, using two pumps.
  */

//define all variables
/*
  define Pin Configuration
*/
/*
   Water Moisture Sensors
*/
int moistureProbeInputPins[] = {A4, A5};
int probecount = 0;

/*
   Water Pumps (through relay)
*/
int waterPumpPins[] = {2, 4};
int pumpcount = 0;

/*
   Indicator light showing pumps are on
*/
int waterPumpLed = 7;

/*
   define soil dry tolerances min & max
   (Max)  1023  = No Moisture - completely dry
          800   = start the water cycle
          600   = Very Damp - stop watering
          <300  = Completely Saturated - call 911
*/
int waterMoistureLevel = 800;
int currentMoistureLevel = 600; //declare currentMoistureLevel to be wet to not start the pump.
int currentSensor1Level = 1023;
int currentSensor2Level = 1023;

void setup() {
  /*
     open the serial port
  */
  Serial.begin(9600);

  /*
    Setup the pin modes for each pin declared
  */
  for (probecount = 0; probecount < 2; probecount++) {
    pinMode(moistureProbeInputPins[probecount], INPUT);
  }
  for (pumpcount = 0; pumpcount < 2; pumpcount++) {
    digitalWrite(waterPumpPins[pumpcount], HIGH);  // make sure the pump is OFF
    pinMode(waterPumpPins[pumpcount], OUTPUT);    // set solenoid pin for the water pump
  }

  digitalWrite(waterPumpLed, LOW);  // make sure the LED is OFF
  pinMode(waterPumpLed, OUTPUT);    // set LED pin for the water pump
}

void loop() {
  /*
     Manage the garden
  */
  manageGarden();
  /*
     Send data out over the serial port
  */
  sendData();
}

void manageGarden() {
  /*
     Store the pre-watering moisture Values
  */
  currentSensor1Level = analogRead(moistureProbeInputPins[0]);
  currentSensor2Level = analogRead(moistureProbeInputPins[1]);
  Serial.print("Pre Water moisture level Sensor 1: ");
  Serial.println(currentSensor1Level);
  Serial.print("Pre Water moisture level Sensor 2: ");
  Serial.println(currentSensor2Level);

  /*
     read the value from the moisture sensing probe 1
  */
  currentMoistureLevel = analogRead(moistureProbeInputPins[0]);
  Serial.println("Reading from Sensor 1");
  delay(1000);

  /*
    Test the moisture level in the soil from sensor 1
  */
  while (currentMoistureLevel > waterMoistureLevel) {
    digitalWrite(waterPumpPins[0], LOW);                         // turn on the pump
    digitalWrite(waterPumpLed, HIGH);                         // set the LED on
    Serial.println("Watering from Pump 1");
    delay(5000);                                                   // turn pump on for 5 seconds
    digitalWrite(waterPumpPins[0], HIGH);                          // turn off the pump & leave the LED on to indicate still in water cycle
    Serial.println("Turn Off Pump 1");
    /*
       wait 10 seconds for the water to absorb
    */
    Serial.print("Soaking -");
    delay(2000);
    Serial.print("-");
    delay(2000);
    Serial.print("-");
    delay(2000);
    Serial.print("-");
    delay(2000);
    Serial.println("->");
    delay(2000);
    currentMoistureLevel = analogRead(moistureProbeInputPins[0]);  // read in the moisture value again
    Serial.print("Moisture Sensor 1: ");
    Serial.println(analogRead(moistureProbeInputPins[0]));
  }

  /*
    Turn off the LED
  */
  digitalWrite(waterPumpLed, LOW);   // set the LED off
  /*
     reset currentMoistureLevel and read the value from the moisture sensing probe 2
  */
  currentMoistureLevel = analogRead(moistureProbeInputPins[1]);
  Serial.println("Reading from Sensor 2");
  delay (1000);

  /*
    Test the moisture level in the soil from sensor 2
  */
  while (currentMoistureLevel > waterMoistureLevel) {
    digitalWrite(waterPumpPins[1], LOW);                         // turn on the pump
    digitalWrite(waterPumpLed, HIGH);                         // set the LED on
    Serial.println("Watering from Pump 2");
    delay(5000);                                              // turn pump on for 5 seconds
    digitalWrite(waterPumpPins[1], HIGH);                          // turn off the pump & leave the LED on to indicate still in water cycle
    Serial.println("Turn Off Pump 2");
    /*
       wait 10 seconds for the water to absorb
    */
    Serial.print("Soaking -");
    delay(2000);
    Serial.print("-");
    delay(2000);
    Serial.print("-");
    delay(2000);
    Serial.print("-");
    delay(2000);
    Serial.print("->");
    delay(2000);
    currentMoistureLevel = analogRead(moistureProbeInputPins[1]); // read in the moisture value again
    Serial.print("Moisture Sensor 2: ");
    Serial.println(analogRead(moistureProbeInputPins[1]));
  }

  /*
    Turn off the LED
  */
   digitalWrite(waterPumpLed, LOW);   // set the LED off
}

/*
   Send data back over serial
*/
void sendData() {
  Serial.print("Moisture Sensor 1: ");
  Serial.println(analogRead(moistureProbeInputPins[0]));
  Serial.print("Moisture Sensor 2: ");
  Serial.println(analogRead(moistureProbeInputPins[1]));
  delay(10000);
}
