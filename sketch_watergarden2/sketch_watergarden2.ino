//define all variables

/*
  define Pin Configuration
*/
int moistureProbeInputPin = A0;
int waterPumpPin = 2;
int waterPumpLed = 3;

/*
   define soil dry tolerances min & max
   (Max)  1023  = No Moisture - completely dry
          800   = start the water cycle
          600   = Very Damp - stop watering
          <300  = Completely Saturated - call 911
*/
int waterMoistureLevel = 800;

/*
   declare currentMoistureLevel to be wet to not start the pump.
*/
int currentMoistureLevel = 799;

/*
   decide the max number of watering cycles per day
*/
int max_water_cycles = 10;
int current_water_cycles = 0;


void setup() {
  /*
     open the serial port
  */
  Serial.begin(9600);

  /*
    Setup the pin modes for each pin declared
  */
  pinMode(moistureProbeInputPin, INPUT);
  digitalWrite(waterPumpPin, LOW);  // make sure the pump is OFF
  pinMode(waterPumpPin, OUTPUT);    // set solenoid pin for the water pump
  digitalWrite(waterPumpLed, LOW);  // make sure the LED is OFF
  pinMode(waterPumpLed, OUTPUT);    // set LED pin for the water pump
  
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
     read the value from the moisture sensing probe
  */
  currentMoistureLevel = analogRead(moistureProbeInputPin);

  /*
    Test the moisture level in the soil
  */
  while (currentMoistureLevel > waterMoistureLevel and current_water_cycles < max_water_cycles) {
    digitalWrite(waterPumpPin, HIGH);                         // turn on the pump
    digitalWrite(waterPumpLed, HIGH);                         // set the LED on
    delay(3000);                                              // turn pump on for 3 seconds
    digitalWrite(waterPumpPin, LOW);                          // turn off the pump & leave the LED on to indicate still in water cycle
    delay(10000);                                             //wait 10 seconds for the water to absorb
    currentMoistureLevel = analogRead(moistureProbeInputPin); // read in the moisture value again

    /*
      Quit this after 130 seconds
    */
    current_water_cycles = current_water_cycles + 1;
  }

  /*
    Turn off the pump
  */
  digitalWrite(waterPumpPin, LOW);    // turn off the pump
  digitalWrite(waterPumpLed, LOW);   // set the LED off
}

/*
   Send data back over serial
*/
void sendData() {
  Serial.print("moisture level: ");
  Serial.println(currentMoistureLevel);
  Serial.print("Watered ");
  Serial.print(current_water_cycles);
  Serial.println(" times today");
  delay(10000);
}
