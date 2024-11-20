#pragma once

#include·"Arduino.h"                                                                    #include·<Adafruit_BME680.h>
#include·<Adafruit_I2CDevice.h>
#include·<Adafruit_Sensor.h>
#include·"Wire.h"
#include·<SPI.h>
#include·<RH_RF95.h>

class Receive {
public: 
    /* Initializes the pins and ports of the rf95 */
    Receive();
    
    /* Tries to find the message, and prints out the message's output */
    void _getMessage(int bufferSize);

    /* Changes the frequency of the radio */
    void _changeFrequency(double freq);

private:
    /* Returns an ACK to the transimitter */
    void _sendAck();

    /* Singleton instance of the radio driver */
    RH_RF95 rf95;

    double RF95_FREQ = 915.0;
}
