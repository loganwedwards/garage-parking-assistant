#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define ledPin 3	 // input pin Neopixel is attached to, (D7)
#define NUMPIXELS 8 // number of neopixels in Ring
#define pingPin 4 // Trigger Pin
#define buttonPin 6 // momentary button for training mode
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);

int delayval = 100; // timing delay


int redColor = 0;
int greenColor = 0;
int blueColor = 0;
int buttonState = 0;
long setpoint = 50; // distance in cm
long memorySetpoint;
int maximumRange = 280; // PING))) Max Detection = 300cm                                                                                                                                                                                                                                                            ; // Maximum range needed
int minimumRange = 3; // PING))) Min Detection = 2cm
long duration, distance; // Duration used to calculate distance

// Smoothing and timer for idle operation
const int numReadings = 4;
const int setpointAddress = 0; // EEPROM memory address
long readings[numReadings];
int readIndex = 0;
long total = 0;
float average = 0.0;

void setup() {
  Serial.begin(9600);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // Enable for debugging
  while (!Serial) {
    ;
  }
  EEPROM.get(setpointAddress, memorySetpoint);
  Serial.print("Memory Setpoint ");
  Serial.println(memorySetpoint);
  if (memorySetpoint > 0) {
    setpoint = memorySetpoint;
  }
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  pixels.setBrightness(128);
  pixels.begin(); // Initializes the NeoPixel library.
  pixels.show();
  
  for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i,255, 255, 255); // white
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(50); // Delay for a period of time (in milliseconds).
    
    }
}

void loop() {
  Serial.print("[Distance: ");
  Serial.print(distance);
  Serial.print(" ] / [ Setpoint: ");
  Serial.print(setpoint);
  Serial.println(" ]");
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  Serial.print("Button state ");
  Serial.println(buttonState);
  if (buttonState == LOW) {
    // naive smoothing
    // src: https://www.arduino.cc/en/Tutorial/Smoothing
    distance = getDistance();
    total = total - readings[readIndex];
    readings[readIndex] = distance;
    total = total + distance;
    readIndex = readIndex + 1;
    if (readIndex >= numReadings) {
      readIndex = 0;
    }

    average = total / numReadings;
    float value = (average - distance) / average;
    float variation = abs(value) * 100.0;
    //  Serial.print("Value ");
    //  Serial.print(value);
    //  Serial.print("    Distance ");
    //  Serial.print(distance);
      Serial.print("    Average ");
      Serial.println(average);
    //  Serial.print("    Variation ");
    //  Serial.println(variation);

    // If there is less than 1% change, consider us at idle state
    // since the unit will spend most of its life here.
    if (variation < 1) {
      setIdle();
      delay(500);
    } else {
      // normal operating mode
      // Serial.println("Mode = Ready");
      // Serial.println(distance);
    if (distance <= setpoint) {
      setDanger(); 
    } else if (distance > maximumRange) {
      setIdle(); 
    } else {
      // this represents the mapped index of the number of pixels
      // to show as yellow
      int index = map(average, minimumRange, maximumRange, 0, NUMPIXELS-1);
      setRange(index);
    }
    }
  }
   else {

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    Serial.print("Setting new treshold ");
    Serial.print(distance);
    // training mode on
    for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0, 128, 255, 0)); // purple?
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
    
    setpoint = distance;
    EEPROM.put(setpointAddress, setpoint);
  }
}


long getDistance() {
 pinMode(pingPin, OUTPUT);
 /* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(pingPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(pingPin, HIGH);
 delayMicroseconds(5); 
 
 digitalWrite(pingPin, LOW);
 pinMode(pingPin, INPUT);
 duration = pulseIn(pingPin, HIGH);
// Serial.println(duration);
 //Calculate the distance (in cm) based on the speed of sound.
 return duration / 29 /2;
}

void setRange(int index) {
  pixels.setBrightness(240);
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    if (i >= index) {
       pixels.setPixelColor(i, pixels.Color(255, 75, 10)); // amber/yellow
    } else {
       pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // green
    }

    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
    }
}

void setIdle() {
  pixels.setBrightness(25);
  for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0, 0, 255)); // blue
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
}

void setDanger() {
  pixels.setBrightness(240);
  for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // red
      pixels.show(); // This sends the updated pixel color to the hardware.
  }
  delay(500);
  for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // off
      pixels.show(); // This sends the updated pixel color to the hardware.
  }
  delay(500);
    
}
