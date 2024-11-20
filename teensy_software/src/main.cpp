/* 
 * This program still largely uses the Arduino setup but it was put together in a PlatformIO environment.
 * Make sure you have all the libraries required to run this program listed below.
 * For platformIO, all the libraries are organized in a particular way that you can see in the project folder structure.
 * To run this on the Arduino IDE, just download the necessary libraries directly in Arduino IDE.
 * 
 * See README file for some notes and pointers
 */

/*
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
*/

// Feather9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX

#include "Arduino.h"
#include <Adafruit_BME680.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
#include "Wire.h"
#include <SPI.h>
#include <RH_RF95.h>

// Custom pinout for teensy 4.1
#define RFM95_CS     10  // "B"
#define RFM95_INT    0 // "C"
#define RFM95_RST    1  // "A"
#define RFM95_IRQN   RFM69_INT

// Change to 434.0 or other frequency, must match RX's freq!
// #define RF95_FREQ 915.0
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  while (!Serial) delay(1);
  delay(100);

  Serial.println("Feather LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void loop() {
  delay(1000); // Wait 1 second between transmits, could also 'sleep' here!
  Serial.println("Transmitting..."); // Send a message to rf95_server

  char radiopacket[20] = "Hello World #      ";
  itoa(packetnum++, radiopacket+13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);
  radiopacket[19] = 0;

  Serial.println("Sending...");
  delay(10);
  rf95.send((uint8_t *)radiopacket, 20);

  Serial.println("Waiting for packet to complete...");
  delay(10);
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  Serial.println("Waiting for reply...");
  if (rf95.waitAvailableTimeout(1000)) {
    // Should be a reply message for us now
    if (rf95.recv(buf, &len)) {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
    } else {
      Serial.println("Receive failed");
    }
  } else {
    Serial.println("No reply, is there a listener around?");
  }

}

