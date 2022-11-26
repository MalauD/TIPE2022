#include "interface.hpp"

void CalInterface::start()
{
    Serial.println("Starting calibration");
    Serial.println("Test:");
    this->adc.begin();
    this->adc.set_gain(GAIN_ONE);
    auto reading = this->adc.one_shot_reading(0);
    Serial.print("Adc1: ");
    Serial.print(reading.getAdcValueByAddr(AdcAddr::ADC1));
    Serial.print(" Adc2: ");
    Serial.print(reading.getAdcValueByAddr(AdcAddr::ADC2));
    Serial.print(" Adc1: ");
    Serial.print(reading.getAdcValueByAddr(AdcAddr::ADC3));
    Serial.print(" Adc1: ");
    Serial.println(reading.getAdcValueByAddr(AdcAddr::ADC4));
    Serial.println("Enter value to start calibration");
    while (!Serial.available())
        ;

    while (true)
    {
        int16_t val = Serial.parseInt();
        if (val != 0)
        {
            Serial.print("Value: ");
            Serial.println(val);
            this->dataSet.appendDataPoint({val,
                                           this->adc.one_shot_reading(0).getAdcValueByAddr(AdcAddr::ADC1)});
        }
    }
}