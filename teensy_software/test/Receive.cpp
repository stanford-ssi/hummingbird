#include "Receive.h"

// Custom pinout for teensy 4.1
#define RFM95_CS     10  // "B"
#define RFM95_INT    0 // "C"
#define RFM95_RST    1  // "A"
#define RFM95_IRQN   RFM69_INT
#define TRANSMITTER_POWER 23 

Receive::Receive(){
    // Setting pinouts
    rf95(RFM95_CS, RFM95_INT);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);

    // Initializing radio
    while (!rf95.init()){
        Serial.println("LoRa radio init failed");
        Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    }
    Serial.println("LoRa radio initialized!");

    // Setting frequency of radio
    // Note: Frequency must be same as RX
    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("setFrequency failed");
    }

    // Setting transmit power.
    Serial.print("Setting frequency to: ");
    Serial.println(RFM95_CS);

    rf95.setTxPower(TRANSMITTER_POWER, false);
}

void _getMessage(int bufferSize){
    const int secondBufferSize = 140;
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      digitalWrite(LED_BUILTIN, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");

      Serial.println((char*)buf);

      char secondBuffer[secondBufferSize];
      snprintf(buffer1, sizeof(buffer1), "The value of the received message is: %s\n", (char*)buf);

      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      _sendAck();
    } else {
        Serial.println("Receive failed!");
    }
}

void _sendAck(){
    // ACK message
    uint8_t data[] = "And hello back to you";
    
    // Sending the packet
    rf95.send(data, sizeof(data));
    rf95.waitPacketSent();
    Serial.println("Sent a reply");
    digitalWrite(LED_BUILTIN, LOW);
}

