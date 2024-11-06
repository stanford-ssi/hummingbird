/* 
 * This program still largely uses the Arduino setup but it was put together in a PlatformIO environment.
 * Make sure you have all the libraries required to run this program listed below.
 * For platformIO, all the libraries are organized in a particular way that you can see in the project folder structure.
 * To run this on the Arduino IDE, just download the necessary libraries directly in Arduino IDE.
 * 
 * See README file for some notes and pointers
 */

#include "Arduino.h"
#include <Adafruit_BME680.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
#include "Wire.h"

// analog inputs: 
// PT: pressure transducer
// LOAD: load cell
#define PT_INPUT A0
#define LOAD_IN_1 A10
#define LOAD_IN_2 A11

// digital inputs via I2C: BME680 (can read temp, pressure, humidity, gas)
// Note from Sam: I was too lazy (and don't have time) to configure the I2C setup from the ground up.
// It can be done, but will need someone with substantial serial communication knowledge and lots of time on their hands.
Adafruit_BME680 bme;    // initializes I2C object

void setup() {
    // Teensy 4.1 default baud rate is 9600, or 6 Mbits/s
    Serial.begin(9600);

    // configure pin inputs/outputs.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PT_INPUT, INPUT);
    pinMode(LOAD_IN_1, INPUT);
    pinMode(LOAD_IN_2, INPUT);

    // begin I2C object. See Adafruit.BME680.h for interface details
    bme.begin();
}

// the loop function runs over and over again forever
void loop() {
    // uncomment each section when ready to test
  
    // sample blink program
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(250);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(250);                      // wait for a second

    /*
    // pressure transducer outputs
    int pt = analogRead(PT_INPUT);  // should output a number between 0 and 1023.
                                    // theoretically translates to 0V - 3.3V.
    Serial.print("PT input: ");
    Serial.println(pt);
    */
    
    /*
    // load cell
    // the load cell is basically a wheatstone bridge and the force reading can be measured by
    // taking the difference between load_in_1 and load_in_2 (voltages measured on opposite ends of the bridge)
    int load_in_1 = analogRead(LOAD_IN_1);
    int load_in_2 = analogRead(LOAD_IN_2);
    int diff = load_in_1 - load_in_2;
    Serial.println("Load cell outputs: ");
    Serial.println(load_in_1);
    Serial.println(load_in_2);
    Serial.print("Diff: ");
    Serial.println(diff);
    */
    
    /*
    // I2C reading from BME680
    float temp = bme.readTemperature();
    Serial.print("Temp: ");
    Serial.println(temp);
    */

    delay(50);    // delay 50ms

}