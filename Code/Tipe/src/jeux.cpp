/* #include <Adafruit_ADS1X15.h>
#include <Arduino.h>

Adafruit_ADS1115 ads;

#ifndef IRAM_ATTR
#define IRAM_ATTR
#endif

volatile bool new_data = false;
void IRAM_ATTR reading_available()
{
    new_data = true;
}

void setup()
{
    Serial.begin(115200);
    ads.setGain(GAIN_ONE);
    ads.setDataRate(RATE_ADS1115_860SPS);
    ads.begin();

    pinMode(5, INPUT);
    attachInterrupt(digitalPinToInterrupt(14), reading_available, FALLING);

    ads.startADCReading(ADS1X15_REG_CONFIG_MUX_SINGLE_0, true);
}

int16_t max_reading = INT16_MAX;

void loop()
{
    if (!new_data)
    {
        return;
    }
    int16_t results = ads.getLastConversionResults();
    if (results < max_reading)
    {
        max_reading = results;
        Serial.println(results);
    }
    new_data = false;
}
 */