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
#include <Adafruit_HX711.h>
#include "Wire.h"
#include <SD.h>

// analog inputs: 
// PT: pressure transducer
// LOAD: load cell
#define PT_INPUT A0
#define LOAD_IN_1 A10
#define LOAD_IN_2 A11

char buffer[50];
// digital inputs via I2C: BME680 (can read temp, pressure, humidity, gas)
// Note from Sam: I was too lazy (and don't have time) to configure the I2C setup from the ground up.
// It can be done, but will need someone with substantial serial communication knowledge and lots of time on their hands.
Adafruit_BME680 bme;    // initializes I2C object
File myFile;

// Define the pins for the HX711 communication (load cell)
const uint8_t DATA_PIN = 18;  // Can use any pins!
const uint8_t CLOCK_PIN = 19; // Can use any pins!

Adafruit_HX711 hx711(DATA_PIN, CLOCK_PIN);

const int chipSelect = BUILTIN_SDCARD;
void setup() {
    // Teensy 4.1 default baud rate is 9600, or 6 Mbits/s
    Serial.begin(9600);
    
    Serial.println("initialiing SD card");
    if (!SD.begin(chipSelect)){


        Serial.println("initialization failed");
        return;
    }
    Serial.println("initialization done");
    
    // configure pin inputs/outputs.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PT_INPUT, INPUT);
    pinMode(LOAD_IN_1, INPUT);
    pinMode(LOAD_IN_2, INPUT);
    myFile = SD.open("sensorData.txt", FILE_WRITE);

    // begin I2C object. See Adafruit.BME680.h for interface details
    bme.begin();

      // Initialize the HX711
    hx711.begin();

    // read and toss 3 values each
    Serial.println("Tareing....");
    for (uint8_t t=0; t<3; t++) {
        hx711.tareA(hx711.readChannelRaw(CHAN_A_GAIN_128));
        hx711.tareA(hx711.readChannelRaw(CHAN_A_GAIN_128));
        hx711.tareB(hx711.readChannelRaw(CHAN_B_GAIN_32));
        hx711.tareB(hx711.readChannelRaw(CHAN_B_GAIN_32));
    }
}

// the loop function runs over and over again forever
void loop() {
    // uncomment each section when ready to test
  
    // sample blink program
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(250);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(250);                      // wait for a second

    
    // pressure transducer outputs
    int pt = analogRead(PT_INPUT);  // should output a number between 0 and 1023.
                                    // theoretically translates to 0V - 3.3V.
    Serial.print("PT input: ");
    Serial.println(pt);
    
    
    
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
    
    
    

    // I2C reading from BME680
    float temp = bme.readTemperature();
    Serial.print("Temp: ");
    Serial.println(temp);
    snprintf(buffer, sizeof(buffer), "The value of heat is: %.2f\n", temp);
    myFile.write(buffer);
    

    
    // RFM95W LoRa Radio is included in seperate scripts:
    // 900MHz_TX_Test.cpp
    // 900MHz_RX_Test.cpp
    
    

    delay(50);    // delay 50ms
    // myFile.flush();

    // Read from Channel A with Gain 128, can also try CHAN_A_GAIN_64 or CHAN_B_GAIN_32
    // since the read is blocking this will not be more than 10 or 80 SPS (L or H switch)
    int32_t weightA128 = hx711.readChannelBlocking(CHAN_A_GAIN_128);
    Serial.print("Channel A (Gain 128): ");
    Serial.println(weightA128);

    // Read from Channel A with Gain 128, can also try CHAN_A_GAIN_64 or CHAN_B_GAIN_32
    int32_t weightB32 = hx711.readChannelBlocking(CHAN_B_GAIN_32);
    Serial.print("Channel B (Gain 32): ");
    Serial.println(weightB32);
    // if( count  5) {

    //     myFile.close();
    // }
}
