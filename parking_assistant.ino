#include <Adafruit_NeoPixel.h>
/*
ultrasonic code: https://arduinobasics.blogspot.com/2012/11/hc-sr04-ultrasonic-sensor.html
*/
#define PIN 2	 // input pin Neopixel is attached to

#define NUMPIXELS      12 // number of neopixels in Ring
#define echoPin 12 // Echo Pin
#define trigPin 13 // Trigger Pin
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 100; // timing delay


int redColor = 0;
int greenColor = 0;
int blueColor = 0;
const int buttonPin = 4;
int buttonState = 0;
long setpoint = 50; // distance in cm
int maximumRange = 200; // Maximum range needed
int minimumRange = 35; // Minimum range needed
long duration, distance; // Duration used to calculate distance

void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pixels.begin(); // Initializes the NeoPixel library.
  // Serial.begin(9600);
}

void loop() {
  
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // training mode on
    for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0, 0, 255)); // blue
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval); // Delay for a period of time (in milliseconds).
	  distance = getDistance();
    }
    distance = getDistance();
    setpoint = distance;
  } else {
    // normal operating mode
    distance = getDistance();
    if (distance <= setpoint) {
      setDanger(); 
    } else if (distance > maximumRange) {
      setIdle(); 
    } else {
      // this represents the mapped index of the number of pixels
      // to show as yellow
      int index = map(distance, minimumRange, maximumRange, 0, NUMPIXELS-1);
      setRange(index);
    }
  }
  
}


long getDistance() {
 /* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 return duration/58.2; 
}

void setRange(int index) {
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    if (i >= index) {
       pixels.setPixelColor(i, pixels.Color(128, 128, 0)); // amber/yellow
    } else {
       pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // green
    }

    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
    }
}

void setIdle() {
  for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // blue
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval); // Delay for a period of time (in milliseconds).
    }
}

void setDanger() {
  for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // blue
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval); // Delay for a period of time (in milliseconds).
    }
}
