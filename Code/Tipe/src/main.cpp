#include <Adafruit_ADS1X15.h>
#include <Arduino.h>

Adafruit_ADS1115 ads;

void setup()
{
    Serial.begin(115200);
    ads.setGain(GAIN_ONE);
    if (!ads.begin())
    {
        Serial.println("Failed to initialize ADS.");
        while (1)
            ;
    }
}

void loop()
{
    int16_t adc0 = ads.readADC_SingleEnded(0);
    Serial.print("ADC0: ");
    Serial.print(adc0);
    Serial.println("");
    // put your main code here, to run repeatedly:
    delay(1000);
}